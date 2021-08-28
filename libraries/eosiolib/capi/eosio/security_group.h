#pragma once
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Propose new participants to the security group.
 *
 * @param data - the buffer containing the packed participants.
 * @param datalen - size of the packed participants
 * @pre `data` is a valid pointer to a range of memory at least `datalen` bytes long that contains packed participants data
 * 
 * @return -1 if proposing a new security group was unsuccessful, otherwise returns 0.
*/
__attribute__((eosio_wasm_import))
int64_t add_security_group_participants(const char* data, uint32_t datalen);

/**
 * Propose to remove participants from the security group.
 *
 * @param data - the buffer containing the packed participants.
 * @param datalen - size of the packed participants
 * @pre `data` is a valid pointer to a range of memory at least `datalen` bytes long that contains packed participants data
 *  
 * @return -1 if proposing a new security group was unsuccessful, otherwise returns 0.
*/
__attribute__((eosio_wasm_import))
int64_t remove_security_group_participants(const char* data, uint32_t datalen);

/**
 * Check if the specified accounts are all in the active security group.
 *
 * @param data - the buffer containing the packed participants.
 * @param datalen - size of the packed participants
 *  
 * @return Returns true if the specified accounts are all in the active security group. 
*/
__attribute__((eosio_wasm_import))
bool in_active_security_group(const char* data, uint32_t datalen);

/**
 * Gets the active security group
 *
 * @param[out] data - the output buffer containing the packed security group.
 * @param datalen - size of the `data` buffer
 *  
 * @return Returns the size required in the buffer (if the buffer is too small, nothing is written).
 *
*/
__attribute__((eosio_wasm_import))
uint32_t get_active_security_group(char* data, uint32_t datalen);

#ifdef __cplusplus
}
#endif
