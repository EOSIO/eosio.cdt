#pragma once
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/print.hpp"

#include <algorithm>
#include <cctype>
#include <functional>

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         void kv_erase(uint64_t db, uint64_t contract, const char* key, uint32_t key_size);

         __attribute__((eosio_wasm_import))
         void kv_set(uint64_t db, uint64_t contract, const char* key, uint32_t key_size, const char* value, uint32_t value_size);

         __attribute__((eosio_wasm_import))
         bool kv_get(uint64_t db, uint64_t contract, const char* key, uint32_t key_size, uint32_t& value_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_get_data(uint64_t db, uint32_t offset, char* data, uint32_t data_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_it_create(uint64_t db, uint64_t contract, const char* prefix, uint32_t size);

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
         int32_t kv_it_next(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_prev(uint32_t itr);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_lower_bound(uint32_t itr, const char* key, uint32_t size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_key(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);

         __attribute__((eosio_wasm_import))
         int32_t kv_it_value(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size);
      }
   }

struct key_type {
   size_t size;
   std::string buffer;

   bool operator==(key_type k){
      return size == k.size && buffer == k.buffer;
   }
   bool operator!=(key_type k){
      return !(size == k.size && buffer == k.buffer);
   }
};

namespace _key_value_detail {
   constexpr static size_t max_stack_buffer_size = 512;
}

/*
Key transformations
The key-value store could provide a lexicographical ordering of uint8_t on the keys. The contract can create an ordering on top by transforming its keys. Example transforms:

   [x] - uint?_t: Convert to big-endian
   [x] - int?_t: Invert the MSB then convert to big-endian
   [x] - strings: Convert 0x00 to (0x00, 0x01). Append (0x00, 0x00) to the end. This transform allows arbitrary-length strings.
   [x] - case-insensitive strings: Convert to upper-case, then apply the above transform. Assumes ASCII.
   [ ] - floating-point:
         There's some bit manipulations, followed by an endian conversion
         limitations:
            Positive 0 and Negative 0 map to the same value
            NaN's and inf's end up with an unusual ordering
   [ ] - struct or tuple: transform each field in order. Concatenate results. (use tuples for composite keys)
*/

template <typename T>
inline T swap_endian(T u) {
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++) {
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    }

    return dest.u;
}

inline key_type make_prefix(eosio::name table_name, eosio::name index_name, uint8_t status = 1) {
   using namespace _key_value_detail;

   auto bige_table = swap_endian<uint64_t>(table_name.value);
   auto bige_index = swap_endian<uint64_t>(index_name.value);

   size_t size_64 = sizeof(index_name);

   size_t buffer_size = (2 * size_64) + sizeof(status);
   void* buffer = buffer_size > _key_value_detail::max_stack_buffer_size ? malloc(buffer_size) : alloca(buffer_size);

   memcpy(buffer, &status, sizeof(status));
   memcpy(((char*)buffer) + sizeof(status), &bige_table, size_64);
   memcpy(((char*)buffer) + sizeof(status) + size_64, &bige_index, size_64);

   std::string s((char*)buffer, buffer_size);

   if (buffer_size > _key_value_detail::max_stack_buffer_size) {
      free(buffer);
   }

   return {buffer_size, s};
}

inline key_type table_key(key_type prefix, key_type key) {
   using namespace _key_value_detail;

   size_t buffer_size = key.size + prefix.size;
   void* buffer = buffer_size > _key_value_detail::max_stack_buffer_size ? malloc(buffer_size) : alloca(buffer_size);

   memcpy(buffer, prefix.buffer.data(), prefix.size);
   memcpy(((char*)buffer) + prefix.size, key.buffer.data(), key.size);

   std::string s((char*)buffer, buffer_size);

   if (buffer_size > _key_value_detail::max_stack_buffer_size) {
      free(buffer);
   }

   return {buffer_size, s};
}

template <typename I>
inline I flip_msb(I val) {
   constexpr static size_t BITS = sizeof(I) * 8;
   return val ^ (static_cast<I>(1) << (BITS - 1));
}

template <typename I>
inline key_type make_key(I val) {
   using namespace _key_value_detail;

   if (std::is_signed<I>::value) {
      val = flip_msb(val);
   }

   auto big_endian = swap_endian<I>(val);

   size_t data_size = pack_size(big_endian);
   void* data_buffer = data_size > _key_value_detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

   datastream<char*> data_ds((char*)data_buffer, data_size);
   data_ds << big_endian;

   std::string s((char*)data_buffer, data_size);

   if (data_size > _key_value_detail::max_stack_buffer_size) {
      free(data_buffer);
   }
   return {data_size, s};
}

inline key_type make_key(std::string val, bool case_insensitive=false) {
   using namespace _key_value_detail;

   if (case_insensitive) {
      std::transform(val.begin(), val.end(), val.begin(), [](unsigned char c) -> unsigned char { return std::toupper(c); });
   }

   size_t data_size = pack_size(val) + 3;
   void* data_buffer = data_size > _key_value_detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

   datastream<char*> data_ds((char*)data_buffer, data_size);
   data_ds << val;

   ((char*)data_buffer)[data_size - 3] = 0x01;
   ((char*)data_buffer)[data_size - 2] = 0x00;
   ((char*)data_buffer)[data_size - 1] = 0x00;

   std::string s((char*)data_buffer, data_size);

   if (data_size > _key_value_detail::max_stack_buffer_size) {
      free(data_buffer);
   }
   return {data_size, s};
}

inline key_type make_key(const char * str, bool case_insensitive=false) {
   return make_key(std::string{str}, case_insensitive);
}

inline key_type make_key(eosio::name n) {
   return make_key(n.value);
}

template<typename T, uint64_t db = eosio::name{"eosio.kvram"}.value>
class kv_table {

   enum class kv_it_stat {
      iterator_ok     = 0,  // Iterator is positioned at a key-value pair
      iterator_erased = -1, // The key-value pair that the iterator used to be positioned at was erased
      iterator_end    = -2, // Iterator is out-of-bounds
   };

public:
   class index {
      class iterator {
      public:
         iterator(eosio::name contract_name, uint32_t itr, kv_it_stat itr_stat, size_t data_size, index* idx) :
                  contract_name{contract_name}, itr{itr}, itr_stat{itr_stat}, data_size{data_size}, idx{idx} {}

         T value() {
            using namespace _key_value_detail;

            eosio::check(itr_stat != kv_it_stat::iterator_end, "Cannot read end iterator");
            eosio::check(data_size > 0, "Cannot read a value of size 0");

            uint32_t actual_value_size;
            uint32_t offset = 0;

            void* buffer = data_size > _key_value_detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);
            auto stat = internal_use_do_not_use::kv_it_value(itr, offset, (char*)buffer, data_size, actual_value_size);

            eosio::check(static_cast<kv_it_stat>(stat) != kv_it_stat::iterator_end, "Error reading value");

            if (idx->name == idx->tbl->primary_index->name) {
               datastream<const char*> ds((char*)buffer, actual_value_size);

               T val;
               ds >> val;
               if (data_size > _key_value_detail::max_stack_buffer_size) {
                  free(buffer);
               }
               return val;
            } else {
               uint32_t actual_data_size;
               auto success = internal_use_do_not_use::kv_get(db, contract_name.value, (const char*)buffer, actual_value_size, actual_data_size);
               eosio::check(success, "failure getting primary key");

               void* pk_buffer = actual_data_size > _key_value_detail::max_stack_buffer_size ? malloc(actual_data_size) : alloca(actual_data_size);
               auto copy_size = internal_use_do_not_use::kv_get_data(db, 0, (char*)pk_buffer, actual_data_size);

               eosio::check(copy_size > 0, "failure getting primary index data");

               datastream<const char*> ds((char*)pk_buffer, copy_size);

               T val;
               ds >> val;

               if (actual_data_size > _key_value_detail::max_stack_buffer_size) {
                  free(pk_buffer);
               }
               if (data_size > _key_value_detail::max_stack_buffer_size) {
                  free(buffer);
               }
               return val;
            }
         }

         iterator& operator++() {
            eosio::check(itr_stat != kv_it_stat::iterator_end, "cannot increment end iterator");
            itr_stat = static_cast<kv_it_stat>(internal_use_do_not_use::kv_it_next(itr));
            return *this;
         }

         iterator operator++(int) {
            iterator copy(*this);
            ++(*this);
            return copy;
         }

         iterator& operator--() {
            itr_stat = static_cast<kv_it_stat>(internal_use_do_not_use::kv_it_prev(itr));
            eosio::check(itr_stat != kv_it_stat::iterator_end, "incremented past the beginning");
            return *this;
         }

         iterator operator--(int) {
            iterator copy(*this);
            --(*this);
            return copy;
         }

         bool operator==(iterator b) {
            if (itr_stat == kv_it_stat::iterator_end) {
               return b.itr_stat == kv_it_stat::iterator_end;
            }
            if (b.itr_stat == kv_it_stat::iterator_end) {
               return false;
            }
            return key() == b.key();
         }

         bool operator!=(iterator b) {
            if (itr_stat == kv_it_stat::iterator_end) {
               return b.itr_stat != kv_it_stat::iterator_end;
            }
            if (b.itr_stat == kv_it_stat::iterator_end) {
               return true;
            }
            return key() != b.key();
         }

      private:
         eosio::name contract_name;
         index* idx;

         size_t data_size;

         uint32_t itr;
         kv_it_stat itr_stat;

         key_type key() {
            return idx->get_key(value());
         }
      };

   public:
      eosio::name name;
      eosio::name table_name;
      eosio::name contract_name;

      kv_table* tbl;

      index() = default;

      index(eosio::name name, key_type (T::*key_function)() const): name{name}, key_function{key_function} {}

      template <typename K>
      iterator find(K key) {
         uint32_t value_size;

         auto prefix = make_prefix(table_name, name);
         auto t_key = table_key(prefix, make_key(key));

         auto success = internal_use_do_not_use::kv_get(db, contract_name.value, t_key.buffer.data(), t_key.size, value_size);
         if (!success) {
            return end();
         }

         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, prefix.buffer.data(), prefix.size);

         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, t_key.buffer.data(), t_key.size);
         auto cmp = internal_use_do_not_use::kv_it_key_compare(itr, t_key.buffer.data(), t_key.size);

         eosio::check(cmp == 0, "This key does not exist in this iterator");

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), value_size, this};
      }

      iterator end() {
         auto prefix = make_prefix(table_name, name);
         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, prefix.buffer.data(), prefix.size);
         int32_t itr_stat = internal_use_do_not_use::kv_it_move_to_end(itr);

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), 0, this};
      }

      iterator begin() {
         using namespace _key_value_detail;

         auto prefix = make_prefix(table_name, name);
         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, prefix.buffer.data(), prefix.size);
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, "", 0);

         uint32_t value_size;
         uint64_t buffer_size = 1024*1024; // TODO:
         void* buffer = buffer_size > _key_value_detail::max_stack_buffer_size ? malloc(buffer_size) : alloca(buffer_size);
         internal_use_do_not_use::kv_it_value(itr, 0, (char*)buffer, buffer_size, value_size);

         if (buffer_size > _key_value_detail::max_stack_buffer_size) {
            free(buffer);
         }

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat), value_size, this};
      }

      template <typename K>
      std::vector<T> range(K begin, K end) {
         eosio::check(begin <= end, "Beginning of range should be less than or equal to end");

         if (begin == end) {
            std::vector<T> t;
            t.push_back(find(begin).value());
            return t;
         }

         auto begin_itr = find(begin);
         auto end_itr = find(end);
         eosio::check(begin_itr != this->end(), "beginning of range is not in table");
         eosio::check(end_itr != this->end(), "end of range is not in table");

         std::vector<T> return_values;

         return_values.push_back(begin_itr.value());

         iterator itr = begin_itr;
         while(itr != end_itr){
            itr++;
            return_values.push_back(itr.value());
         }

         return return_values;
      }

      key_type get_key(T t) {
         return (t.*key_function)(); 
      }

   private:
      key_type (T::*key_function)() const;
   };

   template<typename I, typename ...Indices>
   void setup_indices(I index, Indices... indices) {
      index->contract_name = contract_name;
      index->table_name = table_name;
      index->tbl = this;

      secondary_indices.push_back(index);
      setup_indices(indices...);
   }

   template<typename I>
   void setup_indices(I index) {
      index->contract_name = contract_name;
      index->table_name = table_name;
      index->tbl = this;
      secondary_indices.push_back(index);
   }

   template <typename ...Indices>
   void init(eosio::name contract, eosio::name table, index* primary, Indices... indices) {
      contract_name = contract;
      table_name = table;

      primary_index = primary;
      primary_index->contract_name = contract;
      primary_index->table_name = table;
      primary_index->tbl = this;

      if constexpr (sizeof...(indices) > 0) {
         setup_indices(indices...);
      }
   }

   void upsert(T value) {
      using namespace _key_value_detail;

      auto t_key = table_key(make_prefix(table_name, primary_index->name), value.primary_key());

      size_t data_size = pack_size(value);
      void* data_buffer = data_size > _key_value_detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);
      datastream<char*> data_ds((char*)data_buffer, data_size);
      data_ds << value;

      internal_use_do_not_use::kv_set(db, contract_name.value, t_key.buffer.data(), t_key.size, (const char*)data_buffer, data_size);

      for (auto& idx : secondary_indices) {
         auto st_key = table_key(make_prefix(table_name, idx->name), idx->get_key(value));
         internal_use_do_not_use::kv_set(db, contract_name.value, st_key.buffer.data(), st_key.size, t_key.buffer.data(), t_key.size);
      }
      
      if (data_size > _key_value_detail::max_stack_buffer_size) {
         free(data_buffer);
      }
   }

   template <typename K>
   void erase(K key) {
      T val = primary_index->find(key).value();

      auto k = table_key(make_prefix(table_name, primary_index->name), make_key(key));
      internal_use_do_not_use::kv_erase(db, contract_name.value, (const char*)k.buffer.data(), k.size);

      for (auto& idx : secondary_indices) {
         auto skey = table_key(make_prefix(table_name, idx->name), idx->get_key(val));
         internal_use_do_not_use::kv_erase(db, contract_name.value, (const char*)skey.buffer.data(), skey.size);
      }
   }

private:
   eosio::name contract_name;
   eosio::name table_name;

   index* primary_index;
   std::vector<index*> secondary_indices;
};
} // eosio
