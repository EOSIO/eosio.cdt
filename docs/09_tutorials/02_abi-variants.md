## ABI variants

ABI text format supports a section at the same level as types, structs, etc. named variants.
Example:

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

Once a table has a variant, it's safe to add more types to the variant over time. It works like a cpp union type, multiple types can be defined in a variant yet only one of those type data can be stored at a time. So this way, if at a later time I want to change the type of a column for my deployed table, I can add that type to the variant `types` array and thus new rows inserted in the table could have this new type added.

TO DO: Any restrictions, pitfalls, gotchas, things to be aware of?

__Note__: The implementation for this section can be found [here](https://github.com/EOSIO/eos/pull/5652).
