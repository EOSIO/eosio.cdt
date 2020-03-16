#pragma once

#include <eosio/wasm_interface.hpp>
#include <eosio/vm/backend.hpp>

namespace eosio { namespace cdt {
   class wasm_runner {
      using backend_t = vm::backend<wasm_interface>;
      using rhf_t = eosio::vm::registered_host_functions<wasm_interface>;
      public:
         wasm_runner(wake_context& ctx, vm::wasm_code code)
            : ctx(ctx), wi(ctx), bkend(code, rhf_t{}) {
            bkend.set_wasm_allocator(&alloc);
            bkend.initialize();
         }

         void execute() {
            bkend(&wi, "env", "apply", (uint64_t)0, (uint64_t)0, (uint64_t)0);
         }

      private:
         static vm::wasm_allocator   alloc;
         wake_context&               ctx;
         wasm_interface              wi;
         vm::backend<wasm_interface> bkend;
   };
}} // ns eosio::wake
