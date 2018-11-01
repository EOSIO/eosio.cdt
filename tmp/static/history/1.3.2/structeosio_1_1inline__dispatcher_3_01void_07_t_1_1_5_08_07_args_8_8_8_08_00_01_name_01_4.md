---
search:
    keywords: ['eosio::inline_dispatcher< void(T::*)(Args...), Name >', 'call', 'call']
---

# struct eosio::inline\_dispatcher< void(T::\*)(Args...), Name >

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**inline\_dispatcher< void(T::\*)(Args...), Name >**](structeosio_1_1inline__dispatcher_3_01void_07_t_1_1_5_08_07_args_8_8_8_08_00_01_name_01_4.md)


## Public Static Functions

|Type|Name|
|-----|-----|
|static void|[**call**](structeosio_1_1inline__dispatcher_3_01void_07_t_1_1_5_08_07_args_8_8_8_08_00_01_name_01_4_a8ccabfd180270a8128685eeaea33c75a.md#1a8ccabfd180270a8128685eeaea33c75a) (**[name](structeosio_1_1name.md)** code, const **[permission\_level](structeosio_1_1permission__level.md)** & perm, std::tuple< Args... > args) |
|static void|[**call**](structeosio_1_1inline__dispatcher_3_01void_07_t_1_1_5_08_07_args_8_8_8_08_00_01_name_01_4_a43a9a1084e226ca5e3b8f306174a18ef.md#1a43a9a1084e226ca5e3b8f306174a18ef) (**[name](structeosio_1_1name.md)** code, std::vector< **[permission\_level](structeosio_1_1permission__level.md)** > perms, std::tuple< Args... > args) |


## Public Static Functions Documentation

### function <a id="1a8ccabfd180270a8128685eeaea33c75a" href="#1a8ccabfd180270a8128685eeaea33c75a">call (1/2)</a>

```cpp
static void eosio::inline_dispatcher< void(T::*)(Args...), Name >::call (
    name code,
    const permission_level & perm,
    std::tuple< Args... > args
)
```



### function <a id="1a43a9a1084e226ca5e3b8f306174a18ef" href="#1a43a9a1084e226ca5e3b8f306174a18ef">call (2/2)</a>

```cpp
static void eosio::inline_dispatcher< void(T::*)(Args...), Name >::call (
    name code,
    std::vector< permission_level > perms,
    std::tuple< Args... > args
)
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/action.hpp`
