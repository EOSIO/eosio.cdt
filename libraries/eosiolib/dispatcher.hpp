#pragma once
#include <eosiolib/print.hpp>
#include <eosiolib/action.hpp>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <boost/mp11/tuple.hpp>

namespace eosio {

   template<typename Contract, typename FirstAction>
   bool dispatch( uint64_t code, uint64_t act ) {
      if( code == FirstAction::get_account() && FirstAction::get_name() == act ) {
         Contract().on( unpack_action_data<FirstAction>() );
         return true;
      }
      return false;
   }


   /**
    * This method will dynamically dispatch an incoming set of actions to
    *
    * ```
    * static Contract::on( ActionType )
    * ```
    *
    * For this to work the Actions must be derived from eosio::contract
    *
    */
   template<typename Contract, typename FirstAction, typename SecondAction, typename... Actions>
   bool dispatch( uint64_t code, uint64_t act ) {
      if( code == FirstAction::get_account() && FirstAction::get_name() == act ) {
         Contract().on( unpack_action_data<FirstAction>() );
         return true;
      }
      return eosio::dispatch<Contract,SecondAction,Actions...>( code, act );
   }

   /**
    * @defgroup dispatcher Dispatcher API
    * @brief Defines functions to dispatch action to proper action handler inside a contract
    * @ingroup contractdev
    */

   /**
    * @defgroup dispatchercpp Dispatcher C++ API
    * @brief Defines C++ functions to dispatch action to proper action handler inside a contract
    * @ingroup dispatcher
    * @{
    */

   /**
    * Unpack the received action and execute the correponding action handler
    *
    * @brief Unpack the received action and execute the correponding action handler
    * @tparam T - The contract class that has the correponding action handler, this contract should be derived from eosio::contract
    * @tparam Q - The namespace of the action handler function
    * @tparam Args - The arguments that the action handler accepts, i.e. members of the action
    * @param obj - The contract object that has the correponding action handler
    * @param func - The action handler
    * @return true
    */
   template<typename T, typename... Args>
   bool execute_action( name self, name code, void (T::*func)(Args...)  ) {
      size_t size = action_data_size();

      //using malloc/free here potentially is not exception-safe, although WASM doesn't support exceptions
      constexpr size_t max_stack_buffer_size = 512;
      void* buffer = nullptr;
      if( size > 0 ) {
         buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
         read_action_data( buffer, size );
      }
      
      std::tuple<std::decay_t<Args>...> args;
      datastream<const char*> ds((char*)buffer, size);
      ds >> args;
      
      T inst(self, code, ds);

      auto f2 = [&]( auto... a ){
         ((&inst)->*func)( a... );
      };

      boost::mp11::tuple_apply( f2, args );
      if ( max_stack_buffer_size < size ) {
         free(buffer);
      }
      return true;
   }
 /// @}  dispatcher

// Helper macro for EOSIO_DISPATCH_INTERNAL
#define EOSIO_DISPATCH_INTERNAL( r, OP, elem ) \
   case eosio::name( BOOST_PP_STRINGIZE(elem) ).value: \
      eosio::execute_action( eosio::name(receiver), eosio::name(code), &OP::elem ); \
      break;

// Helper macro for EOSIO_DISPATCH
#define EOSIO_DISPATCH_HELPER( TYPE,  MEMBERS ) \
   BOOST_PP_SEQ_FOR_EACH( EOSIO_DISPATCH_INTERNAL, TYPE, MEMBERS )

/**
 * @addtogroup dispatcher
 * @{
 */

/**
 * Convenient macro to create contract apply handler
 * To be able to use this macro, the contract needs to be derived from eosio::contract
 *
 * @brief Convenient macro to create contract apply handler
 * @param TYPE - The class name of the contract
 * @param MEMBERS - The sequence of available actions supported by this contract
 *
 * Example:
 * @code
 * EOSIO_DISPATCH( eosio::bios, (setpriv)(setalimits)(setglimits)(setprods)(reqauth) )
 * @endcode
 */
#define EOSIO_DISPATCH( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      if( code == receiver ) { \
         switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
} \
 /// @}  dispatcher

}
