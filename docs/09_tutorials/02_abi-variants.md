## ABI variants

ABI text format supports a section at the same level as `types`, `structs`, etc. named `variants`.
__Example:__

```json
"variants": [
  {
    "name": "operation_amount",
    "types": [
      "extended_asset",
      "float64",
      "time_point_sec"
    ]
  },
  {
    "name": "operation_subaccount",
    "types": [
      "name",
      "extended_symbol",
    ]
  },
  {
    "name": "subop",
    "types": [
      "event",
      "balance_change",
      "set_value",
      "error"
    ]
  }
],
```

Once a table has a variant, it's safe to add more types to the variant over time. It works like a cpp union type, multiple types can be defined in a variant yet only one of those types data can be stored at a time. 

So this way, let's say I want to change the type of a column with variant `operation_subaccount` from `name` to `symbol_code` for my deployed table with the ABI text defined above, I can add the `symbol_code` type to the variant's `types` array and thus new rows inserted in the table can have data of type `symbol_code` for the column with variant `operation_subaccount`. 
The ABI text section defining the variants would look like this:

```json
"variants": [
  {
    "name": "operation_amount",
    "types": [
      "extended_asset",
      "float64",
      "time_point_sec"
    ]
  },
  {
    "name": "operation_subaccount",
    "types": [
      "name",
      "extended_symbol",
      "symbol_code"
    ]
  },
  {
    "name": "subop",
    "types": [
      "event",
      "balance_change",
      "set_value",
      "error"
    ]
  }
],
```

TO DO: Any restrictions, pitfalls, gotchas, things to be aware of?

__Note__: The implementation for this section can be found [here](https://github.com/EOSIO/eos/pull/5652).
