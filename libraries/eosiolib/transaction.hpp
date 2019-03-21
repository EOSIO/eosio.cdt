/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once
#include "transaction.h"
#include "action.hpp"
#include "time.hpp"
#include "serialize.hpp"
#include "system.hpp"

#include <vector>

#warning "<eosiolib/transaction.hpp> is deprecated use <eosio/transaction.hpp>"
namespace eosio {

  /**
   * @defgroup transaction Transaction C++ API
   * @ingroup contracts
   * @brief Type-safe C++ wrappers for transaction C API
   *
   * @details An inline message allows one contract to send another contract a message
   * which is processed immediately after the current message's processing
   * ends such that the success or failure of the parent transaction is
   * dependent on the success of the message. If an inline message fails in
   * processing then the whole tree of transactions and actions rooted in the
   * block will me marked as failing and none of effects on the database will
   * persist.
   *
   * Inline actions and Deferred transactions must adhere to the permissions
   * available to the parent transaction or, in the future, delegated to the
   * contract account for future use.

   * @note There are some methods from the @ref transactioncapi that can be used directly from C++
   * @{
   */
   typedef std::tuple<uint16_t, std::vector<char>> extension;
   typedef std::vector<extension> extensions_type;

   /**
    * Class transaction_header contains details about the transaction
    * @brief Contains details about the transaction
    */

   class transaction_header {
   public:

      /**
       * Construct a new transaction_header with an expiration of now + 60 seconds.
       *
       * @brief Construct a new transaction_header object initialising the transaction header expiration to now + 60 seconds
       */
      transaction_header( time_point_sec exp = time_point_sec(now() + 60) )
         :expiration(exp)
      {}

      time_point_sec  expiration;
      uint16_t        ref_block_num;
      uint32_t        ref_block_prefix;
      unsigned_int    max_net_usage_words = 0UL; /// number of 8 byte words this transaction can serialize into after compressions
      uint8_t         max_cpu_usage_ms = 0UL; /// number of CPU usage units to bill transaction for
      unsigned_int    delay_sec = 0UL; /// number of seconds to delay transaction, default: 0

      EOSLIB_SERIALIZE( transaction_header, (expiration)(ref_block_num)(ref_block_prefix)(max_net_usage_words)(max_cpu_usage_ms)(delay_sec) )
   };

   /**
    * Class transaction contains the actions, context_free_actions and extensions type for a transaction
    * @brief Contains the actions, context_free_actions and extensions type for a transaction
    */
   class transaction : public transaction_header {
   public:

      /**
       * Construct a new transaction with an expiration of now + 60 seconds.
       *
       * @brief Construct a new transaction object initialising the transaction header expiration to now + 60 seconds
       */
      transaction(time_point_sec exp = time_point_sec(now() + 60)) : transaction_header( exp ) {}

      /**
       *  Sends this transaction, packs the transaction then sends it as a deferred transaction
       *
       *  @brief Writes the symbol_code as a string to the provided char buffer
       *  @param sender_id - ID of sender
       *  @param payer - Account paying for RAM
       *  @param replace_existing - Defaults to false, if this is `0`/false then if the provided sender_id is already in use by an in-flight transaction from this contract, which will be a failing assert. If `1` then transaction will atomically cancel/replace the inflight transaction
       */
      void send(const uint128_t& sender_id, name payer, bool replace_existing = false) const {
         auto serialize = pack(*this);
         send_deferred(sender_id, payer.value, serialize.data(), serialize.size(), replace_existing);
      }

      std::vector<action>  context_free_actions;
      std::vector<action>  actions;
      extensions_type      transaction_extensions;

      EOSLIB_SERIALIZE_DERIVED( transaction, transaction_header, (context_free_actions)(actions)(transaction_extensions) )
   };

   /**
    * Struct onerror contains and sender id and packed transaction
    * @brief  Contains and sender id and packed transaction
    */
   struct onerror {
      uint128_t          sender_id;
      std::vector<char> sent_trx;

     /**
      * from_current_action unpacks and returns a onerror struct
      * @brief Unpacks and returns a onerror struct
      */
      static onerror from_current_action() {
         return unpack_action_data<onerror>();
      }

     /**
      * unpack_sent_trx unpacks and returns a transaction
      * @brief Unpacks and returns a transaction
      */
      transaction unpack_sent_trx() const {
         return unpack<transaction>(sent_trx);
      }

      EOSLIB_SERIALIZE( onerror, (sender_id)(sent_trx) )
   };

   /**
    * Retrieve the indicated action from the active transaction.
    *
    * @param type - 0 for context free action, 1 for action
    * @param index - the index of the requested action
    * @return the indicated action
    */
   inline action get_action( uint32_t type, uint32_t index ) {
      constexpr size_t max_stack_buffer_size = 512;
      int s = ::get_action( type, index, nullptr, 0 );
      eosio::check( s > 0, "get_action size failed" );
      size_t size = static_cast<size_t>(s);
      char* buffer = (char*)( max_stack_buffer_size < size ? malloc(size) : alloca(size) );
      auto size2 = ::get_action( type, index, buffer, size );
      eosio::check( size == static_cast<size_t>(size2), "get_action failed" );
      return eosio::unpack<eosio::action>( buffer, size );
   }

   ///}@
}
