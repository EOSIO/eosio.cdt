#pragma once

#include <eosio/wake_context.hpp>
#include <eosio/vm/allocator.hpp>
#include <eosio/vm/host_function.hpp>

#include <string_view>

#define CDT_HOST_FUNC(X) \
   add_host_function<&wasm_interface::X>(#X)

namespace eosio { namespace cdt {
   class wasm_interface {
      public:
         using rhf_t = eosio::vm::registered_host_functions<wasm_interface>;
         wasm_interface(wake_context& ctx)
         : ctx(ctx) {
            CDT_HOST_FUNC(add_project);
            CDT_HOST_FUNC(eosio_assert);
            CDT_HOST_FUNC(memset);
            CDT_HOST_FUNC(memmove);
         }

         void add_project(void* name_data, uint32_t name_len) {
            ctx.add_project(std::string{(const char*)name_data, name_len});
         }

         void eosio_assert(bool pred, void* msg) {
            if (pred) {
               std::cerr << "Error, " << (const char*)msg << "\n";
               throw 0;
            }
         }

         void* memset(void* p1, uint32_t a, uint32_t len) {
            char* _p1 = (char*)p1;
            return ::memset(_p1, a, len);
         }

         void* memmove(void* p1, void* p2, uint32_t len) {
            char* _p1 = (char*)p1;
            const char* _p2 = (const char*)p2;
            return ::memmove(_p1, _p2, len);
         }

      private:
         void _assert(uint32_t, uint32_t) {}
         template <auto FN>
         inline void add_host_function(const std::string& name) { rhf_t::add<wasm_interface, FN, vm::wasm_allocator>("env", name); }
         wake_context ctx;
   };
}} // ns eosio::wake

#undef CDT_HOST_FUNC
