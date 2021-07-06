---
content_title: Resource Payer
---

## Overview

The *Resource Payer* feature, also known as *Transaction Sponsorship*, allows to specify the resource payer for a transaction. As a direct consequence the application developers have a simple and secure way to sponsor, or decline to sponsor, transactions on behalf of their users.

## Concept

Before the `EOSIO 2.2` version, the CPU and NET resource costs for transactions on EOSIO-based blockchains were paid by the end users of the application. This fact made attracting and onboarding new users difficult.

The *Resource Payer* feature makes it easier for both smart contract and full stack developers on EOSIO-based blockchains to allow their users to transact without having to pay for CPU and NET resource costs in a simple and secure manner. This new feature makes the process of onboarding new users straightforward.

Blockchain application developers can use the *Resource Payer* on any EOSIO-based blockchain that enables it via the newly introduced `RESOURCE_PAYER` upgrade protocol feature.

### Related Concepts

A [transaction](https://developers.eos.io/welcome/latest/glossary/index/#transaction) consists of one or more actions executed atomically on the blockchain which alter the state of the blockchain.

You need system resources, [CPU](https://developers.eos.io/welcome/latest/glossary/index/#cpu) and [NET](https://developers.eos.io/welcome/latest/glossary/index/#net), to be able to execute transactions.

The *Resource Payer* feature allows the application developers to designate the payer of the CPU and NET resources cost to a random blockchain [account](https://developers.eos.io/welcome/latest/glossary/index/#account).

The *Resource Payer* feature is not available by default. To enable the feature, the `RESOURCE_PAYER` upgrade protocol feature must be enabled. For more information about upgrading protocol features, read the [Consensus Protocol Upgrade Process](https://developers.eos.io/manuals/eos/latest/nodeos/upgrade-guides/1.8-upgrade-guide/#upgrade-process-for-all-eosio-networks-including-test-networks) documentation.

When you send the transaction to the blockchain, if you want to designate the resource payer, you must specify in the transaction definition the payer information which consists of the following:

* The payer account name
* The maximum CPU limit amount the payer supports expressed in microseconds
* The maximum NET limit amount the payer supports expressed in bytes

The resource payer information must be defined as a transaction extension and placed in the transaction’s extension list. For more details please see the examples provided in the following section.

## Examples

Find below one example that illustrate how to send transaction to the blockchain which contain resource payer information.

### EOSJS

To send a transaction to the blockchain using javascript via the `eosjs` library, follow the [How To Send A Transaction](https://developers.eos.io/manuals/eos/latest/cleos/how-to-guides/how-to-submit-a-transaction) steps and note in the json example the resource payer information set in the ``transaction_extensions`` field.

### Cleos

[[info | Cleos Support Will Be Available Starting With v2.2 RC2 Onwards]]
| The ability to send transaction with resource payer information using `cleos` will be added in the v2.2 RC2 release version.
