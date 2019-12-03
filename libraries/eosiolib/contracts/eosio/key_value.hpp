#pragma once
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/print.hpp"

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

using key_type = std::pair<size_t, std::string>;

template<typename T>
class kv_table {
   constexpr static size_t max_stack_buffer_size = 512;
   constexpr static uint64_t db = eosio::name{"eosio.kvram"}.value;

   enum class kv_it_stat {
      iterator_ok     = 0,  // Iterator is positioned at a key-value pair
      iterator_erased = -1, // The key-value pair that the iterator used to be positioned at was erased
      iterator_end    = -2, // Iterator is out-of-bounds
   };

   struct iterator {
      uint32_t itr;
      kv_it_stat itr_stat;

      size_t data_size;

      iterator(eosio::name contract_name, uint32_t itr, kv_it_stat itr_stat): contract_name{contract_name}, itr{itr}, itr_stat{itr_stat} {}
      iterator(eosio::name contract_name, uint32_t itr, kv_it_stat itr_stat, size_t data_size): contract_name{contract_name}, itr{itr}, itr_stat{itr_stat}, data_size{data_size} {}

      const T value() {
         #if 0
            uint32_t actual_size;
            uint32_t offset = 0; // TODO: How to use offset/is it needed?

            // TODO
            key_type _key = key();
            size_t key_size = pack_size(_key);
            void* key_buffer = max_stack_buffer_size < key_size ? malloc(key_size) : alloca(key_size);
            datastream<char*> key_ds((char*)key_buffer, key_size);
            key_ds << _key;
            // TODO

            uint32_t value_size;
            internal_use_do_not_use::kv_get(db, contract_name.value, (const char*)key_buffer, key_size, value_size);

            size_t data_size = size_t(value_size);
         #endif
         uint32_t actual_size;
         uint32_t offset = 0; // TODO: How to use offset/is it needed?

         void* buffer = max_stack_buffer_size < data_size ? malloc(data_size) : alloca(data_size);
         eosio::print_f("data_size %\n", data_size);
         internal_use_do_not_use::kv_it_value(itr, offset, (char*)buffer, data_size, actual_size);
         datastream<const char*> ds((char*)buffer, actual_size);

         T val;
         ds >> val;

         /*if (max_stack_buffer_size < key_size) {
            free(key_buffer);
         }*/

         if (max_stack_buffer_size < data_size) {
            free(buffer);
         }
         return val;
      }

      const key_type key() {
         #if 0
            uint32_t copy_size;

            size_t key_size = sizeof(K);
            void* buffer = max_stack_buffer_size < key_size ? malloc(key_size) : alloca(key_size);
            internal_use_do_not_use::kv_it_key(itr, 0, (char*)buffer, key_size, copy_size);

            datastream<const char*> ds((char*)buffer, copy_size);

            K key;
            ds >> key;

            if (max_stack_buffer_size < key_size) {
               free(buffer);
            }
         #endif

         auto key = std::make_pair(size_t(0), "");

         return key;
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
   };

public:
   struct index {
      eosio::name name;
      key_type (T::*key_function)() const;

      eosio::name contract_name;

      index() = default;

      index(eosio::name name, key_type (T::*key_function)() const): name{name}, key_function{key_function} {}

      template <typename K>
      iterator find(K key) {
         uint32_t value_size;

         #if 0
            // size_t key_size = pack_size(key);
            // void* key_buffer = max_stack_buffer_size < key_size ? malloc(key_size) : alloca(key_size);
            // datastream<char*> key_ds((char*)key_buffer, key_size);
            // key_ds << key;
         #endif
         eosio::print_f("\nfind: %\n", key);
         auto [key_size, _key] = make_key(key);
         eosio::print_f("key_size: %\n", key_size);
         eosio::print_f("key: ");
         eosio::printhex(_key.data(), key_size);
         eosio::print_f("\n");

         auto success = internal_use_do_not_use::kv_get(db, contract_name.value, (char*)_key.data(), key_size, value_size);
         if (!success) {
            eosio::print_f("    returning end??\n");
            return end();
         }

         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, "", 0);

         kv_it_stat itr_stat = static_cast<kv_it_stat>(internal_use_do_not_use::kv_it_lower_bound(itr, (const char*) _key.data(), key_size));
         auto cmp = internal_use_do_not_use::kv_it_key_compare(itr, (const char*)_key.data(), key_size);

         /*if (max_stack_buffer_size < key_size) {
            free(key_buffer);
         }*/

         eosio::check(cmp == 0, "This key does not exist in this iterator");

         eosio::print_f("value_size %\n", value_size);

         return {contract_name, itr, itr_stat, value_size};
      }

      /*
      void erase(K key) {
         size_t key_size = pack_size(key);
         void* key_buffer = max_stack_buffer_size < key_size ? malloc(key_size) : alloca(key_size);
         datastream<char*> key_ds((char*)key_buffer, key_size);
         key_ds << key;

         eosio::check(find(key) != end(), "cannot erase key that does not exist");

         internal_use_do_not_use::kv_erase(db, contract_name.value, (const char*)key_buffer, key_size);

         if (max_stack_buffer_size < key_size) {
            free(key_buffer);
         }
      }
      */

      iterator end() {
         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, "", 0);
         int32_t itr_stat = internal_use_do_not_use::kv_it_move_to_end(itr);

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat)};
      }

      iterator begin() {
         uint32_t itr = internal_use_do_not_use::kv_it_create(db, contract_name.value, "", 0);
         int32_t itr_stat = internal_use_do_not_use::kv_it_lower_bound(itr, "", 0);

         return {contract_name, itr, static_cast<kv_it_stat>(itr_stat)};
      }

      std::vector<T> range(key_type b, key_type e) {
         if (b == e) {
            std::vector<T> t;
            t.push_back(find(b).value());
            return t;
         }
         auto begin_itr = find(b);
         auto end_itr = find(e);
         eosio::check(begin_itr != end(), "beginning of range is not in table");
         eosio::check(end_itr != end(), "end of range is not in table");

         std::vector<T> return_values;

         return_values.push_back(begin_itr.value());

         iterator itr = begin_itr;
         while(itr != end_itr){
            itr++;
            return_values.push_back(itr.value());
         }

         return return_values;
      }

      key_type get_key(T& t) {
         return (t.*key_function)(); 
      }
   };

   kv_table() = default;

   template<typename I, typename ...Indices>
   void setup_indices(I index, Indices... indices) {
      secondary_indices.push_back(index);
      setup_indices(indices...);
   }

   template<typename I>
   void setup_indices(I index) {
      secondary_indices.push_back(index);
   }

   template <typename ...Indices>
   void init(eosio::name contract, eosio::name table, index* primary, Indices... indices) {
      contract_name = contract;
      table_name = table;

      primary_index = primary;
      primary_index->contract_name = contract;
      setup_indices(indices...);
   }

   void upsert(T value) {
      auto [key_size, key] = value.primary_key();

      eosio::print_f("upsert: %\n", value.n1);
      eosio::print_f("key_size: %\n", key_size);
      eosio::print_f("key: ");
      eosio::printhex(key.data(), key_size);
      eosio::print_f("\n");

      size_t data_size = pack_size(value);
      void* data_buffer = max_stack_buffer_size < data_size ? malloc(data_size) : alloca(data_size);
      datastream<char*> data_ds((char*)data_buffer, data_size);
      data_ds << value;

      internal_use_do_not_use::kv_set(db, contract_name.value, (const char*)key.data(), key_size, (const char*)data_buffer, data_size);

      for (auto& idx : secondary_indices) {
         auto [skey_size, skey] = idx->get_key(value);
         eosio::print_f("    skey_size: %\n", skey_size);
         eosio::print_f("    skey: ");
         eosio::printhex(skey.data(), skey_size);
         eosio::print_f("\n");

         internal_use_do_not_use::kv_set(db, contract_name.value, (const char*)skey.data(), skey_size, (const char*)key.data(), key_size);
      }
      
      if (max_stack_buffer_size < data_size) {
         free(data_buffer);
      }
   }

   template <typename K>
   void erase(K key) {
      size_t key_size = pack_size(key);
      void* key_buffer = max_stack_buffer_size < key_size ? malloc(key_size) : alloca(key_size);
      datastream<char*> key_ds((char*)key_buffer, key_size);
      key_ds << key;

      internal_use_do_not_use::kv_erase(db, contract_name.value, (const char*)key_buffer, key_size);

      /* TODO:
       * Loop through all of the indexes
       * delete with secondary index keys
       * (may need to get struct first to use key methods)
       */

      if (max_stack_buffer_size < key_size) {
         free(key_buffer);
      }
   }


private:
   eosio::name contract_name;
   eosio::name table_name;

   index* primary_index;
   std::vector<index*> secondary_indices;
};


/*
Key transformations
The key-value store could provide a lexicographical ordering of uint8_t on the keys. The contract can create an ordering on top by transforming its keys. Example transforms:

   uint?_t: Convert to big-endian
   int?_t: Invert the MSB then convert to big-endian
   strings: Convert 0x00 to (0x00, 0x01). Append (0x00, 0x00) to the end. This transform allows arbitrary-length strings.
   case-insensitive strings: Convert to upper-case, then apply the above transform. Assumes ASCII.
   floating-point:
      There's some bit manipulations, followed by an endian conversion
      limitations:
         Positive 0 and Negative 0 map to the same value
         NaN's and inf's end up with an unusual ordering
   struct or tuple: transform each field in order. Concatenate results.
*/
template <typename T>
T swap_endian(T u) {
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

template<typename T, typename ...Ts>
key_type make_key(T t, Ts... ts) {
   auto A = make_key(t);
   auto B = make_key(ts...);
   return {0, ""};
}
template<>
key_type make_key(std::string str) {
   constexpr static size_t max_stack_buffer_size = 512;
   size_t data_size = pack_size(str) + 3;
   void* data_buffer = max_stack_buffer_size < data_size ? malloc(data_size) : alloca(data_size);
   datastream<char*> data_ds((char*)data_buffer, data_size);
   data_ds << str;
   ((char*)data_buffer)[data_size - 3] = 0x01;
   ((char*)data_buffer)[data_size - 2] = 0x00;
   ((char*)data_buffer)[data_size - 1] = 0x00;
   std::string s{(char*)data_buffer};
   if (max_stack_buffer_size < data_size) {
      free(data_buffer);
   }
   return {data_size, s};
}
template<>
key_type make_key(eosio::name n) {
   constexpr static size_t max_stack_buffer_size = 512;
   auto big_endian = swap_endian<uint64_t>(n.value);

   size_t data_size = pack_size(big_endian);
   void* data_buffer = max_stack_buffer_size < data_size ? malloc(data_size) : alloca(data_size);
   datastream<char*> data_ds((char*)data_buffer, data_size);
   data_ds << big_endian;

   std::string s{(char*)data_buffer};
   if (max_stack_buffer_size < data_size) {
      free(data_buffer);
   }
   return {data_size, s};
}


} // eosio
