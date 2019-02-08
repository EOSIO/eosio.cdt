#pragma once

#warning "<eosiolib/ignore.hpp> is deprecated use <eosio/ignore.hpp>"

namespace eosio {
   /**
    * @brief Tells the datastream to ignore this type, but allows the abi generator to add the correct type.
    * 
    * @details Currently non-ignore types can not succeed an ignore type in a method definition, i.e. void foo(float, ignore<int>) is allowed and void foo(float, ignore<int>, int) is not allowed.
    * @note This restriction will be relaxed in a later release.
    * Currently non-ignore types can not succeed an ignore type in a method definition, i.e. void foo(float, ignore<int>) is allowed and void foo(float, ignore<int>, int) is not allowed.
    * This restriction will be relaxed in a later release.
    */
   template <typename T>
   struct [[eosio::ignore]] ignore {};

    /**
    * Wrapper class to allow sending inline actions with the correct payload
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
