#pragma once
#include "privileged.hpp"

#include <vector>

namespace eosio {

   /**
   *  @defgroup producer_schedule
   *  @brief Defines both the order, account name, and signing keys of the active set of producers.
   *  @ingroup types
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

   /// @} producertype
} /// namespace eosio
