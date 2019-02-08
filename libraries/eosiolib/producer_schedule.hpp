#pragma once
#include "privileged.hpp"

#include <vector>

#warning "<eosiolib/producer_schedule.hpp> is deprecated use <eosio/producer_schedule.hpp>"
namespace eosio {

   /**
   *  @defgroup producer_schedule Producer Schedule
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
} /// namespace eosio
