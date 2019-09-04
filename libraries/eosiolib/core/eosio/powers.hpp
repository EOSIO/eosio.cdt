#pragma once

#include "check.hpp"

#include <type_traits>
#include <utility>
#include <array>

namespace eosio {

   namespace detail {

      template<typename Generator, std::size_t... Is>
      constexpr auto generate_array_helper( Generator&& g, std::index_sequence<Is...> )
      -> std::array<decltype(g(std::size_t{}, sizeof...(Is))), sizeof...(Is)>
      {
          return {{g(Is, sizeof...(Is))...}};
      }

      template<std::size_t N, typename Generator>
      constexpr auto generate_array( Generator&& g ) {
          return generate_array_helper( std::forward<Generator>(g), std::make_index_sequence<N>{} );
      }

      template<typename T, T Base, uint8_t Exponent, uint64_t Value, bool Overflow = (Value * Base < Value)>
      struct largest_power_helper {
      private:
         static_assert( std::is_integral_v<T> && std::is_unsigned_v<T> &&!std::is_same_v<T, bool> );
         static_assert( Base > 1 );
         constexpr static T next_value = Value * Base;
         using next = largest_power_helper<T, Base, Exponent+1, next_value>;
      public:
         constexpr static T       value    = next::value;
         constexpr static uint8_t exponent = next::exponent;
      };

      template<typename T, T Base, uint8_t Exponent, uint64_t Value>
      struct largest_power_helper<T, Base, Exponent, Value, true> {
      private:
         static_assert( std::is_integral_v<T> && std::is_unsigned_v<T> &&!std::is_same_v<T, bool> );
         static_assert( Base > 1 );
         static_assert( Exponent < 255 );
      public:
         constexpr static T       value    = Value;
         constexpr static uint8_t exponent = Exponent;
      };

      template<typename T, T Base>
      struct largest_power {
      private:
         using helper = largest_power_helper<T, Base, 0, 1>;
      public:
         constexpr static T       value    = helper::value;
         constexpr static uint8_t exponent = helper::exponent;
      };

      template<typename T>
      constexpr T pow( T base, uint8_t exponent ) {
         if( base <= 1 ) check( false, "base must be at least 2" );
         T prior  = 1;
         T result = prior;
         for( uint8_t i = 0; i < exponent; ++i, prior = result ) {
            result = prior * base;
            if( result <= prior ) check( false, "overflow" );
         }
         return result;
      }

      template<typename T, T Base>
      constexpr T pow_generator( std::size_t i, std::size_t ) {
         return pow( Base, static_cast<uint8_t>(i) );
      }

   }

   template<uint8_t Base, typename T = uint64_t>
   inline constexpr auto powers_of_base = detail::generate_array<detail::largest_power<T, Base>::exponent + 1>( detail::pow_generator<T, Base> );

   /** @returns Base^exponent */
   template<uint8_t Base, typename T = uint64_t>
   constexpr T pow( uint8_t exponent ) {
      const auto& lookup_table = powers_of_base<Base, T>;
      if( exponent >= lookup_table.size() ) check( false, "overflow" );

      return lookup_table[exponent];
   }

}
