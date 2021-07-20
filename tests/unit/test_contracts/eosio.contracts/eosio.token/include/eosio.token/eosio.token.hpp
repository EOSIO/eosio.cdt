#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   /**
    * The `eosio.token` sample system contract defines the structures and actions that allow users to create, issue, and manage tokens for EOSIO based blockchains. It demonstrates one way to implement a smart contract which allows for creation and management of tokens. It is possible for one to create a similar contract which suits different needs. However, it is recommended that if one only needs a token with the below listed actions, that one uses the `eosio.token` contract instead of developing their own.
    * 
    * The `eosio.token` contract class also implements two useful public static methods: `get_supply` and `get_balance`. The first allows one to check the total supply of a specified token, created by an account and the second allows one to check the balance of a token for a specified account (the token creator account has to be specified as well).
    * 
    * The `eosio.token` contract manages the set of tokens, accounts and their corresponding balances, by using two internal multi-index structures: the `accounts` and `stats`. The `accounts` multi-index table holds, for each row, instances of `account` object and the `account` object holds information about the balance of one token. The `accounts` table is scoped to an eosio account, and it keeps the rows indexed based on the token's symbol.  This means that when one queries the `accounts` multi-index table for an account name the result is all the tokens that account holds at the moment.
    * 
    * Similarly, the `stats` multi-index table, holds instances of `currency_stats` objects for each row, which contains information about current supply, maximum supply, and the creator account for a symbol token. The `stats` table is scoped to the token symbol.  Therefore, when one queries the `stats` table for a token symbol the result is one single entry/row corresponding to the queried symbol token if it was previously created, or nothing, otherwise.
    */
   class [[eosio::contract("eosio.token")]] token : public contract {
      public:
         using contract::contract;

         /**
          * Allows `issuer` account to create a token in supply of `maximum_supply`. If validation is successful a new entry in statstable for token symbol scope gets created.
          *
          * @param issuer - the account that creates the token,
          * @param maximum_supply - the maximum supply set for the token created.
          *
          * @pre Token symbol has to be valid,
          * @pre Token symbol must not be already created,
          * @pre maximum_supply has to be smaller than the maximum supply allowed by the system: 1^62 - 1.
          * @pre Maximum supply must be positive;
          */
         [[eosio::action]]
         void create( const name&   issuer,
                      const asset&  maximum_supply);
         /**
          *  This action issues to `to` account a `quantity` of tokens.
          *
          * @param to - the account to issue tokens to, it must be the same as the issuer,
          * @param quntity - the amount of tokens to be issued,
          * @memo - the memo string that accompanies the token issue transaction.
          */
         [[eosio::action]]
         void issue( const name& to, const asset& quantity, const string& memo );

         /**
          * The opposite for create action, if all validations succeed,
          * it debits the statstable.supply amount.
          *
          * @param quantity - the quantity of tokens to retire,
          * @param memo - the memo string to accompany the transaction.
          */
         [[eosio::action]]
         void retire( const asset& quantity, const string& memo );

         /**
          * Allows `from` account to transfer to `to` account the `quantity` tokens.
          * One account is debited and the other is credited with quantity tokens.
          *
          * @param from - the account to transfer from,
          * @param to - the account to be transferred to,
          * @param quantity - the quantity of tokens to be transferred,
          * @param memo - the memo string to accompany the transaction.
          */
         [[eosio::action]]
         void transfer( const name&    from,
                        const name&    to,
                        const asset&   quantity,
                        const string&  memo );
         /**
          * Allows `ram_payer` to create an account `owner` with zero balance for
          * token `symbol` at the expense of `ram_payer`.
          *
          * @param owner - the account to be created,
          * @param symbol - the token to be payed with by `ram_payer`,
          * @param ram_payer - the account that supports the cost of this action.
          *
          * More information can be read [here](https://github.com/EOSIO/eosio.contracts/issues/62)
          * and [here](https://github.com/EOSIO/eosio.contracts/issues/61).
          */
         [[eosio::action]]
         void open( const name& owner, const symbol& symbol, const name& ram_payer );

         /**
          * This action is the opposite for open, it closes the account `owner`
          * for token `symbol`.
          *
          * @param owner - the owner account to execute the close action for,
          * @param symbol - the symbol of the token to execute the close action for.
          *
          * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
          * @pre If the pair of owner plus symbol exists, the balance has to be zero.
          */
         [[eosio::action]]
         void close( const name& owner, const symbol& symbol );

         static asset get_supply( const name& token_contract_account, const symbol_code& sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         static asset get_balance( const name& token_contract_account, const name& owner, const symbol_code& sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }

         using create_action = eosio::action_wrapper<"create"_n, &token::create>;
         using issue_action = eosio::action_wrapper<"issue"_n, &token::issue>;
         using retire_action = eosio::action_wrapper<"retire"_n, &token::retire>;
         using transfer_action = eosio::action_wrapper<"transfer"_n, &token::transfer>;
         using open_action = eosio::action_wrapper<"open"_n, &token::open>;
         using close_action = eosio::action_wrapper<"close"_n, &token::close>;
      private:
         struct [[eosio::table]] account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         struct [[eosio::table]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef eosio::multi_index< "accounts"_n, account > accounts;
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;

         void sub_balance( const name& owner, const asset& value );
         void add_balance( const name& owner, const asset& value, const name& ram_payer );
   };

}
