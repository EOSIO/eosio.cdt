/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once
#include "action.hpp"
#include "print.hpp"
#include "multi_index.hpp"
#include "dispatcher.hpp"
#include "contract.hpp"

#ifndef EOSIO_NATIVE
static_assert( sizeof(long) == sizeof(int), "unexpected size difference" );
#endif

/**
 * Helper macros to reduce the verbosity for common contracts
 */
#define CONTRACT class [[eosio::contract]]
#define ACTION   [[eosio::action]] void
#define TABLE struct [[eosio::table]]

#define _OVERLOAD(_1, _2, MAC, ...) MAC

#define _ACTION_1(NAME) [[eosio::action(#NAME)]] void
#define _ACTION_2(NAME, CONTRACT_NAME)   [[eosio::action(#NAME), eosio::contract(#CONTRACT_NAME)]] void
#define _TABLE_1(NAME) struct [[eosio::table(#NAME)]]
#define _TABLE_2(NAME, CONTRACT_NAME) struct [[eosio::table(#NAME), eosio::contract(#CONTRACT_NAME)]]

#define NAMED_CONTRACT(...) class [[eosio::contract(#__VA_ARGS__)]]
#define NAMED_ACTION(...) _OVERLOAD(__VA_ARGS__, _ACTION_2, _ACTION_1)(__VA_ARGS__)
#define NAMED_TABLE(...) _OVERLOAD(__VA_ARGS__, _TABLE_2, _TABLE_1)(__VA_ARGS__)

/**
 * @defgroup c_api C API
 * @brief C++ API for writing ESIO Smart Contracts
 */

 /**
  * @defgroup cpp_api C++ API
  * @brief C++ API for writing ESIO Smart Contracts
  */
