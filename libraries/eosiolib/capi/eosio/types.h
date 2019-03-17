/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

/**
 *  @defgroup c_types
 *  @ingroup c_api
 *  @brief Specifies builtin types, typedefs and aliases
 */

/**
 *  @addtogroup c_types
 *  @brief Specifies builtin types, typedefs and aliases
 *  @{
 */

/**
 * Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned
 */

/* macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned */
#define ALIGNED(X) __attribute__ ((aligned (16))) X

typedef uint64_t capi_name;

/**
 * EOSIO Public Key. It is 34 bytes.
 */
struct capi_public_key {
   char data[34];
};
typedef struct capi_public_key capi_public_key;

/**
 * EOSIO Signature. It is 66 bytes.
 */
struct capi_signature {
   uint8_t data[66];
};
typedef struct capi_signature capi_signature;

/**
 * 256-bit hash
 */
struct ALIGNED(capi_checksum256) {
   uint8_t hash[32];
};
typedef struct capi_checksum256 capi_checksum256;

/**
 * 160-bit hash
 */
struct ALIGNED(capi_checksum160) {
   uint8_t hash[20];
};
typedef struct capi_checksum160 capi_checksum160;

/**
 * 512-bit hash
 */
struct ALIGNED(capi_checksum512) {
   uint8_t hash[64];
};
typedef struct capi_checksum512 capi_checksum512;

/// @}
