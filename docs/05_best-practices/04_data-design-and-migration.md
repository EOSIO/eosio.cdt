---
content_title: Data design and migration
---

EOSIO based blockchains allow developers to easily update their smart contract code. However, a few things need to be considered when it comes to data updates and/or migration. The multi-index table API is one of the mechanisms (Key-Value API being the other) for storing and updating blockchain state. The multi-index table API creates and uses data structures in RAM. Once created and deployed on the blockchain there are limitations if you want to update these structures. Below you will find a few approaches to your smart contract data design, updates to this design, and for data migration.

# How to modify a data structure defined using multi-index table API

Modifying a deployed multi-index table structure may be done by selecting one of the different strategies outlined below:

## 1. If you don't mind losing the existing data

If you don't mind losing the data from the initial table you can follow these two steps:

1. Erase all records from first table
2. Deploy a new contract with modified table structure

## 2. If you want to keep the existing data

If you want to keep the existing data there are two ways to do it:

### 2.1. Using binary extensions

To learn how to modify the structure using binary extensions read this [tutorial](../09_tutorials/01_binary-extension.md).

### 2.2. Using ABI variants

To learn how to modify the structure using ABI variants read this [tutorial](../09_tutorials/02_abi-variants.md).

### 2.3. Migrate the existing data to a second table

#### 2.3.1. Migration without downtime, but slower

1. Create the new version of your multi-index table alongside the old one.
2. Transfer data from the old table to the new one. You may do so as part of your normal access pattern, first checking the new table to see if the entry you seek is present and if not, check the original table, and if it's present, migrate it while adding the data for the new field, then remove it from the original table to save RAM costs.
3. You must retain both versions of your multi-index table until you have completed this migration, at which point you may update your contract to remove the original version of your multi-index table.

#### 2.3.2. Migration with downtime, but faster

If you prefer less code complexity and can accept downtime for your application:

1. Deploy a version of your contract solely for migration purposes, and run migration transactions on every row of your table until complete. If the first table is big, e.g. has a large number of rows, the transaction time limit could be reached while running the  migration transactions. To mitigate this implement the migrate function to move a limited number of rows each time it runs.
2. Deploy a new contract using only the new version of the table, at which point, your migration and downtime is complete.

[[caution]]
| Both of the above migration methods require some pre-planning (like the ability to put your contract into a maintenance mode for user feedback)
