#pragma once
#include <vector>
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/crypto.hpp"

namespace eosio {

  /**
   *  @defgroup producer_key
   *  @ingroup  contracts
   *  @{
   */

  /**
   * Maps producer with its signing key, used for producer schedule
   *
   * @brief Maps producer with its signing key
   */
  struct producer_key {

    /**
     * Name of the producer
     *
     * @brief Name of the producer
     */
    name             producer_name;

    /**
     * Block signing key used by this producer
     *
     * @brief Block signing key used by this producer
     */
    public_key       block_signing_key;

    friend constexpr bool operator < ( const producer_key& a, const producer_key& b ) {
      return a.producer_name < b.producer_name;
    }

    EOSLIB_SERIALIZE( producer_key, (producer_name)(block_signing_key) )
  };

   /**
   *  @defgroup producer_schedule
   *  @ingroup contracts 
   *  @brief Defines both the order, account name, and signing keys of the active set of producers.
   *
   *  @{
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
   /// @} producer_schedule

   /**
    *  @ingroup contracts 
    *  @brief Returns back the list of active producer names.
    *
    *  @{
    */
   std::vector<name> get_active_producers();

} /// namespace eosio
