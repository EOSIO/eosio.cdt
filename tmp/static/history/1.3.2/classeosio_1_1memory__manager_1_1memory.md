---
search:
    keywords: ['eosio::memory_manager::memory', 'buffer_ptr', 'memory', 'init', 'is_init', 'is_in_heap', 'is_capacity_remaining', 'malloc', 'malloc_from_freed', 'realloc_in_place', 'free', 'cleanup_remaining', 'expand_memory']
---

# class eosio::memory\_manager::memory

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**memory\_manager**](classeosio_1_1memory__manager.md) **::** [**memory**](classeosio_1_1memory__manager_1_1memory.md)


## Classes

|Type|Name|
|-----|-----|
|class|[**buffer\_ptr**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr.md)|


## Public Functions

|Type|Name|
|-----|-----|
||[**memory**](classeosio_1_1memory__manager_1_1memory_af85ade61b24019eea6306cf648f1d625.md#1af85ade61b24019eea6306cf648f1d625) () |
|void|[**init**](classeosio_1_1memory__manager_1_1memory_a09c3fb534b185d02d44c44a65687303d.md#1a09c3fb534b185d02d44c44a65687303d) (char \*const mem\_heap, uint32\_t size) |
|uint32\_t|[**is\_init**](classeosio_1_1memory__manager_1_1memory_accfe081869782f56b8b21d1335368c3e.md#1accfe081869782f56b8b21d1335368c3e) () const |
|uint32\_t|[**is\_in\_heap**](classeosio_1_1memory__manager_1_1memory_a93951eea84e88bf8855dbc8eb96af62d.md#1a93951eea84e88bf8855dbc8eb96af62d) (const char \*const ptr) const |
|uint32\_t|[**is\_capacity\_remaining**](classeosio_1_1memory__manager_1_1memory_a53fc9d9c352df6bc90b44f559bed968b.md#1a53fc9d9c352df6bc90b44f559bed968b) () const |
|char \*|[**malloc**](classeosio_1_1memory__manager_1_1memory_a2e3a3b2c09d28da026c8ed93811b9d6e.md#1a2e3a3b2c09d28da026c8ed93811b9d6e) (uint32\_t size) |
|char \*|[**malloc\_from\_freed**](classeosio_1_1memory__manager_1_1memory_abf4eba30b71bdd9a15478ee22688f581.md#1abf4eba30b71bdd9a15478ee22688f581) (uint32\_t size) |
|char \*|[**realloc\_in\_place**](classeosio_1_1memory__manager_1_1memory_a32ce5b71a89a4388e84748dfc03df3f1.md#1a32ce5b71a89a4388e84748dfc03df3f1) (char \*const ptr, uint32\_t size, uint32\_t \* orig\_ptr\_size) |
|void|[**free**](classeosio_1_1memory__manager_1_1memory_a53ae2045de1772dae532bb03da755eee.md#1a53ae2045de1772dae532bb03da755eee) (char \* ptr) |
|void|[**cleanup\_remaining**](classeosio_1_1memory__manager_1_1memory_af9adcac3f8b1fbea2fb929290d92e7d3.md#1af9adcac3f8b1fbea2fb929290d92e7d3) () |
|bool|[**expand\_memory**](classeosio_1_1memory__manager_1_1memory_aa2c5013fac6521de3cf024201696341c.md#1aa2c5013fac6521de3cf024201696341c) (char \* exp\_mem, uint32\_t size) |


## Public Functions Documentation

### function <a id="1af85ade61b24019eea6306cf648f1d625" href="#1af85ade61b24019eea6306cf648f1d625">memory</a>

```cpp
eosio::memory_manager::memory::memory ()
```



### function <a id="1a09c3fb534b185d02d44c44a65687303d" href="#1a09c3fb534b185d02d44c44a65687303d">init</a>

```cpp
void eosio::memory_manager::memory::init (
    char *const mem_heap,
    uint32_t size
)
```



### function <a id="1accfe081869782f56b8b21d1335368c3e" href="#1accfe081869782f56b8b21d1335368c3e">is\_init</a>

```cpp
uint32_t eosio::memory_manager::memory::is_init () const
```



### function <a id="1a93951eea84e88bf8855dbc8eb96af62d" href="#1a93951eea84e88bf8855dbc8eb96af62d">is\_in\_heap</a>

```cpp
uint32_t eosio::memory_manager::memory::is_in_heap (
    const char *const ptr
) const
```



### function <a id="1a53fc9d9c352df6bc90b44f559bed968b" href="#1a53fc9d9c352df6bc90b44f559bed968b">is\_capacity\_remaining</a>

```cpp
uint32_t eosio::memory_manager::memory::is_capacity_remaining () const
```



### function <a id="1a2e3a3b2c09d28da026c8ed93811b9d6e" href="#1a2e3a3b2c09d28da026c8ed93811b9d6e">malloc</a>

```cpp
char * eosio::memory_manager::memory::malloc (
    uint32_t size
)
```



### function <a id="1abf4eba30b71bdd9a15478ee22688f581" href="#1abf4eba30b71bdd9a15478ee22688f581">malloc\_from\_freed</a>

```cpp
char * eosio::memory_manager::memory::malloc_from_freed (
    uint32_t size
)
```



### function <a id="1a32ce5b71a89a4388e84748dfc03df3f1" href="#1a32ce5b71a89a4388e84748dfc03df3f1">realloc\_in\_place</a>

```cpp
char * eosio::memory_manager::memory::realloc_in_place (
    char *const ptr,
    uint32_t size,
    uint32_t * orig_ptr_size
)
```



### function <a id="1a53ae2045de1772dae532bb03da755eee" href="#1a53ae2045de1772dae532bb03da755eee">free</a>

```cpp
void eosio::memory_manager::memory::free (
    char * ptr
)
```



### function <a id="1af9adcac3f8b1fbea2fb929290d92e7d3" href="#1af9adcac3f8b1fbea2fb929290d92e7d3">cleanup\_remaining</a>

```cpp
void eosio::memory_manager::memory::cleanup_remaining ()
```



### function <a id="1aa2c5013fac6521de3cf024201696341c" href="#1aa2c5013fac6521de3cf024201696341c">expand\_memory</a>

```cpp
bool eosio::memory_manager::memory::expand_memory (
    char * exp_mem,
    uint32_t size
)
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/eosiolib.cpp`
