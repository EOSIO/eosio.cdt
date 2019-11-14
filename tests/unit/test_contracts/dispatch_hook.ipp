#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <eosio/action.hpp>
#include <tuple>

using namespace eosio;

enum handlers {
   notify_any_handler = 1,
   notify_handler = 2,
   post_dispatch_handler = 4,
   pre_dispatch_handler = 8
};

class [[eosio::contract("dispatch_hook")]] dispatch_hook : eosio::contract {
public:
   using eosio::contract::contract;
#if !defined(NOTIFY_ANY_NO_MATCH) && !defined(NOTIFY_ANY_MATCH) && !defined(NOTIFY_NO_MATCH) && !defined(NOTIFY_MATCH)
   [[eosio::action]] void dummy() {}
#endif
#ifdef NOTIFY_ANY_NO_MATCH
   [[eosio::on_notify("*::missing")]] void on_notify1(name, int flag) { eosio::check(false, "should not execute notify for non-existent action"); }
#endif
#ifdef NOTIFY_ANY_MATCH
   [[eosio::on_notify("*::forward")]] void on_notify2(name, int flag) { eosio::check((flag & notify_any_handler) != 0, "in notify any handler"); }
#endif
#ifdef NOTIFY_NO_MATCH
   [[eosio::on_notify("test::missing")]] void on_notify3(name, int flag) { eosio::check(false, "should not execute notify for non-existent action"); }
#endif
#ifdef NOTIFY_MATCH
   [[eosio::on_notify("test::forward")]] void on_notify4(name, int flag) { eosio::check((flag & notify_handler) != 0, "in notify handler"); }
#endif
};

extern "C" {
#ifdef PRE_DISPATCH
   bool pre_dispatch(name self, name original_receiver, name action) {
      auto [_, res] = unpack_action_data<std::tuple<name, int>>();
      return (res & pre_dispatch_handler) == 0;
   }
#endif
#ifdef POST_DISPATCH
   void post_dispatch(name self, name original_receiver, name action) {
      auto [_, res] = unpack_action_data<std::tuple<name, int>>();
      check((res & post_dispatch_handler) != 0, "in post_dispatch");
   }
#endif
}
