## ABI variants

ABI Text format supports a section at the same level as types, structs, etc. named variants. Example:

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

The implementation for this section can be found [here](https://github.com/EOSIO/eos/pull/5652).
