/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <eosiolib/types.h>
#include <eosiolib/name.hpp>
#include <functional>
#include <tuple>

namespace eosio {

   typedef std::vector<std::tuple<uint16_t,std::vector<char>>> extensions_type;

} // namespace eosio

namespace std {
   /**
    *  Provide less for checksum256
    *  @brief Provide less for checksum256
    */
   template<>
   struct less<checksum256> : binary_function<checksum256, checksum256, bool> {
      bool operator()( const checksum256& lhs, const checksum256& rhs ) const {
         return memcmp(&lhs, &rhs, sizeof(lhs)) < 0;
      }
   };

} // namespace std

/**
 * Equality Operator for checksum256
 *
 * @brief Equality Operator for checksum256
 * @param lhs - First data to be compared
 * @param rhs - Second data to be compared
 * @return true - if equal
 * @return false - if unequal
 */
inline bool operator==(const checksum256& lhs, const checksum256& rhs) {
   return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

/**
 * Equality Operator for checksum160
 *
 * @brief Equality Operator for checksum256
 * @param lhs - First data to be compared
 * @param rhs - Second data to be compared
 * @return true - if equal
 * @return false - if unequal
 */
inline bool operator==(const checksum160& lhs, const checksum160& rhs) {
   return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

/**
 * Equality Operator for checksum160
 *
 * @brief Equality Operator for checksum256
 * @param lhs - First data to be compared
 * @param rhs - Second data to be compared
 * @return true - if unequal
 * @return false - if equal
 */
inline bool operator!=(const checksum160& lhs, const checksum160& rhs) {
   return memcmp(&lhs, &rhs, sizeof(lhs)) != 0;
}
