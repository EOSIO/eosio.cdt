/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <alloca.h>
#include <string>

namespace eosio {

   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         void eosio_assert( uint32_t test, const char* msg );

         __attribute__((eosio_wasm_import))
         void eosio_assert_message( uint32_t test, const char* msg, uint32_t msg_len );

         __attribute__((eosio_wasm_import))
         void eosio_assert_code( uint32_t test, uint64_t code );
      }
   }

   /**
    *  @defgroup system System
    *  @ingroup core
    *  @brief Defines wrappers over eosio_assert
    */

   /**
    *  Assert if the predicate fails and use the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  eosio::check(a == b, "a does not equal b");
    *  @endcode
    */
   inline void check(bool pred, const char* msg) {
      if (!pred) {
         internal_use_do_not_use::eosio_assert(false, msg);
      }
   }

    /**
    *  Assert if the predicate fails and use the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  eosio::check(a == b, "a does not equal b");
    *  @endcode
    */
   inline void check(bool pred, const std::string& msg) {
      if (!pred) {
         internal_use_do_not_use::eosio_assert(false, msg.c_str());
      }
   }

   /**
    *  Assert if the predicate fails and use the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  eosio::check(a == b, "a does not equal b");
    *  @endcode
    */
   inline void check(bool pred, std::string&& msg) {
      if (!pred) {
         internal_use_do_not_use::eosio_assert(false, msg.c_str());
      }
   }

   /**
    *  Assert if the predicate fails and use a subset of the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  const char* msg = "a does not equal b b does not equal a";
    *  eosio::check(a == b, "a does not equal b", 18);
    *  @endcode
    */
   inline void check(bool pred, const char* msg, size_t n) {
      if (!pred) {
         internal_use_do_not_use::eosio_assert_message(false, msg, n);
      }
   }

   /**
    *  Assert if the predicate fails and use a subset of the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  std::string msg = "a does not equal b b does not equal a";
    *  eosio::check(a == b, msg, 18);
    *  @endcode
    */
   inline void check(bool pred, const std::string& msg, size_t n) {
      if (!pred) {
         internal_use_do_not_use::eosio_assert_message(false, msg.c_str(), n);
      }
   }

    /**
    *  Assert if the predicate fails and use the supplied error code.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  eosio::check(a == b, 13);
    *  @endcode
    */
   inline void check(bool pred, uint64_t code) {
      if (!pred) {
         internal_use_do_not_use::eosio_assert_code(false, code);
      }
   }
} // namespace eosio
