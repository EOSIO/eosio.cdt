#pragma once
#include <set>
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/serialize.hpp"

namespace eosio {

namespace internal_use_do_not_use {
extern "C" {
__attribute__((eosio_wasm_import)) int64_t add_security_group_participants(const char* data, uint32_t datalen);

__attribute__((eosio_wasm_import)) int64_t remove_security_group_participants(const char* data, uint32_t datalen);

__attribute__((eosio_wasm_import)) bool in_active_security_group(const char* data, uint32_t datalen);

__attribute__((eosio_wasm_import)) uint32_t get_active_security_group(char* data, uint32_t datalen);
}
} // namespace internal_use_do_not_use

/**
 *  @defgroup security_group Security Group
 *  @ingroup contracts
 *  @brief Defines C++ security group API
 */

struct security_group {
   uint32_t version;
   std::set<name> participants;
   CDT_REFLECT(version, participants);
};

/**
 * Propose new participants to the security group.
 *
 * @ingroup security_group
 * @param participants - the participants.
 *
 * @return -1 if proposing a new security group was unsuccessful, otherwise returns 0.
 */
inline int64_t add_security_group_participants(const std::set<name>& participants) {
   auto packed_participants = eosio::pack( participants );
   return internal_use_do_not_use::add_security_group_participants( packed_participants.data(), packed_participants.size() );
}

/**
 * Propose to remove participants from the security group.
 *å
 * @ingroup security_group
 * @param participants - the participants.
 *å
 * @return -1 if proposing a new security group was unsuccessful, otherwise returns 0.
 */
inline int64_t remove_security_group_participants(const std::set<name>& participants){
   auto packed_participants = eosio::pack( participants );
   return internal_use_do_not_use::remove_security_group_participants( packed_participants.data(), packed_participants.size() );
}

/**
 * Check if the specified accounts are all in the active security group.
 *
 * @ingroup security_group
 * @param participants - the participants.
 *
 * @return Returns true if the specified accounts are all in the active security group.
 */
inline bool in_active_security_group(const std::set<name>& participants){
   auto packed_participants = eosio::pack( participants );
   return internal_use_do_not_use::in_active_security_group( packed_participants.data(), packed_participants.size() );
}

/**
 * Gets the active security group
 *
 * @ingroup security_group
 * @param[out] packed_security_group - the buffer containing the packed security_group.
 *
 * @return Returns the size required in the buffer (if the buffer is too small, nothing is written).
 *
 */
inline security_group get_active_security_group() {
   size_t buffer_size = internal_use_do_not_use::get_active_security_group(0, 0);
   std::vector<char> buffer(buffer_size);
   internal_use_do_not_use::get_active_security_group(buffer.data(), buffer_size);
   return eosio::unpack<security_group>(buffer);
}
} // namespace eosio