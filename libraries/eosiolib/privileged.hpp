#pragma once
#include "privileged.h"
#include "serialize.hpp"
#include "crypto.hpp"

namespace eosio {

  /**
   * @addtogroup privileged Privileged C++ API
   * @ingroup cpp_api
   * Defines C++ Privileged API
   * @{
   */

   /**
    * Tunable blockchain configuration that can be changed via consensus
    */
   struct blockchain_parameters {

      /**
      * The maxiumum net usage in instructions for a block
      * @brief the maxiumum net usage in instructions for a block
      */
      uint64_t max_block_net_usage;

      /**
      * The target percent (1% == 100, 100%= 10,000) of maximum net usage; exceeding this triggers congestion handling
      * @brief The target percent (1% == 100, 100%= 10,000) of maximum net usage; exceeding this triggers congestion handling
      */
      uint32_t target_block_net_usage_pct;

      /**
      * The maximum objectively measured net usage that the chain will allow regardless of account limits
      * @brief The maximum objectively measured net usage that the chain will allow regardless of account limits
      */
      uint32_t max_transaction_net_usage;

      /**
       * The base amount of net usage billed for a transaction to cover incidentals
       */
      uint32_t base_per_transaction_net_usage;

      /**
       * The amount of net usage leeway available whilst executing a transaction (still checks against new limits without leeway at the end of the transaction)
       * @brief The amount of net usage leeway available whilst executing a transaction  (still checks against new limits without leeway at the end of the transaction)
       */
      uint32_t net_usage_leeway;

      /**
      * The numerator for the discount on net usage of context-free data
      * @brief The numerator for the discount on net usage of context-free data
      */
      uint32_t context_free_discount_net_usage_num;

      /**
      * The denominator for the discount on net usage of context-free data
      * @brief The denominator for the discount on net usage of context-free data
      */
      uint32_t context_free_discount_net_usage_den;

      /**
      * The maxiumum billable cpu usage (in microseconds) for a block
      * @brief The maxiumum billable cpu usage (in microseconds) for a block
      */
      uint32_t max_block_cpu_usage;

      /**
      * The target percent (1% == 100, 100%= 10,000) of maximum cpu usage; exceeding this triggers congestion handling
      * @brief The target percent (1% == 100, 100%= 10,000) of maximum cpu usage; exceeding this triggers congestion handling
      */
      uint32_t target_block_cpu_usage_pct;

      /**
      * The maximum billable cpu usage (in microseconds) that the chain will allow regardless of account limits
      * @brief The maximum billable cpu usage (in microseconds) that the chain will allow regardless of account limits
      */
      uint32_t max_transaction_cpu_usage;

      /**
      * The minimum billable cpu usage (in microseconds) that the chain requires
      * @brief The minimum billable cpu usage (in microseconds) that the chain requires
      */
      uint32_t min_transaction_cpu_usage;

      /**
       * Maximum lifetime of a transacton
       * @brief Maximum lifetime of a transacton
       */
      uint32_t max_transaction_lifetime;

      /**
      * The number of seconds after the time a deferred transaction can first execute until it expires
      * @brief the number of seconds after the time a deferred transaction can first execute until it expires
      */
      uint32_t deferred_trx_expiration_window;


      /**
      * The maximum number of seconds that can be imposed as a delay requirement by authorization checks
      * @brief The maximum number of seconds that can be imposed as a delay requirement by authorization checks
      */
      uint32_t max_transaction_delay;

      /**
       * Maximum size of inline action
       * @brief Maximum size of inline action
       */
      uint32_t max_inline_action_size;

      /**
       * Maximum depth of inline action
       * @brief Maximum depth of inline action
       */
      uint16_t max_inline_action_depth;

      /**
       * Maximum authority depth
       * @brief Maximum authority depth
       */
      uint16_t max_authority_depth;


      EOSLIB_SERIALIZE( blockchain_parameters,
                        (max_block_net_usage)(target_block_net_usage_pct)
                        (max_transaction_net_usage)(base_per_transaction_net_usage)(net_usage_leeway)
                        (context_free_discount_net_usage_num)(context_free_discount_net_usage_den)

                        (max_block_cpu_usage)(target_block_cpu_usage_pct)
                        (max_transaction_cpu_usage)(min_transaction_cpu_usage)

                        (max_transaction_lifetime)(deferred_trx_expiration_window)(max_transaction_delay)
                        (max_inline_action_size)(max_inline_action_depth)(max_authority_depth)
      )
   };

   /**
    * @brief Set the blockchain parameters
    * Set the blockchain parameters
    * @param params - New blockchain parameters to set
    */
   void set_blockchain_parameters(const eosio::blockchain_parameters& params);

   /**
    * @brief Retrieve the blolckchain parameters
    * Retrieve the blolckchain parameters
    * @param params - It will be replaced with the retrieved blockchain params
    */
   void get_blockchain_parameters(eosio::blockchain_parameters& params);

   ///@}

   /**
   *  @defgroup types
   *  @{
   */

   /**
    * Maps producer with its signing key, used for producer schedule
    *
    * @brief Maps producer with its signing key
    */
   struct producer_key {

      /**
       * Name of the producer
       *
       * @brief Name of the producer
       */
      name             producer_name;

      /**
       * Block signing key used by this producer
       *
       * @brief Block signing key used by this producer
       */
      public_key       block_signing_key;

      friend constexpr bool operator < ( const producer_key& a, const producer_key& b ) {
         return a.producer_name < b.producer_name;
      }

      EOSLIB_SERIALIZE( producer_key, (producer_name)(block_signing_key) )
   };

   ///@}


}
