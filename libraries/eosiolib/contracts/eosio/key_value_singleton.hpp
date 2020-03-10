#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/print.hpp"
#include "../../core/eosio/utility.hpp"
#include "../../core/eosio/varint.hpp"

namespace detail {
   constexpr inline size_t max_stack_buffer_size = 512;
}

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         int64_t kv_erase(uint64_t db, uint64_t contract, const char* key, uint32_t key_size);

         __attribute__((eosio_wasm_import))
         int64_t kv_set(uint64_t db, uint64_t contract, const char* key, uint32_t key_size, const char* value, uint32_t value_size);

         __attribute__((eosio_wasm_import))
         bool kv_get(uint64_t db, uint64_t contract, const char* key, uint32_t key_size, uint32_t& value_size);

         __attribute__((eosio_wasm_import))
         uint32_t kv_get_data(uint64_t db, uint32_t offset, char* data, uint32_t data_size);
      }
   }

   template <typename T, eosio::name::raw SingletonName, eosio::name::raw DbName = "eosio.kvram"_n>
   class kv_singleton {
   public:
      kv_singleton(eosio::name contract_name) : contract_name{contract_name} {
         key = make_prefix(singleton_name);
      }

      ~kv_singleton() {
         if (is_dirty) {
            write_back();
         }
      }

      const T& get() {
         if (!is_cached) {
            uint32_t copy_size;
            uint32_t value_size;

            auto success = internal_use_do_not_use::kv_get(db_name, contract_name.value, key.data(), key.size(), value_size);

            eosio::check(success, "tried to get a singleton that does not exist");

            void* val_buffer = value_size > detail::max_stack_buffer_size ? malloc(value_size) : alloca(value_size);
            copy_size = internal_use_do_not_use::kv_get_data(db_name, 0, (char*)val_buffer, value_size);

            T val;
            deserialize(val, val_buffer, copy_size);

            value() = val;
            is_cached = true;

            raw_original.resize(copy_size);
            memcpy(raw_original.data(), val_buffer, copy_size);

            if (value_size > detail::max_stack_buffer_size) {
               free(val_buffer);
            }
         }

         return value();
      }

      void set(const T& val) {
         value() = val;
         is_dirty = true;
         is_cached = true;
      }

      void erase() {
         internal_use_do_not_use::kv_erase(db_name, contract_name.value, key.data(), key.size());
         is_cached = false;
         is_dirty = false;
      }

      void write_back() {
         size_t data_size = get_size(value());
         void* data_buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

         serialize(value(), data_buffer, data_size);

         if (raw_original.size() != data_size || memcmp(raw_original.data(), data_buffer, data_size) != 0) {
            internal_use_do_not_use::kv_set(db_name, contract_name.value, key.data(), key.size(), (const char*)data_buffer, data_size);
         }
      }

   private:
      constexpr static uint64_t db_name = static_cast<uint64_t>(DbName);
      constexpr static uint64_t singleton_name = static_cast<uint64_t>(SingletonName);

      eosio::name contract_name;
      key_type key;

      key_type raw_original;
      bool is_dirty = false;
      bool is_cached = false;

      key_type make_prefix(uint64_t singleton_name, uint8_t status = 2) {
         auto bige_singleton = swap_endian<uint64_t>(singleton_name);

         constexpr size_t buffer_size = sizeof(singleton_name) + sizeof(status);

         key_type ret;
         ret.resize(buffer_size);

         memcpy(ret.data(), &status, sizeof(status));
         memcpy(ret.data() + sizeof(status), &bige_singleton, sizeof(singleton_name));

         return ret;
      }

      template <typename V>
      static void deserialize(V& value, void* buffer, size_t size) {
         datastream<const char*> ds((char*)buffer, size);
         ds >> value;
      }

      template <typename V>
      static void serialize(const V& value, void* buffer, size_t size) {
         datastream<char*> ds((char*)buffer, size);
         ds << value;
      }

      template <typename V>
      static size_t get_size(const V& value) {
         return pack_size(value);
      }

      T& value() {
         static T value;
         return value;
      }
   };
}
