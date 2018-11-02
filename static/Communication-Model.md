An EOSIO Smart Contract consists of a set of action and type definitions. Action definitions specify and implement the behaviors of the contract. The type definitions specify the required content and structures. EOSIO actions operate primarily in a message-based communication architecture. A client invokes actions by sending (pushing) messages to `nodeos`. This can be done using the `cleos` command. It can also be done using one of the EOSIO `send` methods (e.g., `eosio::action::send`). `nodeos` dispatches action requests to the WASM code that implements a contract. That code runs in its entirety, then processing continues to the next action.

EOSIO Smart Contracts can communicate with each other, e.g., to have another contract perform some operation pertinent to the completion of the current transaction, or to trigger a future transaction outside of the scope of the current transaction.

EOSIO supports two basic communication models, *inline* and *deferred*. An operation to perform within the current transaction is an example of an inline action, while a triggered future transaction is an example of a deferred action.

Communication among contracts should be considered as occurring asynchronously. The asynchronous communication model can result in spam, which the resource limiting algorithm will resolve.

## Inline Communication
Inline communication takes the form of requesting other actions that need to be executed as part of the calling action. Inline actions operate with the same scopes and authorities of the original transaction, and are guaranteed to execute with the current transaction. These can effectively be thought of as nested transactions within the calling transaction. If any part of the transaction fails, the inline actions will unwind with the rest of the transaction. Calling the inline action generates no notification outside the scope of the transaction, regardless of success or failure.

## Deferred Communication
Deferred communication conceptually takes the form of action notifications sent to a peer transaction. Deferred actions get scheduled to run, at best, at a later time, at the producer's discretion. There is no guarantee that a deferred action will be executed.

As already mentioned, deferred communication will get scheduled later at the producer's discretion. From the perspective of the originating transaction, i.e., the transaction that creates the deferred transaction, it can only determine whether the create request was submitted successfully or whether it failed (if it fails, it will fail immediately). Deferred transactions carry the authority of the contract that sends them. A transaction can cancel a deferred transaction.
#Transactions VS. Actions
An action represents a single operation, whereas a transaction is a collection of one or more actions. A contract and an account communicate in the form of actions. Actions can be sent individually, or in combined form if they are intended to be executed as a whole. 

*Transaction with one action*.

```base
{
  "expiration": "2018-04-01T15:20:44",
  "region": 0,
  "ref_block_num": 42580,
  "ref_block_prefix": 3987474256,
  "net_usage_words": 21,
  "kcpu_usage": 1000,
  "delay_sec": 0,
  "context_free_actions": [],
  "actions": [{
      "account": "eosio.token",
      "name": "issue",
      "authorization": [{
          "actor": "eosio",
          "permission": "active"
        }
      ],
      "data": "00000000007015d640420f000000000004454f5300000000046d656d6f"
    }
  ],
  "signatures": [
    ""
  ],
  "context_free_data": []
}
```

*Transaction with multiple actions*, these actions must all succeed or the transaction will fail.
```base
{
  "expiration": "...",
  "region": 0,
  "ref_block_num": ...,
  "ref_block_prefix": ...,
  "net_usage_words": ..,
  "kcpu_usage": ..,
  "delay_sec": 0,
  "context_free_actions": [],
  "actions": [{
      "account": "...",
      "name": "...",
      "authorization": [{
          "actor": "...",
          "permission": "..."
        }
      ],
      "data": "..."
    }, {
      "account": "...",
      "name": "...",
      "authorization": [{
          "actor": "...",
          "permission": "..."
        }
      ],
      "data": "..."
    }
  ],
  "signatures": [
    ""
  ],
  "context_free_data": []
}
```

# Action Name Restrictions
Action types are actually **base32 encoded 64-bit integers**. This means they are limited to the characters a-z, 1-5, and '.' for the first 12 characters. If there is a 13th character then it is restricted to the first 16 characters ('.' and a-p).

For more detailed information, please see [link here]()

# Transaction Confirmations
On completion of the transaction, a transaction receipt is generated. This receipt takes the form a hash. Receiving a transaction hash does not mean that the transaction has been confirmed, it only means that the node accepted it without error, which also means that there is a high probability other producers will accept it.

By means of confirmation, you should see the transaction in the transaction history with the block number of which it is included.

# Action Handlers and Action "Apply" Context"
Smart contracts provide action handlers to do the work of requested actions. (More on this below) Each time an action runs, i.e., the action is "applied" by running the `apply` method in the contract implementation, EOSIO creates a new action "apply" context within which the action runs. The diagram below illustrates key elements of the action "apply" context.


![](https://files.readme.io/6d71afc-action-apply-context-diagram.png)

From a global view of an EOSIO blockchain, every node in the EOSIO network gets a copy of and runs every action in every contract. Some of the nodes are doing the actual work of the contract, while others are processing in order to prove the validity of the transaction blocks. It is, therefore, important that contracts be able to determine "who they are", or basically, under which context are they running. Context identification information is provided in the action context, as illustrated in the above diagram by `receiver`, `code`, `action`. `receiver` is the account that is currently processing the action. `code` is the account that authorized the contract. `action` is the ID of the currently running action.

As discussed above, actions operate within transactions; if a transaction fails, the results of all actions in the transaction must be rolled back. A key part of the action context is the Current Transaction Data. This contains a transaction header, an ordered vector of all of the original actions in the transaction, a vector of the context free actions in the transaction, a prunable set of context free data (provided as a vector of blobs) defined by the code that implements the contract, and a full index to the vector of blobs.

Before processing an action, EOSIO sets up a clean working memory for the action. This is where the working variables for the action are held. An action's working memory is available only to that action, even for actions in the same transaction. Variables that might have been set when another action executed are not available within another action's context. The only way to pass state among actions is to persist it to and retrieve it from the EOSIO database. See Persistence API for details on how to use the EOSIO persistence services.

An action can have many side effects. Among these are:
- Change state persisted in the EOSIO persistent storage
- Notify the recipient of the current transaction
- Send inline action requests to a new receiver
- Generate new (deferred) transactions
- Cancel existing (in-flight) deferred transactions (i.e., cancel already-submitted deferred transaction requests)
- 
#Transaction Limitations
Every transaction must execute in 30ms or less. If a transaction contains several actions, and the sum of these actions is greater than 30ms, the entire transaction will fail. In situations without concurrency requirements for their actions this can be circumvented by including the CPU consuming actions in separate transactions.