/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <eosiolib/action.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/dispatcher.hpp>
#include <eosiolib/contract.hpp>

/**
 * Helper macros to reduce the verbosity for common contracts
 */
#define CONTRACT class [[eosio::contract]]
#define ACTION   [[eosio::action]] void
#define TABLE struct [[eosio::table]]

/**
 * @defgroup c_api C API
 * @brief C++ API for writing ESIO Smart Contracts
 */

 /**
  * @defgroup cpp_api C++ API
  * @brief C++ API for writing ESIO Smart Contracts
  */
