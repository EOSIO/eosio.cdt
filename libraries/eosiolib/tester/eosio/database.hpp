// copyright defined in LICENSE.txt

#pragma once

#include <eosio/fixed_bytes.hpp>
#include <eosio/name.hpp>
#include <eosio/shared_memory.hpp>
#include <type_traits>

namespace eosio {

/// \group increment_key Increment Key
/// Increment a database key. Return true if the result wrapped.
inline bool increment_key(uint8_t& key) { return !++key; }

/// \group increment_key
inline bool increment_key(uint16_t& key) { return !++key; }

/// \group increment_key
inline bool increment_key(uint32_t& key) { return !++key; }

/// \group increment_key
inline bool increment_key(uint64_t& key) { return !++key; }

/// \group increment_key
inline bool increment_key(uint128_t& key) { return !++key; }

/// \group increment_key
inline bool increment_key(name& key) { return !++key.value; }

/// \group increment_key
inline bool increment_key(checksum256& key) { return increment_key(key.data()[1]) && increment_key(key.data()[0]); }

/// A row in a contract's table
struct contract_row {
   uint32_t                               block_num   = {};
   bool                                   present     = {};
   name                                   code        = {};
   name                                   scope       = {};
   name                                   table       = {};
   uint64_t                               primary_key = {};
   name                                   payer       = {};
   shared_memory<datastream<const char*>> value       = {};
};

/// Pass this to `query_database` to get `contract_row` for a range of keys.
///
/// The query results are sorted by `key`. Every record has a different key.
/// ```c++
/// struct key {
///     name     code        = {};
///     name     table       = {};
///     name     scope       = {};
///     uint64_t primary_key = {};
///
///     // Construct the key from `data`
///     static key from_data(const contract_row& data);
/// };
/// ```
struct query_contract_row_range_code_table_scope_pk {
   struct key {
      name     code        = {};
      name     table       = {};
      name     scope       = {};
      uint64_t primary_key = {};

      // Extract the key from `data`
      static key from_data(const contract_row& data) {
         return {
            .code        = data.code,
            .table       = data.table,
            .scope       = data.scope,
            .primary_key = data.primary_key,
         };
      }
   };

   /// Identifies query type. Do not modify this field.
   name query_name = "cr.ctsp"_n;

   /// Look at this point of time in history
   uint32_t snapshot_block = {};

   /// Query records with keys in the range [`first`, `last`].
   key first = {};

   /// Query records with keys in the range [`first`, `last`].
   key last = {};

   /// Maximum results to return. The wasm-ql server may cap the number of results to a smaller number.
   uint32_t max_results = {};
};

/// \group increment_key
inline bool increment_key(query_contract_row_range_code_table_scope_pk::key& key) {
   return increment_key(key.primary_key) && //
          increment_key(key.scope) &&       //
          increment_key(key.table) &&       //
          increment_key(key.code);
}

/// \output_section Query Database
/// Query the database. `request` must be one of the `query_*` structs. Returns result in serialized form.
/// The serialized form is the same as `vector<vector<char>>`'s serialized form. Each inner vector contains the
/// serialized form of a record. The record type varies with query.
///
/// Use `for_each_query_result` or `for_each_contract_row` to iterate through the result.
template <typename T>
inline std::vector<char> query_database(const T& request) {
   std::vector<char> result;
   query_database(request, [&result](size_t size) {
      result.resize(size);
      return result.data();
   });
   return result;
}

/// Unpack each record of a query result and call `f(record)`. `T` is the record type.
template <typename T, typename F>
bool for_each_query_result(const std::vector<char>& bytes, F f) {
   datastream<const char*> ds(bytes.data(), bytes.size());
   unsigned_int            size;
   ds >> size;
   for (uint32_t i = 0; i < size.value; ++i) {
      shared_memory<datastream<const char*>> record{};
      ds >> record;
      T r;
      *record >> r;
      if (!f(r))
         return false;
   }
   return true;
}

/// Use with `query_contract_row_*`. Unpack each row of a query result and call
/// `f(row, data)`. `row` is an instance of `contract_row`. `data` is the unpacked
/// contract-specific data. `T` identifies the type of `data`.
template <typename T, typename F>
bool for_each_contract_row(const std::vector<char>& bytes, F f) {
   return for_each_query_result<contract_row>(bytes, [&](contract_row& row) {
      T p;
      if (row.present && row.value->remaining()) {
         // todo: don't assert if serialization fails
         *row.value >> p;
         if (!f(row, &p))
            return false;
      } else {
         if (!f(row, (T*)nullptr))
            return false;
      }
      return true;
   });
}

/// \exclude
extern "C" void query_database(void* req_begin, void* req_end, void* cb_alloc_data,
                               void* (*cb_alloc)(void* cb_alloc_data, size_t size));

/// \exclude
template <typename T, typename Alloc_fn>
inline void query_database(const T& req, Alloc_fn alloc_fn) {
   auto req_data = pack(req);
   query_database(
         req_data.data(), req_data.data() + req_data.size(), &alloc_fn,
         [](void* cb_alloc_data, size_t size) -> void* { return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size); });
}

} // namespace eosio
