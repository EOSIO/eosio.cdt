#pragma once
#include <eosiolib/core_symbol.hpp>
#include <eosiolib/serialize.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/system.h>
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
    * uint64_t representation of a symbol code
    */
   typedef uint64_t symbol_code;

   /**
    * Converts string to uint64_t representation of symbol / non constexpr version
    *
    * @param precision - precision of symbol
    * @param str - the string representation of the symbol
    */
   static constexpr uint64_t string_to_symbol_code( uint8_t precision, const char* str ) {
      uint32_t len = 0;
      while( str[len] ) ++len;

      uint64_t result = 0;
      for( uint32_t i = 0; i < len; ++i ) {
         result |= (uint64_t(str[i]) << (8*(1+i)));
      }

      result |= uint64_t(precision);
      return result;
   }
 
   /**
    * Checks if provided symbol code is valid.
    *
    * @param sym - symbol code of type symbol code_
    * @return true - if symbol is valid
    */
   static constexpr bool is_valid_symbol_code( symbol_code sym ) {
      sym >>= 8;
      for( int i = 0; i < 7; ++i ) {
         char c = (char)(sym & 0xff);
         if( !('A' <= c && c <= 'Z')  ) return false;
         sym >>= 8;
         if( !(sym & 0xff) ) {
            do {
              sym >>= 8;
              if( (sym & 0xff) ) return false;
              ++i;
            } while( i < 7 );
         }
      }
      return true;
   }

   /**
    * Returns the character length of the provided symbol
    *
    * @param sym - symbol to retrieve length for (uint64_t)
    * @return length - character length of the provided symbol
    */
   static constexpr uint32_t symbol_code_length( symbol_code sym ) {
      sym >>= 8; /// skip precision
      uint32_t length = 0;
      while( sym & 0xff && length <= 7) {
         ++length;
         sym >>= 8;
      }

      return length;
   }

   /**
    * \struct Stores information about a symbol
    *
    * @brief Stores information about a symbol
    */
   class symbol {
      public:
      constexpr symbol() : value(0) { }
      constexpr explicit symbol(uint64_t s): value(s) { }
      constexpr symbol(uint8_t prec, const char* str) : value(string_to_symbol_code(prec, str)) {}

      /**
       * Is this symbol valid
       */
      constexpr bool is_valid()const  { return is_valid_symbol_code( value ); }

      /**
       * This symbol's precision
       */
      constexpr uint64_t precision()const { return value & 0xff; }

      /**
       * Returns uint64_t representation of symbol name
       */
      constexpr uint64_t name()const      { return value >> 8;   }

      /**
       * The length of this symbol
       */
      constexpr uint32_t code_length()const { return symbol_code_length( value ); }

      /**
       *
       */
      constexpr operator symbol_code()const { return value; }

      /**
       * %Print the symbol
       *
       * @brief %Print the symbol
       */
      void print(bool show_precision=true)const {
         if( show_precision ){
            ::eosio::print(precision());
            prints(",");
         }

         auto sym = value;
         sym >>= 8;
         for( int i = 0; i < 7; ++i ) {
            char c = (char)(sym & 0xff);
            if( !c ) return;
            prints_l(&c, 1 );
            sym >>= 8;
         }
      }
      private:
     /**
      * The symbol code
      */
      symbol_code value;


      EOSLIB_SERIALIZE( symbol, (value) )
   };

   /**
    * \struct Extended asset which stores the information of the owner of the symbol
    *
    */
   class extended_symbol
   {
      public:
      constexpr extended_symbol() {} 
      constexpr extended_symbol(const symbol sym, const name con) : symbol(sym), contract(con) {}
      constexpr inline symbol get_symbol() { return symbol; }
      constexpr inline name   get_contract() { return contract; }
      /**
       * %Print the extended symbol
       *
       * @brief %Print the extended symbol
       */
      void print()const {
         symbol.print();
         prints("@");
         printn( contract );
      }

      /**
       * Equivalency operator. Returns true if a == b (are the same)
       *
       * @brief Subtraction operator
       * @param a - The extended asset to be subtracted
       * @param b - The extended asset used to subtract
       * @return boolean - true if both provided symbols are the same
       */
      friend bool operator == ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) == std::tie( b.symbol, b.contract );
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Subtraction operator
       * @param a - The extended asset to be subtracted
       * @param b - The extended asset used to subtract
       * @return boolean - true if both provided symbols are the same
       */
      friend bool operator != ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) != std::tie( b.symbol, b.contract );
      }

      friend bool operator < ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) < std::tie( b.symbol, b.contract );
      }

     private:
     /**
      * The symbol
      */
     symbol symbol;

     /**
      * The owner of the symbol
      *
      * @brief The owner of the symbol
      */
     name contract;

      EOSLIB_SERIALIZE( extended_symbol, (symbol)(contract) )
   };

   // }@ symbolapi

} /// namespace eosio
