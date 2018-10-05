/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup types Builtin Types
 *  @ingroup contractdev
 *  @brief Specifies builtin types, typedefs and aliases
 *
 *  @{
 */

/**
 * @brief Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned
 * @details Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned
 */

/* macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned */
#define ALIGNED(X) __attribute__ ((aligned (16))) X

typedef uint64_t capi_name;

/**
 * @brief EOSIO Public Key
 * @details EOSIO Public Key. It is 34 bytes.
 */
struct capi_public_key {
   char data[34];
};

/**
 * @brief EOSIO Signature
 * @details EOSIO Signature. It is 66 bytes.
 */
struct capi_signature {
   uint8_t data[66];
};

/**
 * @brief 256-bit hash
 * @details 256-bit hash
 */
struct ALIGNED(capi_checksum256) {
   uint8_t hash[32];
};

/**
 * @brief 160-bit hash
 * @details 160-bit hash
 */
struct ALIGNED(capi_checksum160) {
   uint8_t hash[20];
};

/**
 * @brief 512-bit hash
 * @details 512-bit hash
 */
struct ALIGNED(capi_checksum512) {
   uint8_t hash[64];
};

#ifdef __cplusplus
} /// extern "C"
#endif
/// @}
