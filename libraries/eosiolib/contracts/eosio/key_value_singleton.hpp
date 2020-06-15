#pragma once
#include "../../core/eosio/datastream.hpp"
#include "../../core/eosio/name.hpp"
#include <eosio/to_key.hpp>

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
      struct state {
         T value;
         char* raw_original;
         size_t raw_original_size;

         bool is_dirty = false;
         bool is_cached = false;

         ~state() {
            if (raw_original_size) {
               free(raw_original);
            }
         }
      };

   public:
      explicit kv_singleton(eosio::name contract_name) : contract_name{contract_name} {
         key = make_prefix();
      }

      ~kv_singleton() {
         if (get_state().is_dirty) {
            store();
         }
      }

      const T& get_or_create() {
          if( !exists() ) {
              auto& ste = get_state();
              ste.is_cached = true;
              ste.is_dirty = true;
          }
          return get();
      };

      T& create_or_modify() {
          if( !exists() ) {
              auto& ste = get_state();
              ste.is_cached = true;
              ste.is_dirty = true;
          }
          return modify();
      };

      const T& get() {
         auto& ste = get_state();
         load_state(ste);

         return ste.value;
      }

      T& modify() {
         auto& ste = get_state();
         load_state(ste);
         ste.is_dirty = true;

         return ste.value;
      }


      void set(const T& val) {
         auto& ste = get_state();
         ste.value = val;
         ste.is_dirty = true;
         ste.is_cached = true;
      }

      bool exists() const {
         uint32_t value_size;

         return internal_use_do_not_use::kv_get(db_name, contract_name.value, key.data(), key.size(), value_size);
      }

      void erase() {
         internal_use_do_not_use::kv_erase(db_name, contract_name.value, key.data(), key.size());
         auto& ste = get_state();
         ste.is_cached = false;
         ste.is_dirty = false;
         ste.raw_original_size = 0;
         free(ste.raw_original);
      }

      void store() {
         auto& ste = get_state();
         if (ste.is_dirty) {
            size_t data_size = get_size(ste.value);
            void* data_buffer = data_size > detail::max_stack_buffer_size ? malloc(data_size) : alloca(data_size);

            serialize(ste.value, data_buffer, data_size);

            if (ste.raw_original_size != data_size || memcmp(ste.raw_original, data_buffer, data_size) != 0) {
               internal_use_do_not_use::kv_set(db_name, contract_name.value, key.data(), key.size(), (const char*)data_buffer, data_size);
            }
         }
      }

   private:
      constexpr static uint64_t db_name = static_cast<uint64_t>(DbName);
      constexpr static uint64_t singleton_name = static_cast<uint64_t>(SingletonName);

      eosio::name contract_name;
      key_type key;

      key_type make_prefix() {
         return make_key(std::make_tuple(0x02, singleton_name));
      }

      template <typename V>
      static void serialize(const V& value, void* buffer, size_t size) {
         datastream<char*> ds((char*)buffer, size);
         unsigned_int i{0};
         ds << i;
         ds << value;
      }

      template <typename... Vs>
      static void serialize(const std::variant<Vs...>& value, void* buffer, size_t size) {
         datastream<char*> ds((char*)buffer, size);
         ds << value;
      }

      template <typename V>
      static void deserialize(V& value, void* buffer, size_t size) {
         unsigned_int idx;
         datastream<const char*> ds((char*)buffer, size);

         ds >> idx;
         eosio::check(idx==unsigned_int(0), "there was an error deserializing this value.");
         ds >> value;
      }

      template <typename... Vs>
      static void deserialize(std::variant<Vs...>& value, void* buffer, size_t size) {
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

      state& get_state() const {
         static state value;
         return value;
      }

      const state& get_state() const {
         static state value;
         return value;
      }

      void load_state(state& ste) {
         if (!ste.is_cached) {
            uint32_t copy_size;
            uint32_t value_size;

            auto success = internal_use_do_not_use::kv_get(db_name, contract_name.value, key.data(), key.size(), value_size);

            if( !success ) {
                eosio::check(success, "the singleton " + eosio::name(SingletonName).to_string() + " does not exist");
            }

            ste.raw_original = (char*)malloc(value_size);
            ste.raw_original_size = value_size;
            copy_size = internal_use_do_not_use::kv_get_data(db_name, 0, ste.raw_original, value_size);

            deserialize(ste.value, ste.raw_original, copy_size);
            ste.is_cached = true;
         }
      }
   };
}
