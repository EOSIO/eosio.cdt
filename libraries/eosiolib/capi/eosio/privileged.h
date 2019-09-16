#pragma once
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup privileged_c Privileged C API
 * @ingroup c_api
 * @brief Defines %C Privileged API
 */

 /**
 * Get the resource limits of an account
 *
 * @param account - name of the account whose resource limit to get
 * @param ram_bytes - pointer to `int64_t` to hold retrieved ram limit in absolute bytes
 * @param net_weight - pointer to `int64_t` to hold net limit
 * @param cpu_weight - pointer to `int64_t` to hold cpu limit
 */
__attribute__((eosio_wasm_import))
void get_resource_limits( capi_name account, int64_t* ram_bytes, int64_t* net_weight, int64_t* cpu_weight );

/**
 * Set the resource limits of an account
 *
 * @param account - name of the account whose resource limit to be set
 * @param ram_bytes - ram limit in absolute bytes
 * @param net_weight - fractionally proportionate net limit of available resources based on (weight / total_weight_of_all_accounts)
 * @param cpu_weight - fractionally proportionate cpu limit of available resources based on (weight / total_weight_of_all_accounts)
 */
__attribute__((eosio_wasm_import))
void set_resource_limits( capi_name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight );

/**
 * Proposes a schedule change
 *
 * This is exactly equivalent to calling `set_proposed_producers_ex(0, producer_data, producer_data_size)`
 *
 * @note Once the block that contains the proposal becomes irreversible, the schedule is promoted to "pending" automatically. Once the block that promotes the schedule is irreversible, the schedule will become "active"
 * @param producer_data - packed data of produce_keys in the appropriate producer schedule order
 * @param producer_data_size - size of the data buffer
 *
 * @return -1 if proposing a new producer schedule was unsuccessful, otherwise returns the version of the new proposed schedule
 */
__attribute__((eosio_wasm_import))
int64_t set_proposed_producers( char *producer_data, uint32_t producer_data_size );

/**
 * Proposes a schedule change with extended features
 *
 * Valid formats:
 * 0 : serialized array of producer_key's. using this format is exactly equivalent to `set_proposed_producers(producer_data, producer_data_size)`
 * 1 : serialized array of producer_authority's
 *
 * @note Once the block that contains the proposal becomes irreversible, the schedule is promoted to "pending" automatically. Once the block that promotes the schedule is irreversible, the schedule will become "active"
 * @param producer_data_format - format of the producer data blob
 * @param producer_data - packed data of representing the producer schedule in the format indicated.
 * @param producer_data_size - size of the data buffer
 *
 * @return -1 if proposing a new producer schedule was unsuccessful, otherwise returns the version of the new proposed schedule
 */
__attribute__((eosio_wasm_import))
int64_t set_proposed_producers_ex( uint64_t producer_data_format, char *producer_data, uint32_t producer_data_size );


/**
 * Check if an account is privileged
 *
 * @param account - name of the account to be checked
 * @return true if the account is privileged
 * @return false if the account is not privileged
 */
__attribute__((eosio_wasm_import))
bool is_privileged( capi_name account );

/**
 * Set the privileged status of an account
 *
 * @param account - name of the account whose privileged account to be set
 * @param is_priv - privileged status
 */
__attribute__((eosio_wasm_import))
void set_privileged( capi_name account, bool is_priv );

/**
 * Set the blockchain parameters
 *
 * @param data - pointer to blockchain parameters packed as bytes
 * @param datalen - size of the packed blockchain parameters
 * @pre `data` is a valid pointer to a range of memory at least `datalen` bytes long that contains packed blockchain params data
 */
__attribute__((eosio_wasm_import))
void set_blockchain_parameters_packed( char* data, uint32_t datalen );

/**
 * Retrieve the blolckchain parameters
 *
 * @param data - output buffer of the blockchain parameters, only retrieved if sufficent size to hold packed data.
 * @param datalen - size of the data buffer, 0 to report required size.
 * @return size of the blockchain parameters
 * @pre `data` is a valid pointer to a range of memory at least `datalen` bytes long
 * @post `data` is filled with packed blockchain parameters
 */
__attribute__((eosio_wasm_import))
uint32_t get_blockchain_parameters_packed( char* data, uint32_t datalen );

/**
 * Pre-activate protocol feature
 *
 * @param feature_digest - digest of the protocol feature to pre-activate
 */
__attribute__((eosio_wasm_import))
void preactivate_feature( const capi_checksum256* feature_digest );

#ifdef __cplusplus
}
#endif
