/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include "transaction.hpp"
#include "../../core/eosio/crypto.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/time.hpp"

#include <set>
#include <limits>

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         int32_t check_transaction_authorization(const char*,  uint32_t,
                                          const char*, uint32_t,
                                          const char*, uint32_t);
         __attribute__((eosio_wasm_import))
         int32_t check_permission_authorization( uint64_t, uint64_t,
                                                 const char*, uint32_t,
                                                 const char*, uint32_t,
                                                 uint64_t);
         __attribute__((eosio_wasm_import))
         int64_t get_permission_last_used(uint64_t, uint64_t);

         __attribute__((eosio_wasm_import))
         int64_t get_account_creation_time(uint64_t);
      }
   }

   /**
   *  Checks if a transaction is authorized by a provided set of keys and permissions
   *  @ingroup permission
   *
   *  @param trx_data - pointer to the start of the serialized transaction
   *  @param trx_size - size (in bytes) of the serialized transaction
   *  @param pubkeys_data - pointer to the start of the serialized vector of provided public keys
   *  @param pubkeys_size  - size (in bytes) of serialized vector of provided public keys (can be 0 if no public keys are to be provided)
   *  @param perms_data - pointer to the start of the serialized vector of provided permissions (empty permission name acts as wildcard)
   *  @param perms_size - size (in bytes) of the serialized vector of provided permissions
   *
   *  @return 1 if the transaction is authorized, 0 otherwise
   */
   bool
   check_transaction_authorization( const char* trx_data,     uint32_t trx_size,
                                    const char* pubkeys_data, uint32_t pubkeys_size,
                                    const char* perms_data,   uint32_t perms_size ) {
      return internal_use_do_not_use::check_transaction_authorization( trx_data, trx_size, pubkeys_data, pubkeys_size, perms_data, perms_size );
   }

   /**
   *  Checks if a permission is authorized by a provided delay and a provided set of keys and permissions
   *  @ingroup permission
   *
   *  @param account - the account owner of the permission
   *  @param permission - the name of the permission to check for authorization
   *  @param pubkeys_data - pointer to the start of the serialized vector of provided public keys
   *  @param pubkeys_size  - size (in bytes) of serialized vector of provided public keys (can be 0 if no public keys are to be provided)
   *  @param perms_data - pointer to the start of the serialized vector of provided permissions (empty permission name acts as wildcard)
   *  @param perms_size - size (in bytes) of the serialized vector of provided permissions
   *  @param delay - the provided delay in microseconds (cannot exceed INT64_MAX)
   *
   *  @return 1 if the permission is authorized, 0 otherwise
   */
   bool
   check_permission_authorization( name account,
                                   name permission,
                                   const char* pubkeys_data, uint32_t pubkeys_size,
                                   const char* perms_data,   uint32_t perms_size,
                                   microseconds delay ) {
      int64_t delay_us = delay.count();
      check(delay_us >= 0, "negative delay is not allowed");
      return internal_use_do_not_use::check_permission_authorization( account.value, permission.value, pubkeys_data, pubkeys_size, perms_data, perms_size,  static_cast<uint64_t>(delay_us) );
   }


  /**
   * @defgroup permission Permission
   * @ingroup contracts
   * @ingroup types
   * @brief Defines C++ API functions for validating authorization of keys and permissions
   */

   /**
    *  Checks if a transaction is authorized by a provided set of keys and permissions
    *  @ingroup permission
    *
    *  @param trx - the transaction for which to check authorizations
    *  @param provided_permissions - the set of permissions which have authorized the transaction (empty permission name acts as wildcard)
    *  @param provided_keys - the set of public keys which have authorized the transaction
    *
    *  @return whether the transaction was authorized by provided keys and permissions
    */
   bool
   check_transaction_authorization( const transaction&                 trx,
                                    const std::set<permission_level>&  provided_permissions ,
                                    const std::set<public_key>&        provided_keys = std::set<public_key>()
                                  )
   {
      auto packed_trx = pack(trx);

      std::vector<char> packed_keys;
      auto nkeys = provided_keys.size();
      if( nkeys > 0 ) {
         packed_keys = pack(provided_keys);
      }

      std::vector<char> packed_perms;
      auto nperms = provided_permissions.size();
      if( nperms > 0 ) {
         packed_perms = pack(provided_permissions);
      }

      auto res = internal_use_do_not_use::check_transaction_authorization( packed_trx.data(),
                                                    packed_trx.size(),
                                                    (nkeys > 0)  ? packed_keys.data()  : (const char*)0,
                                                    (nkeys > 0)  ? packed_keys.size()  : 0,
                                                    (nperms > 0) ? packed_perms.data() : (const char*)0,
                                                    (nperms > 0) ? packed_perms.size() : 0
                                                  );

      return (res > 0);
   }

   /**
    *  Checks if a permission is authorized by a provided delay and a provided set of keys and permissions
    *
    *  @ingroup permission
    *
    *  @param account    - the account owner of the permission
    *  @param permission - the permission name to check for authorization
    *  @param provided_keys - the set of public keys which have authorized the transaction
    *  @param provided_permissions - the set of permissions which have authorized the transaction (empty permission name acts as wildcard)
    *  @param provided_delay_us - the provided delay in microseconds (cannot exceed INT64_MAX)
    *
    *  @return whether the permission was authorized by provided delay, keys, and permissions
    */
   bool
   check_permission_authorization( name                               account,
                                   name                               permission,
                                   const std::set<public_key>&        provided_keys,
                                   const std::set<permission_level>&  provided_permissions = std::set<permission_level>(),
                                   microseconds                       provided_delay = microseconds{std::numeric_limits<int64_t>::max()}
                                 )
   {
      int64_t provided_delay_us = provided_delay.count();
      check(provided_delay_us >= 0, "negative delay is not allowed");
      std::vector<char> packed_keys;
      auto nkeys = provided_keys.size();
      if( nkeys > 0 ) {
         packed_keys = pack(provided_keys);
      }

      std::vector<char> packed_perms;
      auto nperms = provided_permissions.size();
      if( nperms > 0 ) {
         packed_perms = pack(provided_permissions);
      }

      auto res = internal_use_do_not_use::check_permission_authorization( account.value,
                                                   permission.value,
                                                   (nkeys > 0)  ? packed_keys.data()  : (const char*)0,
                                                   (nkeys > 0)  ? packed_keys.size()  : 0,
                                                   (nperms > 0) ? packed_perms.data() : (const char*)0,
                                                   (nperms > 0) ? packed_perms.size() : 0,
                                                   static_cast<uint64_t>(provided_delay_us)
                                                 );

      return (res > 0);
   }

   /**
    *  Returns the last used time of a permission
    *
    *  @ingroup permission
    *
    *  @param account    - the account owner of the permission
    *  @param permission - the name of the permission
    *
    *  @return the last used time (in microseconds since Unix epoch) of the permission
    */
   time_point get_permission_last_used( name account, name permission ) {
      return time_point(
               microseconds(
                  internal_use_do_not_use::get_permission_last_used(account.value, permission.value)));

   }

   /**
    *  Returns the creation time of an account
    *
    *  @ingroup permission
    *
    *  @param account - the account
    *
    *  @return the creation time (in microseconds since Unix epoch) of the account
    */
   time_point get_account_creation_time( name account ) {
      return time_point(
               microseconds(
                  internal_use_do_not_use::get_account_creation_time(account.value)));

   }
}
