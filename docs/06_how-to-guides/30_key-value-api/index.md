---
content_title: Key-Value API
link_text: "Key-Value API"
---

## Overview

The Key-Value API provides a set of C++ classes and structures which facilitates the creation of datastore key value tables on-chain. It is meant to provide the same functionality the multi-index provides in a `simpler` and more `flexible` API with comparable performance.

## Concept

The Key-Value API, or KV API, is a new option for developers to create datastore key value tables on-chain. KV API is more flexible than multi-index and allows developers to search the table in a more human-readable way, unlike multi-index tables where search is over 64-bit values.

On top of flexibility, this new API has a simpler interface and it helps the developer to avoid complex C++ templates constructs.

### Related Terminology and Concepts

A `datastore key value table on-chain`, or a `KV Table`, serves as a storage location which is organized as a table of rows where each row stores objects with the same definition.

The `object definition` consists of a list of `properties` and each property is stored on the corresponding row in the table. The properties of the objects are also referred to as `fields`.

A `KV Table` requires one unique index, of any type, that can be serialized to a binary representation.

A `KV Table` supports zero or more secondary indexes, of any type, that can be serialized to a binary representation. Indexes must be a data member or a function member.

Two types of indexes can be defined, unique or non-unique.

A unique index can be defined just for one property, and it will sort the objects stored in the `KV Table` based on the `specified property`. The unique index also ensures only one instance of an object is stored with a particular value for the `specified property`, and thus ensures the uniqueness of the property for which it is defined.

A non-unique index can be defined for one or multiple properties, and it will sort the objects stored in the `KV Table` based on the `specified property` or the combination of the `specified properties`. Very important though, a non-unique index requires as the first property of its definition a property which has unique values. Therefore although the non-unique index is intended for just one property, its definition will have two properties specified:

1. the first property which must have unique values
2. and the second property, the one for which the non-unique index is built

The main operations provided by the KV API are the following:

* Create an instance of `KV Table`
* Insert object in the `KV Table` instance
* Update object in the `KV Table` instance
* Delete object from the `KV Table` instance
* Search an object in the `KV Table` instance
* Verify if an object exists in the `KV Table` instance
* Retrieve a range of objects from the `KV Table` instance
* Iterate through the objects store in the `KV Table` instance

Consult the [KV API Reference](../../group__keyvalue) the `KV API How-To`s listed below for details.

* [How To Use KV Table](./10_how-to-use-kv-table.md)
* [How To Create Indexes](./20_how-to-create-indexes.md)
* [How To Upsert Into Table](./30_how-to-upsert-into-table.md)
* [How To Delete From Table](./40_how-to-delete-from-table.md)
* [How To Iterate](./50_how-to-iterate.md)
* [How To Check A Record](./60_how-to-check-a-record.md)
* [How To Find In Table](./70_how-to-find-in-table.md)
* [How To Query Range In Table](./80_how-to-query-range-in-table.md)
* [How To Allow Users To Pay](./90_how-to-allow-users-to-pay.md)
