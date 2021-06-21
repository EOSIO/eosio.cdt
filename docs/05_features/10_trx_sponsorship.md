---
content_title: Transaction Sponsorship
---

## Overview

The `Transaction Sponsorship` feature, also known as Resource Payer, allows a simple and secure way for application developers to sponsor, or decline to sponsor, transactions on behalf of their users.

## Concept

Before the `EOSIO 2.2` version, the CPU and NET resource costs for transactions on EOSIO-based blockchains were paid by the end users of the application. This fact made attracting and onboarding new users difficult.

The `Transaction Sponsorship` feature makes it easier for both smart contract and full stack developers on EOSIO-based blockchains to allow their users to transact without having to pay for CPU and NET resource costs in a simple and secure manner. This new feature makes the process of onboarding new users straightforward.

Blockchain application developers can use the `Transaction Sponsorship` on any EOSIO-based blockchain that enables it via the newly introduced `RESOURCE_PAYER` upgrade protocol feature.

### Related Terminology/Concepts

A [transaction](https://developers.eos.io/welcome/latest/glossary/index/#transaction) consists of one or more actions executed atomically on the blockchain which alter the state of the blockchain.

You need system resources, [CPU](https://developers.eos.io/welcome/latest/glossary/index/#cpu) and [NET](https://developers.eos.io/welcome/latest/glossary/index/#net), to be able to execute transactions.

The `Transaction Sponsorship` feature allows the application developers to designate the payer of the CPU and NET resources cost to a random blockchain [account](https://developers.eos.io/welcome/latest/glossary/index/#account).

The `Transaction Sponsorship` feature is not available by default. To enable the feature, the `RESOURCE_PAYER` upgrade protocol feature must be enabled. For more information about upgrading protocol features, read the [Consensus Protocol Upgrade Process](https://developers.eos.io/manuals/eos/latest/nodeos/upgrade-guides/1.8-upgrade-guide/#upgrade-process-for-all-eosio-networks-including-test-networks) documentation.

When you send the transaction to the blockchain, if you want to designate the transaction sponsor, you must specify in the transaction definition the sponsorship information which consists of the following:

* The sponsor’s account name,
* The maximum CPU limit amount the sponsor supports expressed in microseconds,
* The maximum NET limit amount the sponsor supports expressed in bytes.

The sponsorship information must be defined as a transaction extension and placed in the transaction’s extension list. For more details please see the examples provided in the following section.

## Examples

Below are two examples that illustrate how to send a sponsored transaction to the blockchain.

### EOSJS

To send a transaction to the blockchain using javascript via the `eosjs` library, follow the [How To Send A Transaction](https://developers.eos.io/manuals/eos/latest/cleos/how-to-guides/how-to-submit-a-transaction) steps and note in the json example the transaction sponsorship information set in the ``transaction_extensions`` field.

### Cleos

[[warning | Cleos Support Will Be Available Starting With v2.2 RC2 Onwards]]
| The ability to send transaction sponsorship information using `cleos` will be added in the next v2.2 RC2 release.
