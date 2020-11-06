#pragma once
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/varint.hpp"

#include "../../core/eosio/to_key.hpp"

#include <algorithm>
#include <cctype>
#include <functional>


/**
 * @brief Macro to define an index.
 * @details This macro allows users to conveniently define an index without having to specify
 * the index template type, as those can be large/unwieldy to type out. It can be used for both primary and secondary indexes.
 *
 * @param index_name    - The index name.
 * @param member_name   - The name of the member pointer used for the index. This also defines the index's C++ variable name.
 */
#define KV_NAMED_INDEX(index_name, member_name) \
   index<eosio::detail::member_pointer_ret_t<&underlying_type_t::member_name>> member_name{eosio::name{index_name}, &underlying_type_t::member_name};

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         int64_t kv_erase(uint64_t contract, const char* key, uint32_t key_size);

         __attribute__((eosio_wasm_import))
         int64_t kv_set(uint64_t contract, const char* key, uint32_t key_size, const char* value, uint32_t value_size, uint64_t payer);

         __attribute__((eosio_wasm_import))
         bool kv_get(uint64_t contract, const char* key, uint32_t key_size, uint32_t& value_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_get_data(uint32_t offset, char* data, uint32_t data_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_it_create(uint64_t contract, const char* prefix, uint32_t size);

         __attribute__((eosio_wasm_import))
         void kv_it_destroy(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_status(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_compare(uint32_t itr_a, uint32_t itr_b);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_key_compare(uint32_t itr, const char* key, uint32_t size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_move_to_end(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_next(uint32_t itr, uint32_t& found_key_size = (uint32_t&)std::move(uint32_t(0)), uint32_t& found_value_size = (uint32_t&)std::move(uint32_t(0)));

         __attribute__((eosio_wasm_import))
         int32_t kv_it_prev(uint32_t itr, uint32_t& found_key_size = (uint32_t&)std::move(uint32_t(0)), uint32_t& found_value_size = (uint32_t&)std::move(uint32_t(0)));

         __attribute__((eosio_wasm_import))
         int32_t kv_it_lower_bound(uint32_t itr, const char* key, uint32_t size, uint32_t& found_key_size = (uint32_t&)std::move(uint32_t(0)), uint32_t& found_value_size = (uint32_t&)std::move(uint32_t(0)));

         __attribute__((eosio_wasm_import))
         int32_t kv_it_key(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_value(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);
      }
   }

namespace detail {
   template <typename R, typename C>
   auto member_pointer_ret_type(R (C::*)) -> R;
   template <typename R, typename C, typename... Args>
   auto member_pointer_ret_type(R (C::*)(Args...)) -> R;
   template <typename R, typename C, typename... Args>
   auto member_pointer_ret_type(R (C::*)(Args...)const) -> R;

   template <auto MP>
   using member_pointer_ret_t = decltype(member_pointer_ret_type(MP));

   struct scoped_buffer {
      constexpr static inline size_t static_size = 512;
      constexpr inline scoped_buffer(std::size_t sz)
         : ptr(sz > static_size ? new char[sz] : buffer.data()),
           sz(sz) {}

      ~scoped_buffer() {
         if (size() > static_size)
            delete[] ptr;
      }

      constexpr inline void reset(std::size_t new_size) {
         if (new_size > size()) {
            if (new_size > static_size) {
               if (size() > static_size)
                  delete[] ptr;
               ptr = new char[new_size];
            }
         }
         sz = new_size;
      }

      constexpr inline char* data() { return ptr; }
      constexpr inline const char* data() const { return ptr; }
      constexpr inline std::size_t size() const { return sz; }

      std::array<char, static_size> buffer = {};
      char* ptr = nullptr;
      std::size_t sz = 0;
   };

   template <typename V>
   static void serialize(const V& value, void* buffer, size_t size) {
      datastream<char*> ds((char*)buffer, size);
      ds << value;
   }

   template <typename V>
   static void deserialize(V& value, const void* buffer, size_t size) {
      datastream<const char*> ds((char*)buffer, size);
      ds >> value;
   }

   template <typename V>
   static size_t get_size(const V& value) {
      auto size = pack_size(value);
      return size + 1;
   }

   template <typename... Vs>
   static size_t get_size(const std::variant<Vs...>& value) {
      auto size = pack_size(value);
      return size;
   }
}

/* @cond PRIVATE */
template <typename T>
inline key_type make_key(T&& t) {
   auto bytes = convert_to_key(std::forward<T>(t));
   return key_type(std::move(bytes));
}

inline key_type make_prefix(eosio::name table_name, eosio::name index_name, uint8_t status = 1) {
   return make_key(std::make_tuple(status, table_name, index_name));
}

inline key_type table_key(const key_type& prefix, const key_type& key) {
   return prefix + key;
}
/* @endcond */

// This is the "best" way to document a function that does not technically exist using Doxygen.
#if EOSIO_CDT_DOXYGEN
/**
 * @brief A function for converting types to the appropriate binary representation for the EOSIO Key Value database.
 * @details The CDT provides implementations of this function for many of the common primitives and for structs/tuples.
 * If sticking with standard types, contract developers should not need to interact with this function.
 * If doing something more advanced, contract developers may need to provide their own implementation for a special type.
 */
template <typename T>
inline key_type make_key(T val) {
   return {};
}
#endif

namespace kv {
template<typename T, eosio::name::raw TableName>
class table;

namespace internal {

   class table_base;

   class index_base {

   public:
      eosio::name index_name;
      eosio::name table_name;
      eosio::name contract_name;

      key_type to_table_key(const key_type& k) const { return prefix + k; }

   protected:
      index_base() = default;

      template <typename KF, typename T>
      index_base(eosio::name index_name, KF&&, T*)
         : index_name{index_name},
           converter(new to_key_converter<KF{}>{}) {}

      key_type get_key(const void* ptr) const { return converter->convert(ptr); }

      void get(const key_type& key, void* ret_val, void (*deserialize)(void*, const void*, std::size_t)) const;

      table_base* tbl;
      key_type prefix;

   private:
      template<typename T, eosio::name::raw TableName>
      friend class eosio::kv::table;
      friend class table_base;
      friend class iterator_base;

      std::unique_ptr<to_key_converter_base> converter;

      virtual void setup() = 0;
   };

   class table_base {
    protected:
      friend class index_base;
      friend class iterator_base;
      eosio::name contract_name;
      eosio::name table_name;

      eosio::name primary_index_name;

      index_base* primary_index;
      std::vector<index_base*> secondary_indices;

      void put(const void* value, void* old_value,
               std::size_t (*get_size)(const void*),
               void (*deserialize)(void*, const void*, std::size_t),
               void (*serialize)(const void*, void*, std::size_t),
               eosio::name payer) {
         uint32_t value_size;

         auto primary_key = primary_index->get_key(value);
         auto tbl_key = make_prefix(table_name, primary_index->index_name) + primary_key;

         auto primary_key_found = internal_use_do_not_use::kv_get(contract_name.value, tbl_key.data(), tbl_key.size(), value_size);

         if (primary_key_found) {
            detail::scoped_buffer buff = {value_size};
            auto copy_size = internal_use_do_not_use::kv_get_data(0, buff.data(), buff.size());

            deserialize(old_value, buff.data(), copy_size);
         }

         for (const auto& idx : secondary_indices) {
            uint32_t value_size;
            auto sec_tbl_key = make_prefix(table_name, idx->index_name) + idx->get_key(value);
            auto sec_found = internal_use_do_not_use::kv_get(contract_name.value, sec_tbl_key.data(), sec_tbl_key.size(), value_size);

            if (!primary_key_found) {
               eosio::check(!sec_found, "Attempted to store an existing secondary index.");
               internal_use_do_not_use::kv_set(contract_name.value, sec_tbl_key.data(), sec_tbl_key.size(), tbl_key.data(), tbl_key.size(), payer.value);
            } else {
               if (sec_found) {
                  detail::scoped_buffer buff = {value_size};
                  auto copy_size = internal_use_do_not_use::kv_get_data(0, buff.data(), buff.size());

                  auto res = memcmp(buff.data(), tbl_key.data(), copy_size);
                  eosio::check(copy_size == tbl_key.size() && res == 0, "Attempted to update an existing secondary index.");
               } else {
                  auto old_sec_key = make_prefix(table_name, idx->index_name) + idx->get_key(old_value);
                  internal_use_do_not_use::kv_erase(contract_name.value, old_sec_key.data(), old_sec_key.size());
                  internal_use_do_not_use::kv_set(contract_name.value, sec_tbl_key.data(), sec_tbl_key.size(), tbl_key.data(), tbl_key.size(), payer.value);
               }
            }
         }

         detail::scoped_buffer buff = {get_size(value)};

         serialize(value, buff.data(), buff.size());

         internal_use_do_not_use::kv_set(contract_name.value, tbl_key.data(), tbl_key.size(), buff.data(), buff.size(), payer.value);
      }

      void erase(const void* value) {
         uint32_t value_size;

         auto primary_key = primary_index->get_key(value);
         auto tbl_key = make_prefix(table_name, primary_index->index_name) + primary_key;
         auto primary_key_found = internal_use_do_not_use::kv_get(contract_name.value, tbl_key.data(), tbl_key.size(), value_size);

         if (!primary_key_found) {
            return;
         }

         for (const auto& idx : secondary_indices) {
            auto sec_tbl_key = make_prefix(table_name, idx->index_name) + idx->get_key(value);
            internal_use_do_not_use::kv_erase(contract_name.value, sec_tbl_key.data(), sec_tbl_key.size());
         }

         internal_use_do_not_use::kv_erase(contract_name.value, tbl_key.data(), tbl_key.size());
      }
   };

   inline void index_base::get(const key_type& key, void* ret_val, void (*deserialize)(void*, const void*, std::size_t)) const {
      uint32_t value_size;
      uint32_t actual_data_size;

      auto success = internal_use_do_not_use::kv_get(contract_name.value, key.data(), key.size(), value_size);
      if (!success) {
         return;
      }

      detail::scoped_buffer buff = {value_size};

      auto copy_size = internal_use_do_not_use::kv_get_data(0, buff.data(), buff.size());

      bool is_primary = index_name == tbl->primary_index_name;
      if (!is_primary) {
         auto success = internal_use_do_not_use::kv_get(contract_name.value, buff.data(), copy_size, actual_data_size);
         eosio::check(success, "failure getting primary key");

         buff.reset(actual_data_size);
         auto pk_copy_size = internal_use_do_not_use::kv_get_data(0, buff.data(), buff.size());
      }

      deserialize(ret_val, buff.data(), buff.size());
   }

   class iterator_base {
   public:
      enum class status {
         iterator_ok     = 0,  // Iterator is positioned at a key-value pair
         iterator_erased = -1, // The key-value pair that the iterator used to be positioned at was erased
         iterator_end    = -2, // Iterator is out-of-bounds
      };

      iterator_base() = default;

      iterator_base(uint32_t itr, status itr_stat, const index_base* index) : itr{itr}, itr_stat{itr_stat}, index{index} {}

      iterator_base(iterator_base&& other) :
         itr(std::exchange(other.itr, 0)),
         itr_stat(std::move(other.itr_stat))
      {}

      ~iterator_base() {
         if (itr) {
            internal_use_do_not_use::kv_it_destroy(itr);
         }
      }

      iterator_base& operator=(iterator_base&& other) {
         if (itr) {
            internal_use_do_not_use::kv_it_destroy(itr);
         }
         itr = std::exchange(other.itr, 0);
         itr_stat = std::move(other.itr_stat);
         return *this;
      }

      bool is_end()const { return itr_stat != status::iterator_end; }

      /**
       * Returns the value that the iterator points to.
       * @ingroup keyvalue
       *
       * @return The value that the iterator points to.
       */
     void value(void* val, void (*deserialize)(void*, const void*, std::size_t)) const {
         using namespace detail;

         eosio::check(itr_stat != status::iterator_end, "Cannot read end iterator");

         uint32_t value_size;
         uint32_t actual_value_size;
         uint32_t actual_data_size;
         uint32_t offset = 0;

         // call once to get the value_size
         internal_use_do_not_use::kv_it_value(itr, 0, (char*)nullptr, 0, value_size);

         detail::scoped_buffer buff = {value_size};
         auto stat = internal_use_do_not_use::kv_it_value(itr, offset, buff.data(), buff.size(), actual_value_size);

         eosio::check(static_cast<status>(stat) == status::iterator_ok, "Error reading value");

         bool is_primary = index->index_name == index->tbl->primary_index_name;
         if (!is_primary) {
            auto success = internal_use_do_not_use::kv_get(index->contract_name.value, buff.data(), actual_value_size, actual_data_size);
            eosio::check(success, "failure getting primary key in `value()`");

            buff.reset(actual_data_size);
            internal_use_do_not_use::kv_get_data(0, buff.data(), buff.size());
         }

         deserialize(val, buff.data(), buff.size());
      }

      key_type key() const {
         uint32_t actual_value_size;
         uint32_t value_size;

         // call once to get the value size
         internal_use_do_not_use::kv_it_key(itr, 0, (char*)nullptr, 0, value_size);

         detail::scoped_buffer buff = {value_size};
         auto stat = internal_use_do_not_use::kv_it_key(itr, 0, buff.data(), buff.size(), actual_value_size);

         eosio::check(static_cast<status>(stat) == status::iterator_ok, "Error getting key");

         return eosio::key_type{buff.data(), actual_value_size};
      }

   protected:
      uint32_t itr;
      status itr_stat;

      const index_base* index;

      int compare(const iterator_base& b) const {
         bool a_is_end = !itr || itr_stat == status::iterator_end;
         bool b_is_end = !b.itr || b.itr_stat == status::iterator_end;
         if (a_is_end && b_is_end) {
            return 0;
         } else if (a_is_end && b.itr) {
            return 1;
         } else if (itr && b_is_end) {
            return -1;
         } else {
            return internal_use_do_not_use::kv_it_compare(itr, b.itr);
         }
      }
   };

}

/**
 * @defgroup keyvalue Key Value Table
 * @ingroup contracts
 *
 * @brief Defines an EOSIO Key Value Table
 * @details EOSIO Key Value API provides a C++ interface to the EOSIO Key Value database.
 * Key Value Tables require 1 primary index, of any type that can be serialized to a binary representation.
 * Key Value Tables support 0 or more secondary index, of any type that can be serialized to a binary representation.
 * Indexes must be a member variable or a member function.
 *
 * @tparam T         - the type of the data stored as the value of the table
  */
template<typename T, eosio::name::raw TableName>
class table : internal::table_base {
public:
   using underlying_type_t = T;
   template<typename K>
   class index;

private:
   using index_base = internal::index_base;

   class base_iterator : public internal::iterator_base {
   public:
      using iterator_base::iterator_base;
      /**
       * Returns the value that the iterator points to.
       * @ingroup keyvalue
       *
       * @return The value that the iterator points to.
       */
      T value() const {
         T val;
         iterator_base::value(&val, &table::deserialize_fun);
         return val;
      }
   };

   class iterator : public base_iterator {
      using base_iterator::itr;
      using base_iterator::itr_stat;
      using base_iterator::index;

      template<typename K>
      friend class index;

   public:
      using status = typename base_iterator::status;

      iterator() = default;

      iterator(uint32_t itr, status itr_stat, const index_base* index) : base_iterator{itr, itr_stat, index} {}

      iterator(iterator&& other) : base_iterator{std::move(other)} {}

      iterator& operator=(iterator&& other) {
         if (itr) {
            internal_use_do_not_use::kv_it_destroy(itr);
         }
         itr = std::exchange(other.itr, 0);
         itr_stat = std::move(other.itr_stat);
         index = std::move(other.index);
         return *this;
      }

      iterator& operator++() {
         eosio::check(itr_stat != status::iterator_end, "cannot increment end iterator");
         itr_stat = static_cast<status>(internal_use_do_not_use::kv_it_next(itr));
         return *this;
      }

      iterator& operator--() {
         if (!itr) {
            itr = internal_use_do_not_use::kv_it_create(index->contract_name.value, index->prefix.data(), index->prefix.size());
         }
         itr_stat = static_cast<status>(internal_use_do_not_use::kv_it_prev(itr));
         eosio::check(itr_stat != status::iterator_end, "decremented past the beginning");
         return *this;
      }

      bool operator==(const iterator& b) const {
         return base_iterator::compare(b) == 0;
      }

      bool operator!=(const iterator& b) const {
         return base_iterator::compare(b) != 0;
      }

      bool operator<(const iterator& b) const {
         return base_iterator::compare(b) < 0;
      }

      bool operator<=(const iterator& b) const {
         return base_iterator::compare(b) <= 0;
      }

      bool operator>(const iterator& b) const {
         return base_iterator::compare(b) > 0;
      }

      bool operator>=(const iterator& b) const {
         return base_iterator::compare(b) >= 0;
      }
   };

   class reverse_iterator : public base_iterator {
      using base_iterator::itr;
      using base_iterator::itr_stat;
      using base_iterator::index;

   public:
      using status = typename base_iterator::status;

      reverse_iterator() = default;

      reverse_iterator(uint32_t itr, status itr_stat, const index_base* index) : base_iterator{itr, itr_stat, index} {}

      reverse_iterator(reverse_iterator&& other) : base_iterator{std::move(other)} {}

      reverse_iterator& operator=(reverse_iterator&& other) {
         if (itr) {
            internal_use_do_not_use::kv_it_destroy(itr);
         }
         itr = std::exchange(other.itr, 0);
         itr_stat = std::move(other.itr_stat);
         index = std::move(other.index);
         return *this;
      }

      reverse_iterator& operator++() {
         eosio::check(itr_stat != status::iterator_end, "incremented past the end");
         itr_stat = static_cast<status>(internal_use_do_not_use::kv_it_prev(itr));
         return *this;
      }

      reverse_iterator& operator--() {
         if (!itr) {
            itr = internal_use_do_not_use::kv_it_create(index->contract_name.value, index->prefix.data(), index->prefix.size());
            itr_stat = static_cast<status>(internal_use_do_not_use::kv_it_lower_bound(itr, "", 0));
         }
         itr_stat = static_cast<status>(internal_use_do_not_use::kv_it_next(itr));
         eosio::check(itr_stat != status::iterator_end, "decremented past the beginning");
         return *this;
      }

      int compare(const reverse_iterator& b) const {
         bool a_is_end = !itr || itr_stat == status::iterator_end;
         bool b_is_end = !b.itr || b.itr_stat == status::iterator_end;
         if (a_is_end && b_is_end) {
            return 0;
         } else if (a_is_end && b.itr) {
            return 1;
         } else if (itr && b_is_end) {
            return -1;
         } else {
            return -(internal_use_do_not_use::kv_it_compare(itr, b.itr));
         }
      }

      bool operator==(const reverse_iterator& b) const {
         return compare(b) == 0;
      }

      bool operator!=(const reverse_iterator& b) const {
         return compare(b) != 0;
      }

      bool operator<(const reverse_iterator& b) const {
         return compare(b) < 0;
      }

      bool operator<=(const reverse_iterator& b) const {
         return compare(b) <= 0;
      }

      bool operator>(const reverse_iterator& b) const {
         return compare(b) > 0;
      }

      bool operator>=(const reverse_iterator& b) const {
         return compare(b) >= 0;
      }
   };

public:
   using iterator = table::iterator;
   using value_type = T;

   /**
    * @ingroup keyvalue
    *
    * @brief Defines an index on an EOSIO Key Value Table
    * @details A Key Value Index allows a user of the table to search based on a given field.
    * The only restrictions on that field are that it is serializable to a binary representation sortable by the KV intrinsics.
    * Convenience functions exist to handle most of the primitive types as well as some more complex types, and are
    * used automatically where possible.
    *
    * @tparam K - The type of the key used in the index.
    */
   template<typename K>
   class index : public index_base {
   public:
      using iterator = table::iterator;
      using table<T, TableName>::index_base::tbl;
      using table<T, TableName>::index_base::table_name;
      using table<T, TableName>::index_base::contract_name;
      using table<T, TableName>::index_base::index_name;
      using table<T, TableName>::index_base::prefix;

      template <typename KF>
      index(eosio::name name, KF&& kf) : index_base{name, std::forward<KF>(kf), (T*)nullptr} {
         static_assert(std::is_same_v<K, std::remove_cv_t<std::decay_t<decltype(std::invoke(kf, std::declval<const T*>()))>>>,
               "Make sure the variable/function passed to the constructor returns the same type as the template parameter.");
      }

      /**
       * Search for an existing object in a table by the index, using the given key.
       * @ingroup keyvalue
       *
       * @param key - The key to search for.
       * @return An iterator to the found object OR the `end` iterator if the given key was not found.
       */
      iterator find(const K& key) const {
         auto t_key = prefix + make_key(key);

         uint32_t itr = internal_use_do_not_use::kv_it_create(contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.data(), t_key.size());

         auto cmp = internal_use_do_not_use::kv_it_key_compare(itr, t_key.data(), t_key.size());

         if (cmp != 0) {
            internal_use_do_not_use::kv_it_destroy(itr);
            return end();
         }

         return {itr, static_cast<typename iterator::status>(itr_stat), this};
      }

      /**
       * Check if a given key exists in the index.
       * @ingroup keyvalue
       *
       * @param key - The key to check for.
       * @return If the key exists or not.
       */
      bool exists(const K& key) const {
         uint32_t value_size;
         auto t_key = prefix + make_key(key);

         return internal_use_do_not_use::kv_get(contract_name.value, t_key.data(), t_key.size(), value_size);
      }

      /**
       * Get the value for an existing object in a table by the index, using the given key.
       * @ingroup keyvalue
       *
       * @param key - The key to search for.
       * @return The value corresponding to the key.
       */
      T operator[](const K& key) const {
         auto opt = get(key);
         eosio::check(opt.has_value(), "Key not found in `[]`");
         return *opt;
      }

      /**
       * Get the value for an existing object in a table by the index, using the given key.
       * @ingroup keyvalue
       *
       * @param key - The key to search for.
       * @return A std::optional of the value corresponding to the key.
       */
      std::optional<T> get(const K& key) const {
         std::optional<T> ret_val;
         auto k = prefix + make_key(key);
         index_base::get(k, &ret_val, &deserialize_optional_fun);
         return ret_val;
      }

      /**
       * Returns an iterator to the object with the lowest key (by this index) in the table.
       * @ingroup keyvalue
       *
       * @return An iterator to the object with the lowest key (by this index) in the table.
       */
      iterator begin() const {
         uint32_t itr = internal_use_do_not_use::kv_it_create(contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, "", 0);

         return {itr, static_cast<typename iterator::status>(itr_stat), this};
      }

      /**
       * Returns an iterator pointing past the end. It does not point to any element, therefore `value` should not be called on it.
       * @ingroup keyvalue
       *
       * @return An iterator pointing past the end.
       */
      iterator end() const {
         return {0, iterator::status::iterator_end, this};
      }

      /**
       * Returns a reverse iterator to the object with the highest key (by this index) in the table.
       * @ingroup keyvalue
       *
       * @return A reverse iterator to the object with the highest key (by this index) in the table.
       */
      reverse_iterator rbegin() const {
         uint32_t itr = internal_use_do_not_use::kv_it_create(contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_prev(itr);

         return {itr, static_cast<typename iterator::status>(itr_stat), this};
      }

      /**
       * Returns a reverse iterator pointing past the beginning. It does not point to any element, therefore `value` should not be called on it.
       * @ingroup keyvalue
       *
       * @return A reverse iterator pointing past the beginning.
       */
      reverse_iterator rend() const {
         return {0, iterator::status::iterator_end, this};
      }

      /**
       * Returns an iterator pointing to the element with the lowest key greater than or equal to the given key.
       * @ingroup keyvalue
       *
       * @return An iterator pointing to the element with the lowest key greater than or equal to the given key.
       */
      iterator lower_bound(const K& key) const {
         auto t_key = prefix + make_key(key);

         uint32_t itr = internal_use_do_not_use::kv_it_create(contract_name.value, prefix.data(), prefix.size());
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.data(), t_key.size());

         return {itr, static_cast<typename iterator::status>(itr_stat), this};
      }

      /**
       * Returns an iterator pointing to the first element greater than the given key.
       * @ingroup keyvalue
       *
       * @return An iterator pointing to the first element greater than the given key.
       */
      iterator upper_bound(const K& key) const {
         auto t_key = prefix + make_key(key);
         auto it = lower_bound(key);

         int32_t cmp;

         if (it.itr == 0 || it.itr_stat == base_iterator::status::iterator_end) {
            cmp = 1;
         } else {
            cmp = internal_use_do_not_use::kv_it_key_compare(it.itr, t_key.data(), t_key.size());
         }
         if (cmp == 0) {
            ++it;
         }

         return it;
      }

      /**
       * Returns a vector of objects that fall between the specifed range. The range is inclusive, exclusive.
       * @ingroup keyvalue
       *
       * @param begin - The beginning of the range (inclusive).
       * @param end - The end of the range (exclusive).
       * @return A vector containing all the objects that fall between the range.
       */
      std::vector<T> range(const K& b, const K& e) const {
         std::vector<T> return_values;

         for(auto itr = lower_bound(b), end_itr = lower_bound(e); itr < end_itr; ++itr) {
            return_values.push_back(itr.value());
         }

         return return_values;
      }

      void setup() override {
         prefix = make_prefix(table_name, index_name);
      }
   };

   /**
    * @ingroup keyvalue
    * Puts a value into the table. If the value already exists, it updates the existing entry.
    * The key is determined from the defined primary index.
    * If the put attempts to store over an existing secondary index, the transaction will be aborted.
    *
    * @param value - The entry to be stored in the table.
    * @param payer - The payer for the entry.
    */
   void put(const T& value, eosio::name payer) {
      T old_value;
      table_base::put(&value, &old_value, &get_size_fun, &deserialize_fun, &serialize_fun, payer);
   }

   /* @cond PRIVATE */
   static void deserialize_optional_fun(void* value, const void* buffer, std::size_t buffer_size) {
      static_cast<std::optional<T>*>(value)->emplace();
      return detail::deserialize(**static_cast<std::optional<T>*>(value), buffer, buffer_size);
   }
   static void deserialize_fun(void* value, const void* buffer, std::size_t buffer_size) {
      return detail::deserialize(*static_cast<T*>(value), buffer, buffer_size);
   }
   static void serialize_fun(const void* value, void* buffer, std::size_t buffer_size) {
      return detail::serialize(*static_cast<const T*>(value), buffer, buffer_size);
   }
   static std::size_t get_size_fun(const void* value) {
      return detail::get_size(*static_cast<const T*>(value));
   }
   /* @endcond */

   /**
    * Removes a value from the table.
    * @ingroup keyvalue
    *
    * @param key - The key of the value to be removed.
    */
   void erase(const T& value) {
      table_base::erase(&value);
   }

protected:
   table() = default;

   template <typename I>
   void setup_indices(I& index) {
      index_base* idx = &index;
      idx->contract_name = contract_name;
      idx->table_name = table_name;
      idx->tbl = this;

      idx->setup();
      secondary_indices.push_back(idx);
   }

   template <typename PrimaryIndex, typename... SecondaryIndices>
   void init(eosio::name contract, PrimaryIndex& prim_index, SecondaryIndices&... indices) {
      validate_types(prim_index);
      (validate_types(indices), ...);

      contract_name = contract;
      table_name = eosio::name{TableName};

      primary_index = &prim_index;
      primary_index->contract_name = contract_name;
      primary_index->table_name = table_name;
      primary_index->tbl = this;

      primary_index->setup();

      primary_index_name = primary_index->index_name;

      (setup_indices(indices), ...);
   }

private:

   constexpr void validate_types() {}

   template <typename Type>
   constexpr void validate_types(Type& t) {
      constexpr bool is_index = std::is_base_of_v<index_base, std::decay_t<Type>>;
      static_assert(is_index, "Incorrect type passed to init. Must be a reference to an index.");
   }
};
} // namespace kv
} // namespace eosio
