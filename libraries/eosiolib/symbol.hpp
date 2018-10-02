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
    * \struct Stores the symbol code
    * @brief Stores the symbol code
    */
   class symbol_code {
      public:
         constexpr explicit symbol_code(uint64_t s): value(s) { }
         symbol_code( const char* str ) {
            uint32_t len = 0;
            while (str[len]) ++len;
            for ( uint32_t i=0; i < len; i++ ) {
               eosio_assert(str[i] >= 'A' && str[i] <= 'Z', "error, not a valid symbol, you can only use [A-Z]");
               value |= (uint64_t(str[i]) << (8*i));
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
                  for ( int i=0; i < 7; i++ ) {
                     sym >>= 8;
                     if ( (sym & 0xFF) ) return false;
                  }
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
         inline void set(uint64_t sym) { value = sym; }
         inline uint64_t raw()const { return value; }
        
      /**
       * %Print the symbol code
       *
       * @brief %Print the symbol code
       */
      void print()const {
         auto sym = value;
         for( int i = 0; i < 7; ++i ) {
            char c = (char)(sym & 0xff);
            if( !c ) return;
            prints_l(&c, 1 );
            sym >>= 8;
         }
      }

      /**
       * Equivalency operator. Returns true if a == b (are the same)
       *
       * @brief Equivalency operator
       * @return boolean - true if both provided symbol_codes are the same
       */
      friend bool operator == ( const symbol_code& a, const symbol_code& b ) {
         return a.value == b.value;
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Inverted equivalency operator
       * @return boolean - true if both provided symbol_codes are not the same
       */
      friend bool operator != ( const symbol_code& a, const symbol_code& b ) {
         return a.value != b.value;
      }

      /**
       * Less than operator. Returns true if a < b.
       * @brief Less than operator
       * @return boolean - true if symbol_code `a` is less than `b`
       */
      friend bool operator < ( const symbol_code& a, const symbol_code& b ) {
         return a.value < b.value;
      }

      private:
         uint64_t value = 0; 

      EOSLIB_SERIALIZE( symbol_code, (value) )
   };

   /**
    * \struct Stores information about a symbol
    *
    * @brief Stores information about a symbol
    */
   class symbol {
      public:
      constexpr symbol() : _precision(0),_code((uint64_t)0) { }
      constexpr explicit symbol( uint64_t sym ) : _precision(sym & 0xFF), _code(sym >> 8) { }
      symbol(const uint8_t precision, const symbol_code& sc) : _precision(precision), _code(sc) {
         eosio_assert(sc.is_valid(), "symbol is not valid");
      }
      constexpr symbol(const uint8_t precision, symbol_code&& sc) : _precision(precision), _code(std::move(sc)) {}

      /**
       * Is this symbol valid
       */
      constexpr bool is_valid()const  { return _code.is_valid(); }

      /**
       * This symbol's precision
       */
      constexpr uint64_t precision()const { return _precision; }

      /**
       * Returns representation of symbol name
       */
      constexpr symbol_code code()const      { return _code;   }

      /**
       * Returns uint64_t repreresentation of the symbol
       */
      constexpr uint64_t raw()const          { return (_code.raw() << 8) | _precision; }
      /**
       * The length of this symbol
       */
      constexpr uint32_t code_length()const { return _code.length(); }

      /**
       *
       */
      constexpr operator symbol_code()const { return _code; }

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
         _code.print();
      }
      /**
       * Equivalency operator. Returns true if a == b (are the same)
       *
       * @brief Equivalency operator
       * @return boolean - true if both provided symbols are the same
       */
      friend bool operator == ( const symbol& a, const symbol& b ) {
        return std::tie( a._code, a._precision ) == std::tie( b._code, b._precision );
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Inverted equivalency operator
       * @return boolean - true if both provided symbols are not the same
       */
      friend bool operator != ( const symbol& a, const symbol& b ) {
        return std::tie( a._code, a._precision ) != std::tie( b._code, b._precision );
      }

      /**
       * Less than operator. Returns true if a < b.
       * @brief Less than operator
       * @return boolean - true if symbol `a` is less than `b`
       */
      friend bool operator < ( const symbol& a, const symbol& b ) {
        return std::tie( a._code, a._precision ) < std::tie( b._code, b._precision );
      }

      private:
     /**
      * The symbol code
      */
      symbol_code _code;
      uint8_t _precision;
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
       * @brief Equivalency operator
       * @return boolean - true if both provided symbols are the same
       */
      friend bool operator == ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) == std::tie( b.symbol, b.contract );
      }

      /**
       * Inverted equivalency operator. Returns true if a != b (are different)
       *
       * @brief Inverted equivalency operator
       * @return boolean - true if both provided symbols are the same
       */
      friend bool operator != ( const extended_symbol& a, const extended_symbol& b ) {
        return std::tie( a.symbol, a.contract ) != std::tie( b.symbol, b.contract );
      }

      /**
       * Less than operator. Returns true if a < b.
       * @brief Less than operator
       * @return boolean - true if symbol `a` is less than `b`
       */
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

   namespace detail {
      template <size_t N, char C, char... Str>
      struct calculate_sym_code {
         static_assert( C >= 'A' && C <= 'Z', "not a valid symbol, must use only [A-Z]");
         static constexpr uint64_t value = ((uint64_t(C)) << (8*N)) + calculate_sym_code<N+1, Str...>::value;
      };
      template <size_t N, char C>
      struct calculate_sym_code<N, C> {
         static_assert( C >= 'A' && C <= 'Z', "not a valid symbol, must use only [A-Z]");
         static constexpr uint64_t value = ((uint64_t(C)) << (8*N));
      };

   } // ns detail
} /// namespace eosio

template <typename T, T... Str>
constexpr eosio::symbol_code operator""_s() {
   return eosio::symbol_code(eosio::detail::calculate_sym_code<0, Str...>::value);
}
