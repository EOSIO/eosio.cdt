#pragma once

#include "datastream.hpp"

namespace eosio {
   /**
    * @defgroup ignore
    * @ingroup core
    * @brief Enables telling the datastream to ignore this type, but allowing abi generator to add the correct type.
    */

   /**
    * Tells the datastream to ignore this type, but allows the abi generator to add the correct type.
    *
    * @ingroup ignore
    * @details Currently non-ignore types can not succeed an ignore type in a method definition, i.e. void foo(float, ignore<int>) is allowed and void foo(float, ignore<int>, int) is not allowed.
    * @note This restriction will be relaxed in a later release. Currently non-ignore types can not succeed an ignore type in a method definition, i.e. void foo(float, ignore<int>) is allowed and void foo(float, ignore<int>, int) is not allowed.
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

   /**
    *  Serialize an ignored_wrapper type into a stream
    *
    *  @brief Serialize ignored_wrapper<T>'s T value
    *  @param ds - The stream to write
    *  @param val - The value to serialize
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream, typename T>
   inline DataStream& operator<<(DataStream& ds, const ::eosio::ignore_wrapper<T>& val) {
     ds << val.value;
     return ds;
   }

   /**
    *  Serialize an ignored type into a stream
    *
    *  @brief Serialize an ignored type
    *  @param ds - The stream to write
    *  @param ignore - The value to serialize
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream, typename T>
   inline DataStream& operator<<(DataStream& ds, const ::eosio::ignore<T>& val) {
     return ds;
   }

   /**
    *  Deserialize an ignored type from a stream
    *
    *  @brief Deserialize an ignored type
    *  @param ds - The stream to read
    *  @param ignored - The destination for deserialized value
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream, typename T>
   inline DataStream& operator>>(DataStream& ds, ::eosio::ignore<T>&) {
     return ds;
   }
}
