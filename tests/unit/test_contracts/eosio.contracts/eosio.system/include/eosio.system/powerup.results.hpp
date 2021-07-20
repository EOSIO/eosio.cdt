#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/name.hpp>

using eosio::action_wrapper;
using eosio::asset;
using eosio::name;

/**
 * The action `powerresult` of `power.results` is a no-op.
 * It is added as an inline convenience action to `powerup` reservation.
 * This inline convenience action does not have any effect, however,
 * its data includes the result of the parent action and appears in its trace.
 */
class [[eosio::contract("powup.results")]] powup_results : eosio::contract {
   public:

      using eosio::contract::contract;

      /**
       * powupresult action.
       *
       * @param fee       - powerup fee amount
       * @param powup_net - amount of powup NET tokens
       * @param powup_cpu - amount of powup CPU tokens
       */
      [[eosio::action]]
      void powupresult( const asset& fee, const int64_t powup_net, const int64_t powup_cpu );

      using powupresult_action  = action_wrapper<"powupresult"_n,  &powup_results::powupresult>;
};
