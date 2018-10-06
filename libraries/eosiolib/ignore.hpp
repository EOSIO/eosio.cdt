#pragma once

namespace eosio {
   /**
    * Tells the datastream not ignore this type, but allows the abi generator to add the correct type.
    * @brief Tells the datastream not ignore this type, but allows the abi generator to add the correct type.
    */
   template <typename T>
   struct [[eosio::ignore]] ignore {};

    /**
    * Wrapper class to allow sending inline actions with the correct payload 
    * @brief Wrapper class to allow sending inline actions with the correct payload 
    */
   template <typename T>
   struct ignore_wrapper {
      constexpr ignore_wrapper() {}
      constexpr ignore_wrapper(T val) : value(val) {}
      constexpr ignore_wrapper(ignore<T> val) {}
      constexpr inline T get() { return value; }
      constexpr operator T() { return value; }
      constexpr operator ignore<T>() { return {}; }
      T value;
   };
} //ns eosio
