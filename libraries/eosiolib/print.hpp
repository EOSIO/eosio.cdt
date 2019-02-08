/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once
#include "print.h"
#include "name.hpp"
#include "symbol.hpp"
#include "fixed_bytes.hpp"

#include <utility>
#include <string>

#warning "<eosiolib/print.hpp> is deprecated use <eosio/print.hpp>"
/**
   *  @defgroup console Console C++ API
   *  @ingroup core
   *  @brief Defines C++ wrapper to log/print text messages
   *
   *  @details This API uses C++ variadic templates and type detection to
   *  make it easy to print any native type. You can even overload
   *  the `print()` method for your own custom types.
   *
   *  **Example:**
   *  ```
   *     print( "hello world, this is a number: ", 5 );
   *  ```
   *
   *  @section override Overriding Print for your Types
   *
   *  There are two ways to overload print:
   *  1. implement void print( const T& )
   *  2. implement T::print()const
   *
   *  @{
   */


namespace eosio {

   /**
    *  Prints string
    *
    *  @param ptr - a null terminated string
    */
   inline void print( const char* ptr ) {
      prints(ptr);
   }

   /**
    * Prints 8-128 bit signed integer
    *
    * @param num to be printed
    */
   template <typename T, std::enable_if_t<std::is_integral<std::decay_t<T>>::value &&
                                          std::is_signed<std::decay_t<T>>::value, int> = 0>
   inline void print( T num ) {
      if constexpr(std::is_same<T, int128_t>::value)
         printi128(&num);
      else if constexpr(std::is_same<T, char>::value)
         prints_l( &num, 1 );
      else
         printi(num);
   }

   /**
    * Prints 8-128 bit unsigned integer
    *
    * @param num to be printed
    */
   template <typename T, std::enable_if_t<std::is_integral<std::decay_t<T>>::value &&
                                          !std::is_signed<std::decay_t<T>>::value, int> = 0>
   inline void print( T num ) {
      if constexpr(std::is_same<T, uint128_t>::value)
         printui128(&num);
      else if constexpr(std::is_same<T, bool>::value)
         prints(num?"true":"false");
      else
         printui(num);
   }

   /**
    * Prints single-precision floating point number (i.e. float)
    *
    * @param num to be printed
    */
   inline void print( float num ) { printsf( num ); }

   /**
    * Prints double-precision floating point number (i.e. double)
    *
    * @param num to be printed
    */
   inline void print( double num ) { printdf( num ); }

   /**
    * Prints quadruple-precision floating point number (i.e. long double)
    *
    * @param num to be printed
    */
   inline void print( long double num ) { printqf( &num ); }

   /**
    * Prints fixed_bytes as a hexidecimal string
    *
    * @brief Prints fixed_bytes as a hexidecimal string
    * @param val to be printed
    */
   template<size_t Size>
   inline void print( const fixed_bytes<Size>& val ) {
      auto arr = val.extract_as_byte_array();
      printhex(static_cast<const void*>(arr.data()), arr.size());
   }

  /**
    * Prints fixed_bytes as a hexidecimal string
    *
    * @brief Prints fixed_bytes as a hexidecimal string
    * @param val to be printed
    */
   template<size_t Size>
   inline void print( fixed_bytes<Size>& val ) {
      print(static_cast<const fixed_bytes<Size>&>(val));
   }

   /**
    * Prints a 64 bit names as base32 encoded string
    *
    * @param name 64 bit name to be printed
    */
   inline void print( name name ) {
      printn(name.value);
   }

   /**
    * Prints a symbol_code
    *
    * @param sym_code symbol code to be printed
    */
   inline void print( eosio::symbol_code sym_code ) {
      char buffer[7];
      auto end = sym_code.write_as_string( buffer, buffer + sizeof(buffer) );
      if( buffer < end )
         prints_l( buffer, (end-buffer) );
   }

  /**
    * Prints class object
    *
    * @param t to be printed
    * @pre T must implements print() function
    */
   template<typename T, std::enable_if_t<!std::is_integral<std::decay_t<T>>::value, int> = 0>
   inline void print( T&& t ) {
      if constexpr (std::is_same<std::decay_t<T>, std::string>::value)
         prints_l( t.c_str(), t.size() );
      else if constexpr (std::is_same<std::decay_t<T>, char*>::value)
         prints(t);
      else
         t.print();
   }

   /**
    * Prints null terminated string
    *
    * @param s null terminated string to be printed
    */
   inline void print_f( const char* s ) {
      prints(s);
   }

   /**
    * Prints formatted string. It behaves similar to C printf/
    *
    * @tparam Arg - Type of the value used to replace the format specifier
    * @tparam Args - Type of the value used to replace the format specifier
    * @param s - Null terminated string with to be printed (it can contains format specifier)
    * @param val - The value used to replace the format specifier
    * @param rest - The values used to replace the format specifier
    *
    * Example:
    * @code
    * print_f("Number of apples: %", 10);
    * @endcode
    */
   template <typename Arg, typename... Args>
   inline void print_f( const char* s, Arg val, Args... rest ) {
      while ( *s != '\0' ) {
         if ( *s == '%' ) {
            print( val );
            print_f( s+1, rest... );
            return;
         }
         prints_l( s, 1 );
         s++;
      }
   }

    /**
     *  Print out value / list of values
     *
     *  @tparam Arg - Type of the value used to replace the format specifier
     *  @tparam Args - Type of the value used to replace the format specifier
     *  @param a - The value to be printed
     *  @param args - The other values to be printed
     *
     *  Example:
     *
     *  @code
     *  const char *s = "Hello World!";
     *  uint64_t unsigned_64_bit_int = 1e+18;
     *  uint128_t unsigned_128_bit_int (87654323456);
     *  uint64_t string_as_unsigned_64_bit = "abcde"_n;
     *  print(s , unsigned_64_bit_int, unsigned_128_bit_int, string_as_unsigned_64_bit);
     *  // Ouput: Hello World!100000000000000000087654323456abcde
     *  @endcode
     */
   template<typename Arg, typename... Args>
   void print( Arg&& a, Args&&... args ) {
      print(std::forward<Arg>(a));
      print(std::forward<Args>(args)...);
   }

   /**
    * Simulate C++ style streams
    */
   class iostream {};

   /**
    *  Overload c++ iostream
    *
    *  @tparam Arg - Type of the value used to replace the format specifier
    *  @tparam Args - Type of the value used to replace the format specifier
    *  @param out - Output strem
    *  @param v - The value to be printed
    *  @return iostream& - Reference to the input output stream
    *
    *  Example:
    *
    *  @code
    *  const char *s = "Hello World!";
    *  uint64_t unsigned_64_bit_int = 1e+18;
    *  uint128_t unsigned_128_bit_int (87654323456);
    *  uint64_t string_as_unsigned_64_bit = "abcde"_n;
    *  std::out << s << " " << unsigned_64_bit_int << " "  << unsigned_128_bit_int << " " << string_as_unsigned_64_bit;
    *  // Output: Hello World! 1000000000000000000 87654323456 abcde
    *  @endcode
    */
   template<typename T>
   inline iostream& operator<<( iostream& out, const T& v ) {
      print( v );
      return out;
   }

   static iostream cout;

   /// @} consolecppapi


}
