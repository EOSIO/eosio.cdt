#pragma once
#include <vector>
#include <eosio/name.hpp>
#include <eosio/crypto.hpp>
#include <eosio/serialize.hpp>

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

    CDT_REFLECT(producer_name, block_signing_key);
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

      CDT_REFLECT(version, producers);
   };

   /**
    *  @defgroup producer_authority Producer Authority
    *  @ingroup contracts
    *  @ingroup types
    *  @brief Maps producer with its a flexible authority structure, used for producer schedule
    */

   /**
    * pairs a public key with an integer weight
    *
    * @ingroup producer_authority
    */
   struct key_weight {
      /**
       * public key used in a weighted threshold multi-sig authority
       *
       * @brief public key used in a weighted threshold multi-sig authority
       */
      public_key key;

      /**
       * weight associated with a signature from the private key associated with the accompanying public key
       *
       * @brief weight of the public key
       */
      uint16_t   weight;

      CDT_REFLECT(key, weight);
   };

   /**
    * block signing authority version 0
    * this authority allows for a weighted threshold multi-sig per-producer
    *
    * @ingroup producer_authority
    *
    * @brief weighted threshold multi-sig authority
    */
   struct block_signing_authority_v0 {
      /**
       * minimum threshold of accumulated weights from component keys that satisfies this authority
       *
       * @brief minimum threshold of accumulated weights from component keys that satisfies this authority
       */
      uint32_t                    threshold;

      /**
       * component keys and their associated weights
       *
       * @brief component keys and their associated weights
       */
      std::vector<key_weight>     keys;

      bool is_valid()const;

      CDT_REFLECT(threshold, keys);
   };

   /**
    * variant of all possible block signing authorities
    *
    * @ingroup producer_authority
    */
   using block_signing_authority = std::variant<block_signing_authority_v0>;

   /**
    * Maps producer with its signing key, used for producer schedule
    *
    * @ingroup producer_authority
    *
    * @brief Maps producer with its signing key
    */
   struct producer_authority {

      /**
       * Name of the producer
       *
       * @brief Name of the producer
       */
      name             producer_name;

      /**
       * The block signing authority used by this producer
       */
      block_signing_authority       authority;

      friend constexpr bool operator < ( const producer_authority& a, const producer_authority& b ) {
         return a.producer_name < b.producer_name;
      }

      CDT_REFLECT(producer_name, authority);
   };

   /**
    *  Returns back the list of active producer names.
    *
    *  @ingroup producer_schedule
    */
   std::vector<name> get_active_producers();

} /// namespace eosio
