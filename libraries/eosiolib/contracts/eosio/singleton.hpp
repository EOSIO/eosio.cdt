#pragma once
#include "multi_index.hpp"
#include "system.hpp"

namespace  eosio {

   /**
    *  @defgroup singleton Singleton Table
    *  @ingroup contracts
    *  @brief Defines EOSIO Singleton Table used with %multiindex
    */

   /**
    *  This wrapper uses a single table to store named objects various types.
    *
    *  @ingroup singleton
    *  @tparam SingletonName - the name of this singleton variable
    *  @tparam T - the type of the singleton
    */
   template<name::raw SingletonName, typename T>
   class singleton
   {
      /**
       * Primary key of the data inside singleton table
       */
      constexpr static uint64_t pk_value = static_cast<uint64_t>(SingletonName);

      /**
       * Structure of data inside the singleton table
       */
      struct row {
         /**
          * Value to be stored inside the singleton table
          */
         T value;

         /**
          * Get primary key of the data
          *
          * @return uint64_t - Primary Key
          */
         uint64_t primary_key() const { return pk_value; }

         EOSLIB_SERIALIZE( row, (value) )
      };

      typedef eosio::multi_index<SingletonName, row> table;

      public:

         /**
          * Construct a new singleton object given the table's owner and the scope
          *
          * @param code - The table's owner
          * @param scope - The scope of the table
          */
         singleton( name code, uint64_t scope ) : _t( code, scope ) {}

         /**
          *  Check if the singleton table exists
          *
          * @return true - if exists
          * @return false - otherwise
          */
         bool exists() {
            return _t.find( pk_value ) != _t.end();
         }

         /**
          * Get the value stored inside the singleton table. Will throw an exception if it doesn't exist
          *
          * @brief Get the value stored inside the singleton table
          * @return T - The value stored
          */
         T get() {
            auto itr = _t.find( pk_value );
            eosio::check( itr != _t.end(), "singleton does not exist" );
            return itr->value;
         }

         /**
          * Get the value stored inside the singleton table. If it doesn't exist, it will return the specified default value
          *
          * @param def - The default value to be returned in case the data doesn't exist
          * @return T - The value stored
          */
         T get_or_default( const T& def = T() ) {
            auto itr = _t.find( pk_value );
            return itr != _t.end() ? itr->value : def;
         }

         /**
          * Get the value stored inside the singleton table. If it doesn't exist, it will create a new one with the specified default value
          *
          * @param bill_to_account - The account to bill for the newly created data if the data doesn't exist
          * @param def - The default value to be created in case the data doesn't exist
          * @return T - The value stored
          */
         T get_or_create( name bill_to_account, const T& def = T() ) {
            auto itr = _t.find( pk_value );
            return itr != _t.end() ? itr->value
               : _t.emplace(bill_to_account, [&](row& r) { r.value = def; })->value;
         }

         /**
          * Set new value to the singleton table
          *
          * @param value - New value to be set
          * @param bill_to_account - Account to pay for the new value
          */
         void set( const T& value, name bill_to_account ) {
            auto itr = _t.find( pk_value );
            if( itr != _t.end() ) {
               _t.modify(itr, bill_to_account, [&](row& r) { r.value = value; });
            } else {
               _t.emplace(bill_to_account, [&](row& r) { r.value = value; });
            }
         }

         /**
          * Remove the only data inside singleton table
          */
         void remove( ) {
            auto itr = _t.find( pk_value );
            if( itr != _t.end() ) {
               _t.erase(itr);
            }
         }

      private:
         table _t;
   };
} /// namespace eosio
