#pragma once
#include "producer_schedule.hpp"
#include "../../core/eosio/crypto.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/serialize.hpp"

namespace eosio {

   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         bool is_privileged( uint64_t account );

        __attribute__((eosio_wasm_import))
        void get_resource_limits( uint64_t account, int64_t* ram_bytes, int64_t* net_weight, int64_t* cpu_weight );

        __attribute__((eosio_wasm_import))
        void set_resource_limits( uint64_t account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight );

        __attribute__((eosio_wasm_import))
        void set_privileged( uint64_t account, bool is_priv );

        __attribute__((eosio_wasm_import))
        void set_blockchain_parameters_packed( char* data, uint32_t datalen );

        __attribute__((eosio_wasm_import))
        uint32_t get_blockchain_parameters_packed( char* data, uint32_t datalen );

        __attribute((eosio_wasm_import))
        int64_t set_proposed_producers( char*, uint32_t );
      }
   }

  /**
   *  @defgroup privileged Privileged
   *  @ingroup contracts
   *  @brief Defines C++ Privileged API
   */

   /**
    *  Tunable blockchain configuration that can be changed via consensus
    *  @ingroup privileged
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
    *  Set the blockchain parameters
    *
    *  @ingroup privileged
    *  @param params - New blockchain parameters to set
    */
   void set_blockchain_parameters(const eosio::blockchain_parameters& params);

   /**
    *  Retrieve the blolckchain parameters
    *
    *  @ingroup privileged
    *  @param params - It will be replaced with the retrieved blockchain params
    */
   void get_blockchain_parameters(eosio::blockchain_parameters& params);

    /**
    *  Get the resource limits of an account
    *
    *  @ingroup privileged
    *  @param account - name of the account whose resource limit to get
    *  @param ram_bytes -  output to hold retrieved ram limit in absolute bytes
    *  @param net_weight - output to hold net limit
    *  @param cpu_weight - output to hold cpu limit
    */
   inline void get_resource_limits( name account, int64_t& ram_bytes, int64_t& net_weight, int64_t& cpu_weight ) {
      internal_use_do_not_use::get_resource_limits( account.value, &ram_bytes, &net_weight, &cpu_weight );
   }

   /**
    *  Set the resource limits of an account
    *
    *  @ingroup privileged
    *  @param account - name of the account whose resource limit to be set
    *  @param ram_bytes - ram limit in absolute bytes
    *  @param net_weight - fractionally proportionate net limit of available resources based on (weight / total_weight_of_all_accounts)
    *  @param cpu_weight - fractionally proportionate cpu limit of available resources based on (weight / total_weight_of_all_accounts)
    */
   inline void set_resource_limits( name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight ) {
      internal_use_do_not_use::set_resource_limits( account.value, ram_bytes, net_weight, cpu_weight );
   }

   /**
    *  Proposes a schedule change
    *
    *  @ingroup privileged
    *  @note Once the block that contains the proposal becomes irreversible, the schedule is promoted to "pending" automatically. Once the block that promotes the schedule is irreversible, the schedule will become "active"
    *  @param producers - vector of producer keys
    *
    *  @return an optional value of the version of the new proposed schedule if successful
    */
   std::optional<uint64_t> set_proposed_producers( const std::vector<producer_key>& prods );

   /**
    *  Check if an account is privileged
    *
    *  @ingroup privileged
    *  @param account - name of the account to be checked
    *  @return true if the account is privileged
    *  @return false if the account is not privileged
    */
   inline bool is_privileged( name account ) {
      return internal_use_do_not_use::is_privileged( account.value );
   }

   /**
    *  Set the privileged status of an account
    *
    *  @ingroup privileged
    *  @param account - name of the account whose privileged account to be set
    *  @param is_priv - privileged status
    */
   inline void set_privileged( name account, bool is_priv ) {
      internal_use_do_not_use::set_privileged( account.value, is_priv );
   }

}
