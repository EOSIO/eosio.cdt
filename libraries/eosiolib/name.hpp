/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include "serialize.hpp"
#include "system.hpp"

#include <string>
#include <string_view>

namespace eosio {

   /**
    * @defgroup name
    * @ingroup types
    * @brief EOSIO Types
    * @{

    /*
    * Wraps a %uint64_t to ensure it is only passed to methods that expect a %name.
    * Ensures value is only passed to methods that expect a %name and that no mathematical
    * operations occur.  Also enables specialization of print
    */
   struct name {
   public:
      enum class raw : uint64_t {};

      /**
       * Construct a new name
       *
       * @brief Construct a new name object with a default value of 0
       *
       */
      constexpr name()
         : value{}
      {}

      /**
       * Construct a new name given a unit64_t value
       *
       * @brief Construct a new name object initialising value with val
       * @param val - The unit64_t value
       *
       */
      constexpr explicit name( uint64_t val )
         : value{val}
      {}

      /**
       * Construct a new name with a scoped enumerated type of raw uint64_t.
       *
       * @brief Construct a new name object, initialising value with r
       * @param r - The raw value which is a scoped enumerated type of unit64_t
       *
       */
      constexpr explicit name( name::raw r )
         : value(static_cast<uint64_t>(r))
      {}

      /**
       * Construct a new name given an string.
       *
       * @brief Construct a new name object, initialising value with str
       * @param str - The string value which validates and converts to a unit64_t
       *
       */
      constexpr explicit name( std::string_view str )
         : value{0}
      {
         if( 13 < str.size() )
            eosio::check( false, "string is too long to be a valid name" );
         if( str.empty() )
            return;

         uint8_t length = (12 < str.size()) ? 12 : str.size();

         for( decltype(length) i = 0; i < length; ++i ) {
            value <<= 5;
            value |= char_to_value( str[i] );
         }
         value <<= ( 4 + 5*(12 - length) );
         
         if( str.size() == 13 ) {
            uint64_t val = char_to_value( str[12] );
            if( 0x0FULL < val  )
               eosio::check(false, "thirteenth character in name cannot be a letter that comes after j");
            value |= val;
         }
      }

      /**
       *  Converts a %name Base32 symbol into its corresponding value
       *
       *  @param c - Character to be converted
       *  @return constexpr char - Converted value
       */
      static constexpr uint8_t char_to_value( char ch ) {
         switch(ch) {
            case '.':
               return 0;
               break;
            case '1' ... '5':
               return (ch - '1') + 1;
               break;
            case 'a' ... 'z':
               return (ch - 'a') + 6;
               break;
            default:
               eosio::check( false, "character is not in allowed character set for names" );
         }  
         return 0; // control flow will never reach here; just added to suppress warning
      }

      /**
       *  Returns the length of the %name
       */
      constexpr uint8_t length()const {
         if( value == 0 )
            return 0;
         
         constexpr uint64_t mask{0xF800000000000000ULL};

         uint64_t val{value};
         uint8_t length{};
         for( uint8_t i{}; i < 13; val <<= 5, ++i )
            if( 0 < (val & mask) )
               length = i;
         return length + 1;
      }

      /**
       *  Returns the suffix of the %name
       */
      constexpr name suffix()const {
         uint32_t remaining_bits_after_last_actual_dot{};
         uint32_t tmp{};
         for( int32_t remaining_bits{59}; 4 <= remaining_bits; remaining_bits -= 5 ) { // Remaining_bits must remain signed integer
            // Get characters in name, one-by-one, in order from left to right (not including the 13th character)
            uint8_t ch = (value >> remaining_bits) & 0x1FULL;
            if( ch ) // if this character is not a dot
               remaining_bits_after_last_actual_dot = tmp;
            else // if this character is a dot
               tmp = static_cast<uint32_t>(remaining_bits);
         }

         uint64_t thirteenth_character{value & 0x0FULL};
         if( thirteenth_character ) // if 13th character is not a dot
            remaining_bits_after_last_actual_dot = tmp;

         if( remaining_bits_after_last_actual_dot == 0 ) // there is no actual dot in the %name other than potentially leading dots
            return name{value};

         // At this point remaining_bits_after_last_actual_dot has to be within the range of 4 to 59 (and restricted to increments of 5)

         // Mask for remaining bits corresponding to characters after last actual dot, except for 4 least significant bits (corresponds to 13th character)
         uint64_t mask{(1ULL << remaining_bits_after_last_actual_dot) - 16};
         uint32_t shift{64 - remaining_bits_after_last_actual_dot};

         return name{ ((value & mask) << shift) + (thirteenth_character << (shift-1)) };
      }

      /**
       * Casts a name to raw
       *
       * @return Returns an instance of raw based on the value of a name
       */
      constexpr operator raw()const { return raw(value); }

      /**
       * Explicit cast to bool of the uint64_t value of the name
       *
       * @return Returns true if the name is set to the default value of 0 else true
       */
      constexpr explicit operator bool()const { return value != 0; }

      /**
       *  Writes the %name as a string to char buffer
       *
       *  @pre Appropriate Size Precondition: (begin + 13) <= end and (begin + 13) does not overflow
       *  @pre Valid Memory Region Precondition: The range [begin, end) must be a valid range of memory to write to.
       *  @param begin - The start of the char buffer
       *  @param end - Just past the end of the char buffer
       *  @return char* - Just past the end of the last character written (returns begin if the Appropriate Size Precondition is not satisfied)
       *  @post If the Appropriate Size Precondition is satisfied, the range [begin, returned pointer) contains the string representation of the %name
       */
      char* write_as_string( char* begin, char* end )const {
         static const char* charmap{".12345abcdefghijklmnopqrstuvwxyz"};
         constexpr uint64_t mask{0xF800000000000000ULL};

         if( (begin + 13) < begin || end < (begin + 13) ) return begin;

         uint64_t val{value};
         for( uint8_t i{}; i < 13; ++i, val <<= 5 ) {
            if( val == 0 ) return begin;

            uint8_t index = (val & mask) >> ((i == 12) ? 60 : 59);
            *begin = charmap[index];
            ++begin;
         }
         return begin;
      }

      /**
       *  Returns the name as a string.
       *
       *  @brief Returns the name value as a string by calling write_as_string(); returning the buffer produced by write_as_string()
       */
      std::string to_string()const {
         char buffer[13];
         char* end{write_as_string( buffer, buffer + sizeof(buffer) )};
         return {buffer, end};
      }

      /**
       * Equivalency operator. Returns true if a == b
       *
       * @return boolean - true if both provided %name values are the same
       */
      friend constexpr bool operator==( const name& a, const name& b ) {
         return a.value == b.value;
      }

      /**
       * Inverted equivalency operator. Returns true if a != b
       *
       * @return boolean - true if both provided %name values are not the same
       */
      friend constexpr bool operator!=( const name& a, const name& b ) {
         return a.value != b.value;
      }

      /**
       * Less than operator. Returns true if a < b
       *
       * @return boolean - true if %name a is less than b
       */
      friend constexpr bool operator<( const name& a, const name& b ) {
         return a.value < b.value;
      }

      /**
       * Less than or equal to operator. Returns true if a <= b
       *
       * @return boolean - true if %name a is less than or equal to b
       */
      friend constexpr bool operator<=( const name& a, const name& b ) {
         return a.value <= b.value;
      }

      /**
       * Greater than operator. Returns true if a > b
       *
       * @return boolean - true if %name a is greater than b
       */
      friend constexpr bool operator>( const name& a, const name& b ) {
         return a.value > b.value;
      }

      /**
       * Greater than or equal to operator. Returns true if a >= b
       *
       * @return boolean - true if %name a is greater than or equal to b
       */
      friend constexpr bool operator>=( const name& a, const name& b ) {
         return a.value >= b.value;
      }
      
      uint64_t value{};

      EOSLIB_SERIALIZE( name, (value) )
   };

   namespace detail {
      template <char... Str>
      struct to_const_char_arr {
         static constexpr const char value[] = {Str...};
      };
   } /// namespace detail
} /// namespace eosio

/**
 * %name literal operator
 *
 * @brief "foo"_n is a shortcut for name("foo")
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
template <typename T, T... Str>
inline constexpr eosio::name operator""_n() {
   constexpr auto x = eosio::name{std::string_view{eosio::detail::to_const_char_arr<Str...>::value, sizeof...(Str)}};
   return x;
}
#pragma clang diagnostic pop
