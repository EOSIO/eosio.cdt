---
content_title: Key-Value Table
link_text: "Key-Value Table"
---

#### KV Table

A `datastore key value table on-chain`, or a `KV Table`, serves as a storage location which is organized as a table of rows where each row stores objects with the same definition.

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

The `object definition` consists of a list of `properties` and each property is stored on the corresponding row in the table. The properties of the objects are also referred to as `fields`.

A `KV Table` requires one unique index, of any type, that can be serialized to a binary representation.

A `KV Table` supports zero or more secondary indexes, of any type, that can be serialized to a binary representation. Indexes must be a data member or a function member.

Two types of indexes can be defined, unique or non-unique.

A unique index can be defined just for one property, and it will sort the objects stored in the `KV Table` based on the `specified property`. The unique index also ensures only one instance of an object is stored with a particular value for the `specified property`, and thus ensures the uniqueness of the property for which it is defined.

A non-unique index can be defined for one or multiple properties, and it will sort the objects stored in the `KV Table` based on the `specified property` or the combination of the `specified properties`. Very important though, a non-unique index requires as the last property of its definition a property which has unique values. Therefore although the non-unique index is intended for just one property, its definition will have two properties specified:

1. the first property, the one for which the non-unique index is built,
2. and the last property which must have unique values.

The main operations provided by the KV API are the following:

* Create an instance of `KV Table`
* Insert object in the `KV Table` instance
* Update object in the `KV Table` instance
* Delete object from the `KV Table` instance
* Search an object in the `KV Table` instance
* Verify if an object exists in the `KV Table` instance
* Retrieve a range of objects from the `KV Table` instance
* Iterate through the objects store in the `KV Table` instance

Consult the [KV Table Reference](../../../classeosio_1_1kv_1_1table/#class-eosiokvtable) and the `KV Table How-To`s listed below for details.

* [How To Use KV Table](./10_how-to-use-kv-table.md)
* [How To Create Indexes](./20_how-to-create-indexes-kv-table.md)
* [How To Upsert Into KV Table](./30_how-to-upsert-into-kv-table.md)
* [How To Delete From KV Table](./40_how-to-delete-from-kv-table.md)
* [How To Iterate](./50_how-to-iterate-kv-table.md)
* [How To Check A Record](./60_how-to-check-a-record-kv-table.md)
* [How To Find In KV Table](./70_how-to-find-in-kv-table.md)
* [How To Query Range In KV Table](./80_how-to-query-range-in-kv-table.md)
* [How To Allow Users To Pay](./90_how-to-allow-users-to-pay-kv-table.md)
