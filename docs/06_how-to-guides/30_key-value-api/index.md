---
content_title: Key-Value API
link_text: "Key-Value API"
---

## Overview

The Key-Value API provides a set of C++ classes and structures which facilitates the creation of datastore key value tables and maps on-chain. It is meant to provide the same functionality the multi-index provides in a `simpler` and more `flexible` API with comparable performance.

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

## Concept

The Key-Value API, or KV API, is a new option for developers to create datastore key value tables on-chain. KV API is more flexible than multi-index and allows developers to search the table in a more human-readable way, unlike multi-index tables where search is over 64-bit values.

On top of flexibility, this new API has a simpler interface and it helps the developer to avoid complex C++ templates constructs.

### Related Terminology and Concepts

The KV API provides two types of storage:

* [Key-Value Map](./kv_map/index.md), or shorter `KV Map`
* [Key-Value Table](./kv_table/index.md), or shorter `KV Table`

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.
