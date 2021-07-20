#pragma once

#include <eosio/eosio.hpp>
#include <eosio/ignore.hpp>
#include <eosio/transaction.hpp>

namespace eosio {
   /**
    * The `eosio.wrap` system contract allows block producers to bypass authorization checks or run privileged actions with 15/21 producer approval and thus simplifies block producers superuser actions. It also makes these actions easier to audit.
    *
    * It does not give block producers any additional powers or privileges that do not already exist within the EOSIO based blockchains. As it is implemented, in an EOSIO based blockchain, 15/21 block producers can change an account's permissions or modify an account's contract code if they decided it is beneficial for the blockchain and community. However, the current method is opaque and leaves undesirable side effects on specific system accounts, and thus the `eosio.wrap `contract solves this matter by providing an easier method of executing important governance actions.
    *
    * The only action implemented by the `eosio.wrap` system contract is the `exec` action. This action allows for execution of a transaction, which is passed to the `exec` method in the form of a packed transaction in json format via the 'trx' parameter and the `executer` account that executes the transaction. The same `executer` account will also be used to pay the RAM and CPU fees needed to execute the transaction.
    */
   class [[eosio::contract("eosio.wrap")]] wrap : public contract {
      public:
         using contract::contract;

         /**
          * Execute action.
          *
          * Execute a transaction while bypassing regular authorization checks.
          * 
          * Preconditions:
          * - Requires authorization of eosio.wrap which needs to be a privileged account.
          *
          * Postconditions:
          * - Deferred transaction RAM usage is billed to 'executer'          *
          * 
          * @param executer - account executing the transaction,
          * @param trx - the transaction to be executed.
          */
         [[eosio::action]]
         void exec( ignore<name> executer, ignore<transaction> trx );

         using exec_action = eosio::action_wrapper<"exec"_n, &wrap::exec>;
   };
} /// namespace eosio
