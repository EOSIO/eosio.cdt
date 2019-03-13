#pragma once
#include <vector>
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/crypto.hpp"

namespace eosio {

  /**
   *  @defgroup producer_key Producer Key
   *  @ingroup contracts
   *  @ingroup types
   *  @brief Maps producer with its signing key, used for producer schedule
   */

  /**
   *  Maps producer with its signing key, used for producer schedule
   *
   *  @ingroup producer_key
   */
  struct producer_key {

    /**
     *  Name of the producer
     *
     *  @ingroup producer_key
     */
    name             producer_name;

    /**
     *  Block signing key used by this producer
     *
     *  @ingroup producer_key
     */
    public_key       block_signing_key;

    /// @cond OPERATORS

    friend constexpr bool operator < ( const producer_key& a, const producer_key& b ) {
      return a.producer_name < b.producer_name;
    }

    /// @endcond

    EOSLIB_SERIALIZE( producer_key, (producer_name)(block_signing_key) )
  };

   /**
    *  @defgroup producer_schedule Producer Schedule
    *  @ingroup contracts
    *  @ingroup types
    *  @brief Defines both the order, account name, and signing keys of the active set of producers.
    */

   /**
    * Defines both the order, account name, and signing keys of the active set of producers.
    *
    * @ingroup producer_schedule
    */
   struct producer_schedule {
      /**
       * Version number of the schedule. It is sequentially incrementing version number
       */
      uint32_t                     version;

      /**
       * List of producers for this schedule, including its signing key
       */
      std::vector<producer_key>    producers;
   };

   /**
    *  Returns back the list of active producer names.
    *
    *  @ingroup producer_schedule
    */
   std::vector<name> get_active_producers();

} /// namespace eosio
