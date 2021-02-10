---
content_title: Deferred transactions
---

Deferred communication conceptually takes the form of action notifications sent to a peer transaction. Deferred actions get scheduled to run, at best, at a later time, at the producer's discretion. There is no guarantee that a deferred action will be executed.

As already mentioned, deferred communication will get scheduled later at the producer's discretion. From the perspective of the originating transaction, i.e., the transaction that creates the deferred transaction, it can only determine whether the create request was submitted successfully or whether it failed (if it fails, it will fail immediately). Deferred transactions carry the authority of the contract that sends them. A transaction can cancel a deferred transaction.

[[warning | Deferred Transactions Are Deprecated]]
| As of [EOSIO 2.0 RC1](https://github.com/EOSIO/eos/releases/tag/v2.0.0-rc1) deferred transactions are deprecated.

Due to the above described behaviors it is not recommended to use `deferred transactions`.

[[warning | Duplicate deferred transaction IDs]]
| In earlier versions, prior to `v1.8.0`, it is possible to observe rare deferred transaction ID collisions because technically the protocol's validation rules allow any number of duplicate deferred transaction IDs. However, the block producing code prior to `v1.8.0` limited this to exactly one duplicate ID in the same block followed by at-most one duplicate ID in the following block. This behavior was mitigated starting with version `v1.8.0` making it functionally impossible to be achieved at the block producing code level. On top of that, the same version `v1.8.0` introduced the optional `NO_DUPLICATE_DEFERRED_ID` protocol feature which, if deployed, it makes this behavior impossible at the protocol layer as well.
