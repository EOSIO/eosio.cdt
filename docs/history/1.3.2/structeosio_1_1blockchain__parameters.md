---
search:
    keywords: ['eosio::blockchain_parameters', 'max_block_net_usage', 'target_block_net_usage_pct', 'max_transaction_net_usage', 'base_per_transaction_net_usage', 'net_usage_leeway', 'context_free_discount_net_usage_num', 'context_free_discount_net_usage_den', 'max_block_cpu_usage', 'target_block_cpu_usage_pct', 'max_transaction_cpu_usage', 'min_transaction_cpu_usage', 'max_transaction_lifetime', 'deferred_trx_expiration_window', 'max_transaction_delay', 'max_inline_action_size', 'max_inline_action_depth', 'max_authority_depth']
---

# struct eosio::blockchain\_parameters

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**blockchain\_parameters**](structeosio_1_1blockchain__parameters.md)


## Public Attributes

|Type|Name|
|-----|-----|
|uint64\_t|[**max\_block\_net\_usage**](structeosio_1_1blockchain__parameters_aaffe7d6c7437826f208c68e9ee2acf65.md#1aaffe7d6c7437826f208c68e9ee2acf65)<br>the maxiumum net usage in instructions for a block |
|uint32\_t|[**target\_block\_net\_usage\_pct**](structeosio_1_1blockchain__parameters_a346f353931e0403d07e27da151a8af4d.md#1a346f353931e0403d07e27da151a8af4d)<br>The target percent (1% == 100, 100%= 10,000) of maximum net usage; exceeding this triggers congestion handling. |
|uint32\_t|[**max\_transaction\_net\_usage**](structeosio_1_1blockchain__parameters_a07d6c92013fce008071d9b610544c4c6.md#1a07d6c92013fce008071d9b610544c4c6)<br>The maximum objectively measured net usage that the chain will allow regardless of account limits. |
|uint32\_t|[**base\_per\_transaction\_net\_usage**](structeosio_1_1blockchain__parameters_a944e5020392feb3aa43452db31dd6c53.md#1a944e5020392feb3aa43452db31dd6c53)|
|uint32\_t|[**net\_usage\_leeway**](structeosio_1_1blockchain__parameters_a5066e04cd3a3d74459e810d253c29ebf.md#1a5066e04cd3a3d74459e810d253c29ebf)<br>The amount of net usage leeway available whilst executing a transaction (still checks against new limits without leeway at the end of the transaction) |
|uint32\_t|[**context\_free\_discount\_net\_usage\_num**](structeosio_1_1blockchain__parameters_a8ee23aba0c372b7d62b18e749e0e8798.md#1a8ee23aba0c372b7d62b18e749e0e8798)<br>The numerator for the discount on net usage of context-free data. |
|uint32\_t|[**context\_free\_discount\_net\_usage\_den**](structeosio_1_1blockchain__parameters_a34aa31b25c026f190097d3b66b5d8b28.md#1a34aa31b25c026f190097d3b66b5d8b28)<br>The denominator for the discount on net usage of context-free data. |
|uint32\_t|[**max\_block\_cpu\_usage**](structeosio_1_1blockchain__parameters_af6a0f851f934100f4cfc5d7cce8c3542.md#1af6a0f851f934100f4cfc5d7cce8c3542)<br>The maxiumum billable cpu usage (in microseconds) for a block. |
|uint32\_t|[**target\_block\_cpu\_usage\_pct**](structeosio_1_1blockchain__parameters_aa8e6bff1b7d255588fb31ccdf6035aba.md#1aa8e6bff1b7d255588fb31ccdf6035aba)<br>The target percent (1% == 100, 100%= 10,000) of maximum cpu usage; exceeding this triggers congestion handling. |
|uint32\_t|[**max\_transaction\_cpu\_usage**](structeosio_1_1blockchain__parameters_a59893f31083ea0239eff0fdb47a3e9a2.md#1a59893f31083ea0239eff0fdb47a3e9a2)<br>The maximum billable cpu usage (in microseconds) that the chain will allow regardless of account limits. |
|uint32\_t|[**min\_transaction\_cpu\_usage**](structeosio_1_1blockchain__parameters_a5c15c7c28e28d93183e85639ddd3dd83.md#1a5c15c7c28e28d93183e85639ddd3dd83)<br>The minimum billable cpu usage (in microseconds) that the chain requires. |
|uint32\_t|[**max\_transaction\_lifetime**](structeosio_1_1blockchain__parameters_ae9595da087ba46dbada5f785ec9191e6.md#1ae9595da087ba46dbada5f785ec9191e6)<br>Maximum lifetime of a transacton. |
|uint32\_t|[**deferred\_trx\_expiration\_window**](structeosio_1_1blockchain__parameters_a8cf445a327ffb230b9751f3b6764efaa.md#1a8cf445a327ffb230b9751f3b6764efaa)<br>the number of seconds after the time a deferred transaction can first execute until it expires |
|uint32\_t|[**max\_transaction\_delay**](structeosio_1_1blockchain__parameters_ac1ad602bce04bb7de2b83acf319605ac.md#1ac1ad602bce04bb7de2b83acf319605ac)<br>The maximum number of seconds that can be imposed as a delay requirement by authorization checks. |
|uint32\_t|[**max\_inline\_action\_size**](structeosio_1_1blockchain__parameters_a7dbcbc6c005be48b9d42fe21ee3c034f.md#1a7dbcbc6c005be48b9d42fe21ee3c034f)<br>Maximum size of inline action. |
|uint16\_t|[**max\_inline\_action\_depth**](structeosio_1_1blockchain__parameters_a2a6f29f85a765e80fab44ca4c8bf4104.md#1a2a6f29f85a765e80fab44ca4c8bf4104)<br>Maximum depth of inline action. |
|uint16\_t|[**max\_authority\_depth**](structeosio_1_1blockchain__parameters_aef65360380b879d1782d0b202b96c776.md#1aef65360380b879d1782d0b202b96c776)<br>Maximum authority depth. |


## Detailed Description

Tunable blockchain configuration that can be changed via consensus 
## Public Attributes Documentation

### variable <a id="1aaffe7d6c7437826f208c68e9ee2acf65" href="#1aaffe7d6c7437826f208c68e9ee2acf65">max\_block\_net\_usage</a>

```cpp
uint64_t eosio::blockchain_parameters::max_block_net_usage;
```

the maxiumum net usage in instructions for a block 

The maxiumum net usage in instructions for a block 

### variable <a id="1a346f353931e0403d07e27da151a8af4d" href="#1a346f353931e0403d07e27da151a8af4d">target\_block\_net\_usage\_pct</a>

```cpp
uint32_t eosio::blockchain_parameters::target_block_net_usage_pct;
```

The target percent (1% == 100, 100%= 10,000) of maximum net usage; exceeding this triggers congestion handling. 

The target percent (1% == 100, 100%= 10,000) of maximum net usage; exceeding this triggers congestion handling 

### variable <a id="1a07d6c92013fce008071d9b610544c4c6" href="#1a07d6c92013fce008071d9b610544c4c6">max\_transaction\_net\_usage</a>

```cpp
uint32_t eosio::blockchain_parameters::max_transaction_net_usage;
```

The maximum objectively measured net usage that the chain will allow regardless of account limits. 

The maximum objectively measured net usage that the chain will allow regardless of account limits 

### variable <a id="1a944e5020392feb3aa43452db31dd6c53" href="#1a944e5020392feb3aa43452db31dd6c53">base\_per\_transaction\_net\_usage</a>

```cpp
uint32_t eosio::blockchain_parameters::base_per_transaction_net_usage;
```


The base amount of net usage billed for a transaction to cover incidentals 

### variable <a id="1a5066e04cd3a3d74459e810d253c29ebf" href="#1a5066e04cd3a3d74459e810d253c29ebf">net\_usage\_leeway</a>

```cpp
uint32_t eosio::blockchain_parameters::net_usage_leeway;
```

The amount of net usage leeway available whilst executing a transaction (still checks against new limits without leeway at the end of the transaction) 

The amount of net usage leeway available whilst executing a transaction (still checks against new limits without leeway at the end of the transaction) 

### variable <a id="1a8ee23aba0c372b7d62b18e749e0e8798" href="#1a8ee23aba0c372b7d62b18e749e0e8798">context\_free\_discount\_net\_usage\_num</a>

```cpp
uint32_t eosio::blockchain_parameters::context_free_discount_net_usage_num;
```

The numerator for the discount on net usage of context-free data. 

The numerator for the discount on net usage of context-free data 

### variable <a id="1a34aa31b25c026f190097d3b66b5d8b28" href="#1a34aa31b25c026f190097d3b66b5d8b28">context\_free\_discount\_net\_usage\_den</a>

```cpp
uint32_t eosio::blockchain_parameters::context_free_discount_net_usage_den;
```

The denominator for the discount on net usage of context-free data. 

The denominator for the discount on net usage of context-free data 

### variable <a id="1af6a0f851f934100f4cfc5d7cce8c3542" href="#1af6a0f851f934100f4cfc5d7cce8c3542">max\_block\_cpu\_usage</a>

```cpp
uint32_t eosio::blockchain_parameters::max_block_cpu_usage;
```

The maxiumum billable cpu usage (in microseconds) for a block. 

The maxiumum billable cpu usage (in microseconds) for a block 

### variable <a id="1aa8e6bff1b7d255588fb31ccdf6035aba" href="#1aa8e6bff1b7d255588fb31ccdf6035aba">target\_block\_cpu\_usage\_pct</a>

```cpp
uint32_t eosio::blockchain_parameters::target_block_cpu_usage_pct;
```

The target percent (1% == 100, 100%= 10,000) of maximum cpu usage; exceeding this triggers congestion handling. 

The target percent (1% == 100, 100%= 10,000) of maximum cpu usage; exceeding this triggers congestion handling 

### variable <a id="1a59893f31083ea0239eff0fdb47a3e9a2" href="#1a59893f31083ea0239eff0fdb47a3e9a2">max\_transaction\_cpu\_usage</a>

```cpp
uint32_t eosio::blockchain_parameters::max_transaction_cpu_usage;
```

The maximum billable cpu usage (in microseconds) that the chain will allow regardless of account limits. 

The maximum billable cpu usage (in microseconds) that the chain will allow regardless of account limits 

### variable <a id="1a5c15c7c28e28d93183e85639ddd3dd83" href="#1a5c15c7c28e28d93183e85639ddd3dd83">min\_transaction\_cpu\_usage</a>

```cpp
uint32_t eosio::blockchain_parameters::min_transaction_cpu_usage;
```

The minimum billable cpu usage (in microseconds) that the chain requires. 

The minimum billable cpu usage (in microseconds) that the chain requires 

### variable <a id="1ae9595da087ba46dbada5f785ec9191e6" href="#1ae9595da087ba46dbada5f785ec9191e6">max\_transaction\_lifetime</a>

```cpp
uint32_t eosio::blockchain_parameters::max_transaction_lifetime;
```

Maximum lifetime of a transacton. 

Maximum lifetime of a transacton 

### variable <a id="1a8cf445a327ffb230b9751f3b6764efaa" href="#1a8cf445a327ffb230b9751f3b6764efaa">deferred\_trx\_expiration\_window</a>

```cpp
uint32_t eosio::blockchain_parameters::deferred_trx_expiration_window;
```

the number of seconds after the time a deferred transaction can first execute until it expires 

The number of seconds after the time a deferred transaction can first execute until it expires 

### variable <a id="1ac1ad602bce04bb7de2b83acf319605ac" href="#1ac1ad602bce04bb7de2b83acf319605ac">max\_transaction\_delay</a>

```cpp
uint32_t eosio::blockchain_parameters::max_transaction_delay;
```

The maximum number of seconds that can be imposed as a delay requirement by authorization checks. 

The maximum number of seconds that can be imposed as a delay requirement by authorization checks 

### variable <a id="1a7dbcbc6c005be48b9d42fe21ee3c034f" href="#1a7dbcbc6c005be48b9d42fe21ee3c034f">max\_inline\_action\_size</a>

```cpp
uint32_t eosio::blockchain_parameters::max_inline_action_size;
```

Maximum size of inline action. 

Maximum size of inline action 

### variable <a id="1a2a6f29f85a765e80fab44ca4c8bf4104" href="#1a2a6f29f85a765e80fab44ca4c8bf4104">max\_inline\_action\_depth</a>

```cpp
uint16_t eosio::blockchain_parameters::max_inline_action_depth;
```

Maximum depth of inline action. 

Maximum depth of inline action 

### variable <a id="1aef65360380b879d1782d0b202b96c776" href="#1aef65360380b879d1782d0b202b96c776">max\_authority\_depth</a>

```cpp
uint16_t eosio::blockchain_parameters::max_authority_depth;
```

Maximum authority depth. 

Maximum authority depth 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/privileged.hpp`
