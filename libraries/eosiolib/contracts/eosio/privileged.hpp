#pragma once
#include "producer_schedule.hpp"
#include "system.hpp"
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
         int64_t get_resource_limit( uint64_t account, uint64_t resource );

         __attribute__((eosio_wasm_import))
         void set_resource_limit( uint64_t account, uint64_t resource, int64_t limit );

         __attribute__((eosio_wasm_import))
         void set_privileged( uint64_t account, bool is_priv );

         __attribute__((eosio_wasm_import))
         void set_blockchain_parameters_packed( char* data, uint32_t datalen );

         __attribute__((eosio_wasm_import))
         uint32_t get_blockchain_parameters_packed( char* data, uint32_t datalen );

         __attribute__((eosio_wasm_import))
         void set_parameters_packed( char* data, uint32_t datalen );

         __attribute__((eosio_wasm_import))
         uint32_t get_parameters_packed( char* id, uint32_t idlen , char* data, uint32_t datalen  );

         __attribute__((eosio_wasm_import))
         void set_kv_parameters_packed( const char* data, uint32_t datalen );

         __attribute__((eosio_wasm_import))
         uint32_t get_kv_parameters_packed( void* data, uint32_t datalen, uint32_t max_version );

         __attribute__((eosio_wasm_import))
         uint32_t get_wasm_parameters_packed( char* data, uint32_t datalen, uint32_t max_version );

         __attribute__((eosio_wasm_import))
         void set_wasm_parameters_packed( const char* data, uint32_t datalen );

         __attribute((eosio_wasm_import))
         int64_t set_proposed_producers( char*, uint32_t );

         __attribute__((eosio_wasm_import))
         void preactivate_feature( const capi_checksum256* feature_digest );

         __attribute__((eosio_wasm_import))
         int64_t set_proposed_producers_ex( uint64_t producer_data_format, char *producer_data, uint32_t producer_data_size );
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
      * The maximum net usage in instructions for a block
      * @brief the maximum net usage in instructions for a block
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
      * The maximum billable cpu usage (in microseconds) for a block
      * @brief The maximum billable cpu usage (in microseconds) for a block
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

   enum {
      max_block_net_usage_id,
      target_block_net_usage_pct_id,
      max_transaction_net_usage_id,
      base_per_transaction_net_usage_id,
      net_usage_leeway_id,
      context_free_discount_net_usage_num_id,
      context_free_discount_net_usage_den_id,
      max_block_cpu_usage_id,
      target_block_cpu_usage_pct_id,
      max_transaction_cpu_usage_id,
      min_transaction_cpu_usage_id,
      max_transaction_lifetime_id,
      deferred_trx_expiration_window_id,
      max_transaction_delay_id,
      max_inline_action_size_id,
      max_inline_action_depth_id,
      max_authority_depth_id,
      max_action_return_value_size_id
   };

   using id_param_pairs_type = std::vector<std::pair<uint32_t, std::variant<uint16_t, uint32_t, uint64_t>>>;

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
    *  Tunable wasm limits configuration parameters.
    *  @ingroup privileged
    */

   struct wasm_parameters {
      /**
      * The maximum total size (in bytes) used for mutable globals.
      * i32 and f32 consume 4 bytes and i64 and f64 consume 8 bytes.
      * Const globals are not included in this count.
      * @brief The maximum total size (in bytes) used for mutable globals.
      */
      std::uint32_t max_mutable_global_bytes;

      /**
      * The maximum number of elements of a table.
      * @brief The maximum number of elements of a table.
      */
      std::uint32_t max_table_elements;

      /**
      * The maximum number of elements in each section.
      * @brief The maximum number of elements in each section.
      */
      std::uint32_t max_section_elements;

      /**
      * The size (in bytes) of the range of memory that may be initialized.
      * Data segments may use the range [0, max_linear_memory_init).
      * @brief The size (in bytes) of the range of memory that may be initialized.
      */
      std::uint32_t max_linear_memory_init;

      /**
      * The maximum total size (in bytes) used by parameters and local variables in a function.
      * @brief The maximum total size (in bytes) used by parameters and local variables in a function.
      */
      std::uint32_t max_func_local_bytes;

      /**
       * The maximum nesting depth of structured control instructions.
       * The function itself is included in this count.
       * @brief The maximum nesting depth of structured control instructions.
      */
      std::uint32_t max_nested_structures;

      /**
      * The maximum size (in bytes) of names used for import and export.
      * @brief The maximum size (in bytes) of names used for import and export.
      */
      std::uint32_t max_symbol_bytes;

      /**
      * The maximum total size (in bytes) of a wasm module.
      * @brief The maximum total size (in bytes) of a wasm module.
      */
      std::uint32_t max_module_bytes;

      /**
      * The maximum size (in bytes) of each function body.
      * @brief The maximum size (in bytes) of each function body.
      */
      std::uint32_t max_code_bytes;

      /**
       * The maximum number of 64 KiB pages of linear memory that a contract can use.
       * Enforced when an action is executed. The initial size of linear memory is also checked at setcode.
      * @brief The maximum number of 64 KiB pages of linear memory that a contract can use.
      */
      std::uint32_t max_pages;

      /**
      * The maximum number of functions that may be on the stack. Enforced when an action is executed.
      * @brief The maximum number of functions that may be on the stack. Enforced when an action is executed.
      */
      std::uint32_t max_call_depth;
   };

   /**
    * Set the configuration for wasm limits.
    *
    * @ingroup privileged
    *
    * @param params - the configuration to set.
   */
   inline void set_wasm_parameters(const eosio::wasm_parameters& params) {
      char buf[sizeof(uint32_t) + sizeof(eosio::wasm_parameters)];
      eosio::datastream<char *> ds( buf, sizeof(buf) );
      ds << uint32_t(0);  // fill in version
      ds << params;
      internal_use_do_not_use::set_wasm_parameters_packed( buf, ds.tellp() );
   }

   /**
    * Get the current wasm limits configuration.
    *
    * @ingroup privileged
    *
    * @param[out] params the ouput for the parameters.
    *
    * @return the size of the parameters read onto the local buffer.
   */
   inline int get_wasm_parameters(eosio::wasm_parameters& params) {
      char buf[sizeof(uint32_t) + sizeof(eosio::wasm_parameters)];
      int sz = internal_use_do_not_use::get_wasm_parameters_packed(buf, sizeof(buf), 0);
      eosio::datastream<char *> ds( buf, sizeof(buf) );
      uint32_t version;
      ds >> version;
      ds >> params;
      return sz;
   }

   /**
    *  Set the blockchain parameters flexibly by id data pair vector
    *
    *  @ingroup privileged
    *  @param params - New blockchain parameters to set, only id and data in the params will be set.
    */
   void set_parameters(const id_param_pairs_type & params);

   /**
    *  Retrieve the blolckchain parameters flexibly by ids
    *
    *  @ingroup privileged
    *  @param param_ids - The id vecter in which ids are being queried from chain, see upper enum
    *  @param params - It is output data with the retrieved blockchain params, before call it should be empty
    */
   void get_parameters(const std::vector<uint32_t> & param_ids,  id_param_pairs_type & params);

   /**
    *  Tunable KV configuration that can be changed via consensus
    *  @ingroup privileged
    */
   struct kv_parameters {
      /**
      * The maximum key size
      * @brief The maximum key size
      */
      uint32_t max_key_size;

      /**
      * The maximum value size
      * @brief The maximum value size
      */
      uint32_t max_value_size;

      /**
       * The maximum number of iterators
      * @brief The maximum number of iterators
       */
      uint32_t max_iterators;

      EOSLIB_SERIALIZE( kv_parameters,
                        (max_key_size)
                        (max_value_size)(max_iterators)
      )
   };

   /**
    *  Get the kv parameters
    *
    *  @ingroup privileged
    *  @param[out] params - kv parameters to get
    *
    *  @return the size of the parameters read onto the local buffer.
    */
   inline int get_kv_parameters(eosio::kv_parameters& params) {
      char buf[sizeof(uint32_t) + sizeof(eosio::kv_parameters)];
      int sz = internal_use_do_not_use::get_kv_parameters_packed(buf, sizeof(buf), 0);
      eosio::datastream<char *> ds( buf, sizeof(buf) );
      uint32_t version;
      ds >> version;
      ds >> params;
      return sz;
   }

   /**
    *  Set the kv parameters
    *
    *  @ingroup privileged
    *  @param params - New kv parameters to set
    */
   inline void set_kv_parameters(const eosio::kv_parameters& params) {
      // set_kv_parameters_packed expects version, max_key_size,
      // max_value_size, and max_iterators,
      // while kv_parameters only contains max_key_size, max_value_size,
      // and max_iterators. That's why we place uint32_t in front
      // of kv_parameters in buf
      char buf[sizeof(uint32_t) + sizeof(eosio::kv_parameters)];
      eosio::datastream<char *> ds( buf, sizeof(buf) );
      ds << uint32_t(0);  // fill in version
      ds << params;
      internal_use_do_not_use::set_kv_parameters_packed( buf, ds.tellp() );
   }

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
   * Get a single resource limit associated with an account.
   *
   * @ingroup privileged
   *
   * @param account - the account whose limits are being modified
   * @param resource - the name of the resource limit which should be either ram, cpu, or net.
   *
   * @return the limit on the resource requested.
   */
   inline int64_t get_resource_limit( name account, name resource ) {
      return internal_use_do_not_use::get_resource_limit( account.value, resource.value );
   }

   /**
   * Update a single resource limit associated with an account.
   *
   * @ingroup privileged
   *
   * @param account - the account whose limits are being modified.
   * @param resource - the resource to update, which should be either ram, cpu, or net.
   * @param limit - the new limit.  A value of -1 means unlimited.
   *
   * @pre limit >= -1
   */
   inline void set_resource_limit( name account, name resource, int64_t limit ) {
      internal_use_do_not_use::set_resource_limit( account.value, resource.value, limit );
   }

   /**
    *  Proposes a schedule change using the legacy producer key format
    *
    *  @ingroup privileged
    *  @note Once the block that contains the proposal becomes irreversible, the schedule is promoted to "pending" automatically. Once the block that promotes the schedule is irreversible, the schedule will become "active"
    *  @param producers - vector of producer keys
    *
    *  @return an optional value of the version of the new proposed schedule if successful
    */
   std::optional<uint64_t> set_proposed_producers( const std::vector<producer_key>& prods );

   /**
    *  Proposes a schedule change using the more flexible key format
    *
    *  @ingroup privileged
    *  @note Once the block that contains the proposal becomes irreversible, the schedule is promoted to "pending" automatically. Once the block that promotes the schedule is irreversible, the schedule will become "active"
    *  @param producers - vector of producer authorities
    *
    *  @return an optional value of the version of the new proposed schedule if successful
    */
   inline std::optional<uint64_t> set_proposed_producers( const std::vector<producer_authority>& prods ) {
      auto packed_prods = eosio::pack( prods );
      int64_t ret = internal_use_do_not_use::set_proposed_producers_ex(1, (char*)packed_prods.data(), packed_prods.size());
      if (ret >= 0)
        return static_cast<uint64_t>(ret);
      return {};
   }

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

   /**
    * Pre-activate protocol feature
    *
    * @ingroup privileged
    * @param feature_digest - digest of the protocol feature to pre-activate
    */
   inline void preactivate_feature( const checksum256& feature_digest ) {
      auto feature_digest_data = feature_digest.extract_as_byte_array();
      internal_use_do_not_use::preactivate_feature(
         reinterpret_cast<const internal_use_do_not_use::capi_checksum256*>( feature_digest_data.data() )
      );
   }

}
