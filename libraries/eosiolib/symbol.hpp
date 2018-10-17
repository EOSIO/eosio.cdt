/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/system.h>
#include <eosiolib/print.h>
#include <eosiolib/name.hpp>
#include <eosiolib/serialize.hpp>
#include <tuple>
#include <limits>
#include <string_view>

namespace eosio {

  /**
   *  @defgroup symbolapi Symbol API
   *  @brief Defines API for managing symbols
   *  @ingroup contractdev
   */

  /**
   *  @defgroup symbolcppapi Symbol CPP API
   *  @brief Defines %CPP API for managing symbols
   *  @ingroup symbolapi
   *  @{
   */

   /**
    * \struct Stores the symbol code
    * @brief Stores the symbol code
    */
   class symbol_code {
   public:
      constexpr symbol_code() : value(0) {}

      constexpr explicit symbol_code( uint64_t raw )
      :value(raw)
      {}

      constexpr explicit symbol_code( std::string_view str )
      :value(0)
      {
         if( str.size() > 7 ) {
            eosio_assert( false, "string is too long to be a valid symbol_code" );
         }
         for( auto itr = str.rbegin(); itr != str.rend(); ++itr ) {
            if( *itr < 'A' || *itr > 'Z') {
               eosio_assert( false, "only uppercase letters allowed in symbol_code string" );
            }
            value <<= 8;
            value |= *itr;
         }
      }

      /**
       * Checks if the symbol code is valid
       * @return true - if symbol is valid
       */
      constexpr bool is_valid()const {
         auto sym = value;
         for ( int i=0; i < 7; i++ ) {
            char c = (char)(sym & 0xFF);
            if ( !('A' <= c && c <= 'Z') ) return false;
            sym >>= 8;
            if ( !(sym & 0xFF) ) {
               do {
                  sym >>= 8;
                  if ( (sym & 0xFF) ) return false;
                  i++;
               } while( i < 7 );
            }
         }
         return true;
      }

      /**
       * Returns the character length of the provided symbol
       *
       * @return length - character length of the provided symbol
       */
      constexpr uint32_t length()const {
         auto sym = value;
         uint32_t len = 0;
         while (sym & 0xFF && len <= 7) {
            len++;
            sym >>= 8;
         }
         return len;
      }

      constexpr uint64_t raw()const { return value; }

      constexpr explicit operator bool()const { return value != 0; }

      /**
       *  Writes the symbol_code as a string to the provided char buffer
       *
       *
       *  @brief Writes the symbol_code as a string to the provided char buffer
       *  @pre Appropriate Size Precondition: (begin + 7) <= end and (begin + 7) does not overflow
       *  @pre Valid Memory Region Precondition: The range [begin, end) must be a valid range of memory to write to.
       *  @param begin - The start of the char buffer
       *  @param end - Just past the end of the char buffer
       *  @return char* - Just past the end of the last character written (returns begin if the Appropriate Size Precondition is not satisfied)
       *  @post If the Appropriate Size Precondition is satisfied, the range [begin, returned pointer) contains the string representation of the symbol_code.
       */
      char* write_as_string( char* begin, char* end )const {
         constexpr uint64_t mask = 0xFFull;

         if( (begin + 7) < begin || (begin + 7) > end ) return begin;

         auto v = value;
         for( auto i = 0; i < 7; ++i, v >>= 8 ) {
            if( v == 0 ) return begin;

            *begin = static_cast<char>(v & mask);
            ++begin;
         }

         return begin;
      }

      std::string to_string()const {
         char buffer[7];
         auto end = write_as_string( buffer, buffer + sizeof(buffer) );
         return {buffer, end};
      }

      /**
       * Equivalency operator. Returns true if a == b (are the same)
       *
       * @brief Equivalency operator
       * @return boolean - true if both provided symbol_codes are the same
       */
      friend constexpr bool operator == ( const symbol_code& a, const symbol_code& b ) {
         return a.value == b.value;
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Inverted equivalency operator
       * @return boolean - true if both provided symbol_codes are not the same
       */
      friend constexpr bool operator != ( const symbol_code& a, const symbol_code& b ) {
         return a.value != b.value;
      }

      /**
       * Less than operator. Returns true if a < b.
       * @brief Less than operator
       * @return boolean - true if symbol_code `a` is less than `b`
       */
      friend constexpr bool operator < ( const symbol_code& a, const symbol_code& b ) {
         return a.value < b.value;
      }

   private:
      uint64_t value = 0;
   };

   /**
    * \struct Stores information about a symbol
    *
    * @brief Stores information about a symbol
    */
   class symbol {
   public:
      constexpr symbol() : value(0) {}

      constexpr explicit symbol( uint64_t raw ) : value(raw) {}

      constexpr symbol( symbol_code sc, uint8_t precision )
      : value( (sc.raw() << 8) | static_cast<uint64_t>(precision) )
      {}

      constexpr symbol( std::string_view ss, uint8_t precision )
      : value( (symbol_code(ss).raw() << 8)  | static_cast<uint64_t>(precision) )
      {}

      /**
       * Is this symbol valid
       */
      constexpr bool is_valid()const                 { return code().is_valid(); }

      /**
       * This symbol's precision
       */
      constexpr uint8_t precision()const             { return static_cast<uint8_t>( value & 0xFFull ); }

      /**
       * Returns representation of symbol name
       */
      constexpr symbol_code code()const              { return symbol_code{value >> 8};   }

      /**
       * Returns uint64_t repreresentation of the symbol
       */
      constexpr uint64_t raw()const                  { return value; }

      constexpr explicit operator bool()const { return value != 0; }

      /**
       * %Print the symbol
       *
       * @brief %Print the symbol
       */
      void print( bool show_precision = true )const {
         if( show_precision ){
            printui( static_cast<uint64_t>(precision()) );
            prints(",");
         }
         char buffer[7];
         auto end = code().write_as_string( buffer, buffer + sizeof(buffer) );
         if( buffer < end )
            prints_l( buffer, (end-buffer) );
      }

      /**
       * Equivalency operator. Returns true if a == b (are the same)
       *
       * @brief Equivalency operator
       * @return boolean - true if both provided symbols are the same
       */
      friend constexpr bool operator == ( const symbol& a, const symbol& b ) {
         return a.value == b.value;
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Inverted equivalency operator
       * @return boolean - true if both provided symbols are not the same
       */
      friend constexpr bool operator != ( const symbol& a, const symbol& b ) {
         return a.value != b.value;
      }

      /**
       * Less than operator. Returns true if a < b.
       * @brief Less than operator
       * @return boolean - true if symbol `a` is less than `b`
       */
      friend constexpr bool operator < ( const symbol& a, const symbol& b ) {
         return a.value < b.value;
      }

   private:
      uint64_t value = 0;
   };

   /**
    * \struct Extended asset which stores the information of the owner of the symbol
    *
    */
   class extended_symbol
   {
   public:
      constexpr extended_symbol() {}

      constexpr extended_symbol( symbol sym, name con ) : symbol(sym), contract(con) {}

      constexpr symbol get_symbol() const { return symbol; }

      constexpr name   get_contract() const { return contract; }

      /**
       * %Print the extended symbol
       *
       * @brief %Print the extended symbol
       */
      void print( bool show_precision = true )const {
         symbol.print( show_precision );
         prints("@");
         printn( contract.value );
      }

      /**
       * Equivalency operator. Returns true if a == b (are the same)
       *
       * @brief Equivalency operator
       * @return boolean - true if both provided extended_symbols are the same
       */
      friend constexpr bool operator == ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) == std::tie( b.symbol, b.contract );
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Inverted equivalency operator
       * @return boolean - true if both provided extended_symbols are not the same
       */
      friend constexpr bool operator != ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) != std::tie( b.symbol, b.contract );
      }

      /**
       * Less than operator. Returns true if a < b.
       * @brief Less than operator
       * @return boolean - true if extended_symbol `a` is less than `b`
       */
      friend constexpr bool operator < ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) < std::tie( b.symbol, b.contract );
      }

   private:
      symbol symbol; ///< the symbol
      name   contract; ///< the token contract hosting the symbol

      EOSLIB_SERIALIZE( extended_symbol, (symbol)(contract) )
   };

   // }@ symbolapi

} /// namespace eosio
