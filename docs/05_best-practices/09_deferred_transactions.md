---
content_title: Deferred transactions
---

Deferred communication conceptually takes the form of action notifications sent to a peer transaction. Deferred actions get scheduled to run, at best, at a later time, at the producer's discretion. There is no guarantee that a deferred action will be executed.

As already mentioned, deferred communication will get scheduled later at the producer's discretion. From the perspective of the originating transaction, i.e., the transaction that creates the deferred transaction, it can only determine whether the create request was submitted successfully or whether it failed (if it fails, it will fail immediately). Deferred transactions carry the authority of the contract that sends them. A transaction can cancel a deferred transaction.

[[warning | Deferred Transactions Are Deprecated]]
| As of [EOSIO 2.0 RC1](https://github.com/EOSIO/eos/releases/tag/v2.0.0-rc1) deferred transactions are deprecated.

Due to the above described behaviors it is not recommended to use `deferred transactions`.
