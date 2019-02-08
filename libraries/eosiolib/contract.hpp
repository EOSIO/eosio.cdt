#pragma once

#include "name.hpp"
#include "datastream.hpp"


#warning "<eosiolib/contract.hpp> is deprecated use <eosio/contract.hpp>"

/**
 * @defgroup contract Contract
 * @ingroup types
 * @brief Defines contract type which is %base class for every EOSIO contract
 * @{
 */
namespace eosio {

/**
 * @brief %Base class for EOSIO contract.
 * @details %Base class for EOSIO contract. %A new contract should derive from this class, so it can make use of EOSIO_ABI macro.
 */
class contract {
   public:
      /**
       * Construct a new contract given the contract name
       *
       * @param receiver - The name of this contract
       * @param code - The code name of the action this contract is processing.
       * @param ds - The datastream used
       */
      contract( name receiver, name code, datastream<const char*> ds ):_self(receiver),_code(code),_ds(ds) {}

      /**
       *
       * Get this contract name
       *
       * @return name - The name of this contract
       */
      inline name get_self()const { return _self; }

      /**
       * The code name of the action this contract is processing.
       *
       * @return name - The code name of the action this contract is processing.
       */
      inline name get_code()const { return _code; }

      /**
       * Get the datastream for this contract
       *
       * @return datastream<const char*> - The datastream for this contract
       */
      inline datastream<const char*> get_datastream()const { return _ds; }

   protected:
      /**
       * The name of this contract
       */
      name _self;

      /**
       * The code name of the action this contract is processing.
       */
      name _code;

      /**
       * The datastream for this contract
       */
      datastream<const char*> _ds = datastream<const char*>(nullptr, 0);
};
}
