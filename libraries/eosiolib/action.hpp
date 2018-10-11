/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <eosiolib/action.h>
#include <eosiolib/datastream.hpp>
#include <eosiolib/serialize.hpp>

#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_tuple.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/facilities/overload.hpp>

namespace eosio {

   /**
    * @defgroup actioncppapi Action C++ API
    * @ingroup actionapi
    * @brief Defines type-safe C++ wrapers for querying action and sending action
    *
    * @note There are some methods from the @ref actioncapi that can be used directly from C++
    *
    * @{
    */

   /**
    *
    *  This method unpacks the current action at type T.
    *
    *  @brief Interpret the action body as type T.
    *  @return Unpacked action data casted as T.
    *
    *  Example:
    *
    *  @code
    *  struct dummy_action {
    *    char a; //1
    *    unsigned long long b; //8
    *    int  c; //4
    *
    *    EOSLIB_SERIALIZE( dummy_action, (a)(b)(c) )
    *  };
    *  dummy_action msg = unpack_action_data<dummy_action>();
    *  @endcode
    */
   template<typename T>
   T unpack_action_data() {
      constexpr size_t max_stack_buffer_size = 512;
      size_t size = action_data_size();
      char* buffer = (char*)( max_stack_buffer_size < size ? malloc(size) : alloca(size) );
      read_action_data( buffer, size );
      return unpack<T>( buffer, size );
   }

   /**
    *  Add the specified account to set of accounts to be notified
    *
    *  @brief Add the specified account to set of accounts to be notified
    *  @param notify_account - name of the account to be verified
    */
   inline void require_recipient( name notify_account ){
      ::require_recipient( notify_account.value );
   }

   /**
    *  All of the listed accounts will be added to the set of accounts to be notified
    *
    *  This helper method enables you to add multiple accounts to accounts to be notified list with a single
    *  call rather than having to call the similar C API multiple times.
    *
    *  @note action.code is also considered as part of the set of notified accounts
    *
    *  @brief Notify an account for this action
    *  @param notify_account account to be notified
    *  @param remaining_accounts accounts to be notified
    *
    *  Example:
    *
    *  @code
    *  require_recipient(N(Account1), N(Account2), N(Account3)); // throws exception if any of them not in set.
    *  @endcode
    */
   template<typename... accounts>
   void require_recipient( name notify_account, accounts... remaining_accounts ){
      ::require_recipient( notify_account.value );
      require_recipient( remaining_accounts... );
   }

   /**
    *  Verifies that @ref name exists in the set of provided auths on a action. Fails if not found.
    *
    *  @brief Verify specified account exists in the set of provided auths
    *  @param name - name of the account to be verified
    */
   inline void require_auth( name n ) {
      ::require_auth( n.value );
   }

   /**
    * Packed representation of a permission level (Authorization)
    *
    * @brief Packed representation of a permission level (Authorization)
    */
   struct permission_level {
      /**
       * Construct a new permission level object with actor name and permission name
       *
       * @brief Construct a new permission level object
       * @param a - Name of the account who owns this authorization
       * @param p - Name of the permission
       */
      permission_level( name a, name p ):actor(a),permission(p){}

      /**
       * Default Constructor
       *
       * @brief Construct a new permission level object
       */
      permission_level(){}

      /**
       * Name of the account who owns this permission
       *
       * @brief Name of the account who owns this permission
       */
      name    actor;
      /**
       * Name of the permission
       *
       * @brief Name of the permission
       */
      name    permission;

      /**
       * Check equality of two permissions
       *
       * @brief Check equality of two permissions
       * @param a - first permission to compare
       * @param b - second permission to compare
       * @return true if equal
       * @return false if unequal
       */
      friend constexpr bool operator == ( const permission_level& a, const permission_level& b ) {
         return std::tie( a.actor, a.permission ) == std::tie( b.actor, b.permission );
      }

      EOSLIB_SERIALIZE( permission_level, (actor)(permission) )
   };

   /**
    * Require the specified authorization for this action. If this action doesn't contain the specified auth, it will fail.
    *
    * @brief Require the specified authorization for this action
    *
    * @param level - Authorization to be required
    */
   inline void require_auth( const permission_level& level ) {
      ::require_auth2( level.actor.value, level.permission.value );
   }

   /**
    *  Verifies that @ref n has auth.
    *
    *  @brief Verifies that @ref n has auth.
    *  @param n - name of the account to be verified
    */
   inline bool has_auth( name n ) {
      return ::has_auth( n.value );
   }

   /**
    *  Verifies that @ref n is an existing account.
    *
    *  @brief Verifies that @ref n is an existing account.
    *  @param n - name of the account to check
    */
   inline bool is_account( name n ) {
      return ::is_account( n.value );
   }

   /**
    * This is the packed representation of an action along with
    * meta-data about the authorization levels.
    *
    * @brief Packed representation of an action
    */
   struct action {
      /**
       * Name of the account the action is intended for
       *
       * @brief Name of the account the action is intended for
       */
      name                       account;

      /**
       * Name of the action
       *
       * @brief Name of the action
       */
      name                       name;

      /**
       * List of permissions that authorize this action
       *
       * @brief List of permissions that authorize this action
       */
      std::vector<permission_level>   authorization;

      /**
       * Payload data
       *
       * @brief Payload data
       */
      std::vector<char>               data;

      /**
       * Default Constructor
       *
       * @brief Construct a new action object
       */
      action() = default;

      /**
       * Construct a new action object with the given action struct
       *
       * @brief Construct a new action object with the given permission, action receiver, action name, action struct
       * @tparam T  - Type of action struct, must be serializable by `pack(...)`
       * @param auth - The permissions that authorizes this action
       * @param a -  The name of the account this action is intended for (action receiver)
       * @param n - The name of the action
       * @param value - The action struct that will be serialized via pack into data
       */
      template<typename T>
      action( const permission_level& auth, struct name a, struct name n, T&& value )
      :account(a), name(n), authorization(1,auth), data(pack(std::forward<T>(value))) {}

      /**
       * Construct a new action object with the given action struct
       *
       * @brief Construct a new action object with the given list of permissions, action receiver, action name, action struct
       * @tparam T  - Type of action struct, must be serializable by `pack(...)`
       * @param auths - The list of permissions that authorize this action
       * @param a -  The name of the account this action is intended for (action receiver)
       * @param n - The name of the action
       * @param value - The action struct that will be serialized via pack into data
       */
      template<typename T>
      action( std::vector<permission_level> auths, struct name a, struct name n, T&& value )
      :account(a), name(n), authorization(std::move(auths)), data(pack(std::forward<T>(value))) {}

      EOSLIB_SERIALIZE( action, (account)(name)(authorization)(data) )

      /**
       * Send the action as inline action
       *
       * @brief Send the action as inline action
       */
      void send() const {
         auto serialize = pack(*this);
         ::send_inline(serialize.data(), serialize.size());
      }

      /**
       * Send the action as inline context free action
       *
       * @brief Send the action as inline context free action
       * @pre This action should not contain any authorizations
       */
      void send_context_free() const {
         eosio_assert( authorization.size() == 0, "context free actions cannot have authorizations");
         auto serialize = pack(*this);
         ::send_context_free_inline(serialize.data(), serialize.size());
      }

      /**
       * Retrieve the unpacked data as T
       *
       * @brief Retrieve the unpacked data as T
       * @tparam T expected type of data
       * @return the action data
       */
      template<typename T>
      T data_as() {
         return unpack<T>( &data[0], data.size() );
      }

   };

   ///@} actioncpp api

   template<typename... Args>
   void dispatch_inline( name code, name act,
                         std::vector<permission_level> perms,
                         std::tuple<Args...> args ) {
      action( perms, code, act, std::move(args) ).send();
   }

   template<typename, name::raw>
   struct inline_dispatcher;


   template<typename T, name::raw Name, typename... Args>
   struct inline_dispatcher<void(T::*)(Args...), Name> {
      static void call(name code, const permission_level& perm, std::tuple<Args...> args) {
         dispatch_inline(code, name(Name), std::vector<permission_level>(1, perm), std::move(args));
      }
      static void call(name code, std::vector<permission_level> perms, std::tuple<Args...> args) {
         dispatch_inline(code, name(Name), std::move(perms), std::move(args));
      }
   };

} // namespace eosio

#define INLINE_ACTION_SENDER3( CONTRACT_CLASS, FUNCTION_NAME, ACTION_NAME  )\
::eosio::inline_dispatcher<decltype(&CONTRACT_CLASS::FUNCTION_NAME), ACTION_NAME>::call

#define INLINE_ACTION_SENDER2( CONTRACT_CLASS, NAME )\
INLINE_ACTION_SENDER3( CONTRACT_CLASS, NAME, ::eosio::name(#NAME) )

#define INLINE_ACTION_SENDER(...) BOOST_PP_OVERLOAD(INLINE_ACTION_SENDER,__VA_ARGS__)(__VA_ARGS__)

/**
 * @addtogroup actioncppapi
 * Additional documentation for group
 * @{
 */

/**
 * Send inline action
 *
 * @brief Send inline action
 * @param CONTRACT - The account this action is intended for
 * @param NAME - The name of the action
 * @param ... - The member of the action specified as ("action_member1_name", action_member1_value)("action_member2_name", action_member2_value)
 */
#define SEND_INLINE_ACTION( CONTRACT, NAME, ... )\
INLINE_ACTION_SENDER(std::decay_t<decltype(CONTRACT)>, NAME)( (CONTRACT).get_self(),\
BOOST_PP_TUPLE_ENUM(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), BOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__)) );

   /// @}
