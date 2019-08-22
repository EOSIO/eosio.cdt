## How to push a deferred transaction from a smart contract to another

Excerpt from: https://developers.eos.io/eosio-nodeos/docs/communication-model#section-deferred-communication

"Deferred communication conceptually takes the form of action notifications sent to a peer transaction. Deferred actions get scheduled to run, at best, at a later time, at the producer's discretion. There is no guarantee that a deferred action will be executed.

As already mentioned, deferred communication will get scheduled later at the producer's discretion. From the perspective of the originating transaction, i.e., the transaction that creates the deferred transaction, it can only determine whether the create request was submitted successfully or whether it failed (if it fails, it will fail immediately). Deferred transactions carry the authority of the contract that sends them. A transaction can cancel a deferred transaction."

TO DO: review the content below, is it ok to make this statement here?
Because of all considerations mentioned above we do not reccommend the use of deferred transactions and we are considering to deprecate them in the future versions.
