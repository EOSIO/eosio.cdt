// not needed, except to prevent an eventual redefinition error on LIKELY with eos-vm
#include <fc/exception/exception.hpp>

#include <eosio/history-tools/callbacks/basic.hpp>
#include <eosio/history-tools/callbacks/chaindb.hpp>
#include <eosio/history-tools/callbacks/compiler_builtins.hpp>
#include <eosio/history-tools/callbacks/console.hpp>
#include <eosio/history-tools/callbacks/memory.hpp>
#include <eosio/history-tools/callbacks/unimplemented.hpp>

namespace eosio { namespace filter_wasm {

struct callbacks;
using backend_t = eosio::vm::backend<callbacks, eosio::vm::jit>;
using rhf_t     = eosio::vm::registered_host_functions<callbacks>;

struct filter_state : history_tools::data_state<backend_t>, history_tools::console_state {
   eosio::vm::wasm_allocator wa = {};
};

// todo: remove basic_callbacks
struct callbacks : history_tools::basic_callbacks<callbacks>,
                   history_tools::chaindb_callbacks<callbacks>,
                   history_tools::compiler_builtins_callbacks<callbacks>,
                   history_tools::console_callbacks<callbacks>,
                   history_tools::data_callbacks<callbacks>,
                   history_tools::db_callbacks<callbacks>,
                   history_tools::memory_callbacks<callbacks>,
                   history_tools::unimplemented_callbacks<callbacks> {
   filter_wasm::filter_state&    filter_state;
   history_tools::chaindb_state& chaindb_state;
   history_tools::db_view_state& db_view_state;

   callbacks(filter_wasm::filter_state& filter_state, history_tools::chaindb_state& chaindb_state,
             history_tools::db_view_state& db_view_state)
       : filter_state{ filter_state }, chaindb_state{ chaindb_state }, db_view_state{ db_view_state } {}

   auto& get_state() { return filter_state; }
   auto& get_chaindb_state() { return chaindb_state; }
   auto& get_db_view_state() { return db_view_state; }
};

void register_callbacks() {
   history_tools::basic_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
   history_tools::chaindb_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
   history_tools::compiler_builtins_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
   history_tools::console_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
   history_tools::data_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
   history_tools::db_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
   history_tools::memory_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
   history_tools::unimplemented_callbacks<callbacks>::register_callbacks<rhf_t, eosio::vm::wasm_allocator>();
}

}} // namespace eosio::filter_wasm
