#pragma once

#include <eosiolib/name.hpp>
#include <eosiolib/datastream.hpp>

namespace eosio {

/**
 * @defgroup contracttype Contract Type
 * @ingroup types
 * @brief Defines contract type which is %base class for every EOSIO contract
 *
 * @{
 *
 */

/**
 * @brief %Base class for EOSIO contract.
 * @details %Base class for EOSIO contract. %A new contract should derive from this class, so it can make use of EOSIO_ABI macro.
 */
class contract {
   public:
      /**
       * Construct a new contract given the contract name
       *
       * @brief Construct a new contract object.
       * @param n - The name of this contract
       */
      contract( name n ):_self(n) {}

      /**
       * Construct a new contract given the contract name
       *
       * @brief Construct a new contract object.
       * @param n - The name of this contract
       * @param ds - The datastream used 
       */
      contract( name n, datastream<const char*> ds ):_self(n), _ds(ds) {}

      /**
       *
       * Get this contract name
       *
       * @brief Get this contract name.
       * @return name - The name of this contract
       */
      inline name get_self()const { return _self; }

      /**
       * Get the datastream for this contract
       * @brief Get the datastream for this contract
       * @return datastream<const char*> - The datastream for this contract 
       */
      inline datastream<const char*> get_datastream()const { return _ds; }

   protected:
      /**
       * The name of this contract
       *
       * @brief The name of this contract.
       */
      name _self;

      /**
       * The datastream for this contract
       *@ The datastream for this contract
       */
      datastream<const char*> _ds = datastream<const char*>(nullptr, 0);
};

/// @} contracttype
} /// namespace eosio
