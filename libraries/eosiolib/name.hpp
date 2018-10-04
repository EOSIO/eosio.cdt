#pragma once

#include <eosiolib/system.h>
#include <string>
#include <string_view>

namespace eosio {

   /**
    *  Wraps a uint64_t to ensure it is only passed to methods that expect a Name and
    *  that no mathematical operations occur.  It also enables specialization of print
    *  so that it is printed as a base32 string.
    *
    *  @brief wraps a uint64_t to ensure it is only passed to methods that expect a Name
    *  @ingroup types
    */
   class name {
   public:
      constexpr name() : value(0) {}

      constexpr explicit name( uint64_t raw )
      :value(raw)
      {}

      constexpr explicit name( std::string_view str )
      :value(0)
      {
         if( str.size() > 13 ) {
            eosio_assert( false, "string is too long to be a valid name" );
         }

         auto n = std::min( str.size(), 12u );
         for( decltype(n) i = 0; i < n; ++i ) {
            value <<= 5;
            value |= char_to_value( str[i] );
         }
         value <<= ( 4 + 5*(12 - n) );
         if( str.size() == 13 ) {
            uint64_t v = char_to_value( str[12] );
            if( v > 0x0Full ) {
               eosio_assert(false, "thirteenth character in name cannot be a letter that comes after j");
            }
            value |= v;
         }
      }

      /**
       *  Converts a (eosio::name style) Base32 symbol into its corresponding value
       *
       *  @brief Converts a (eosio::name style) Base32 symbol into its corresponding value
       *  @param c - Character to be converted
       *  @return constexpr char - Converted value
       */
      static constexpr uint8_t char_to_value( char c ) {
         if( c == '.')
            return 0;
         else if( c >= '1' && c <= '5' )
            return (c - '1') + 1;
         else if( c >= 'a' && c <= 'z' )
            return (c - 'a') + 6;
         else
            eosio_assert( false, "character is not in allowed character set for names" );

         return 0; // control flow will never reach here; just added to suppress warning
      }


      constexpr name suffix()const {
         uint32_t remaining_bits_after_last_actual_dot = 0;
         uint32_t tmp = 0;
         for( int32_t remaining_bits = 59; remaining_bits >= 4; remaining_bits -= 5 ) { // Note: remaining_bits must remain signed integer
            // Get characters one-by-one in name in order from left to right (not including the 13th character)
            auto c = (value >> remaining_bits) & 0x1Full;
            if( !c ) { // if this character is a dot
               tmp = static_cast<uint32_t>(remaining_bits);
            } else { // if this character is not a dot
               remaining_bits_after_last_actual_dot = tmp;
            }
         }

         uint64_t thirteenth_character = value & 0x0Full;
         if( thirteenth_character ) { // if 13th character is not a dot
            remaining_bits_after_last_actual_dot = tmp;
         }

         if( remaining_bits_after_last_actual_dot == 0 ) // there is no actual dot in the name other than potentially leading dots
            return name{value};

         // At this point remaining_bits_after_last_actual_dot has to be within the range of 4 to 59 (and restricted to increments of 5).

         // Mask for remaining bits corresponding to characters after last actual dot, except for 4 least significant bits (corresponds to 13th character).
         uint64_t mask = (1ull << remaining_bits_after_last_actual_dot) - 16;
         uint32_t shift = 64 - remaining_bits_after_last_actual_dot;

         return name{ ((value & mask) << shift) + (thirteenth_character << (shift-1)) };
      }

      /**
       * Returns uint64_t repreresentation of the name
       */
      constexpr uint64_t raw()const { return value; }

      constexpr operator uint64_t()const { return value; }

      // keep in sync with name::operator string() in eosio source code definition for name
      std::string to_string() const {
         static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";

         std::string str(13,'.');

         uint64_t tmp = value;
         for( uint32_t i = 0; i <= 12; ++i ) {
            char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
            str[12-i] = c;
            tmp >>= (i == 0 ? 4 : 5);
         }

         trim_right_dots( str );
         return str;
      }

      /**
       * Equivalency operator. Returns true if a == b (are the same)
       *
       * @brief Equivalency operator
       * @return boolean - true if both provided names are the same
       */
      friend constexpr bool operator == ( const name& a, const name& b ) {
         return a.value == b.value;
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Inverted equivalency operator
       * @return boolean - true if both provided names are not the same
       */
      friend constexpr bool operator != ( const name& a, const name& b ) {
         return a.value != b.value;
      }

      /**
       * Less than operator. Returns true if a < b.
       * @brief Less than operator
       * @return boolean - true if name `a` is less than `b`
       */
      friend constexpr bool operator < ( const name& a, const name& b ) {
         return a.value < b.value;
      }

   private:
      uint64_t value = 0;

      static void trim_right_dots(std::string& str ) {
         const auto last = str.find_last_not_of('.');
         if (last != std::string::npos)
            str = str.substr(0, last + 1);
      }
   };

} /// namespace eosio

/**
 * name literal operator
 *
 * @brief "foo"_n is a shortcut for name{"foo"}
 */
inline constexpr eosio::name operator""_n(const char* s, std::size_t) {
   return eosio::name{s};
}
