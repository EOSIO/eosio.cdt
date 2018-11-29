---
search:
    keywords: ['eosio::multi_index::index', 'const_iterator', 'constants', 'secondary_extractor_type', 'secondary_key_type', 'const_reverse_iterator', 'multi_index', 'validate_index_name', 'name', 'number', 'extract_secondary_key', 'cbegin', 'begin', 'cend', 'end', 'crbegin', 'rbegin', 'crend', 'rend', 'find', 'find', 'require_find', 'require_find', 'get', 'get', 'lower_bound', 'lower_bound', 'upper_bound', 'upper_bound', 'iterator_to', 'modify', 'erase', 'get_code', 'get_scope']
---

# struct eosio::multi\_index::index

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**multi\_index**](classeosio_1_1multi__index.md) **::** [**index**](structeosio_1_1multi__index_1_1index.md)


## Classes

|Type|Name|
|-----|-----|
|struct|[**const\_iterator**](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)|


## Public Types

|Type|Name|
|-----|-----|
|enum|[**constants**](structeosio_1_1multi__index_1_1index_a014b97a5206ff7807cf7c62e772b8181.md#1a014b97a5206ff7807cf7c62e772b8181) { **table\_name** = static\_cast<uint64\_t>(TableName), **index\_name** = static\_cast<uint64\_t>(IndexName), **index\_number** = Number, **index\_table\_name** = (static\_cast<uint64\_t>(TableName) & 0xFFFFFFFFFFFFFFF0ULL)
                                    | (Number & 0x000000000000000FULL) } |
|typedef Extractor|[**secondary\_extractor\_type**](structeosio_1_1multi__index_1_1index_a2f3c3f4f02371ab09d995b4ed08772ce.md#1a2f3c3f4f02371ab09d995b4ed08772ce)|
|typedef std::decay< decltype(Extractor()(nullptr))>::type|[**secondary\_key\_type**](structeosio_1_1multi__index_1_1index_a49f2c5517ef496a53beda1c4fa8165b5.md#1a49f2c5517ef496a53beda1c4fa8165b5)|
|typedef std::reverse\_iterator< **[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)** >|[**const\_reverse\_iterator**](structeosio_1_1multi__index_1_1index_a710c24cb9fa944ba8e26d14b4d407b6b.md#1a710c24cb9fa944ba8e26d14b4d407b6b)<br>struct **[multi\_index::index::const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)** |


## Friends

|Type|Name|
|-----|-----|
|friend class|[**multi\_index**](structeosio_1_1multi__index_1_1index_a474ad2ee1333aa717a29bb55a9d5fc44.md#1a474ad2ee1333aa717a29bb55a9d5fc44)|


## Public Static Functions

|Type|Name|
|-----|-----|
|static constexpr bool|[**validate\_index\_name**](structeosio_1_1multi__index_1_1index_a1961eb9f6802b27c3938b205eba2b4e8.md#1a1961eb9f6802b27c3938b205eba2b4e8) (**[eosio::name](structeosio_1_1name.md)** n) |
|static constexpr uint64\_t|[**name**](structeosio_1_1multi__index_1_1index_a48e393c34c4c66a2fdeb768b9cd244d8.md#1a48e393c34c4c66a2fdeb768b9cd244d8) () |
|static constexpr uint64\_t|[**number**](structeosio_1_1multi__index_1_1index_a123d2367cc502c2e2bd9d234c0759733.md#1a123d2367cc502c2e2bd9d234c0759733) () |
|static auto|[**extract\_secondary\_key**](structeosio_1_1multi__index_1_1index_aa6c304af30c9cd2f5e401e3b8cd415bd.md#1aa6c304af30c9cd2f5e401e3b8cd415bd) (const T & obj) |


## Public Functions

|Type|Name|
|-----|-----|
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**cbegin**](structeosio_1_1multi__index_1_1index_aa34db18061849360328e05fa9eb105f4.md#1aa34db18061849360328e05fa9eb105f4) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**begin**](structeosio_1_1multi__index_1_1index_ae52505fea2dda6750bc8ee2ce523529b.md#1ae52505fea2dda6750bc8ee2ce523529b) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**cend**](structeosio_1_1multi__index_1_1index_aa62e31d8bec7e50750a0149725d90600.md#1aa62e31d8bec7e50750a0149725d90600) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**end**](structeosio_1_1multi__index_1_1index_a723113602490d02f1e5dfd336413e312.md#1a723113602490d02f1e5dfd336413e312) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**crbegin**](structeosio_1_1multi__index_1_1index_a5182e6d7629fb4277866f219cdeb3f63.md#1a5182e6d7629fb4277866f219cdeb3f63) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**rbegin**](structeosio_1_1multi__index_1_1index_af3283589ad1738f4a254ba3a7347e71e.md#1af3283589ad1738f4a254ba3a7347e71e) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**crend**](structeosio_1_1multi__index_1_1index_a772ee8db2ef5a9c447b393cd3b2b8621.md#1a772ee8db2ef5a9c447b393cd3b2b8621) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**rend**](structeosio_1_1multi__index_1_1index_aa9537b1dcbe7e123e84ec825eb4b4766.md#1aa9537b1dcbe7e123e84ec825eb4b4766) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**find**](structeosio_1_1multi__index_1_1index_a17e0b1a69195b5b2dd6c81ed8b87b7d5.md#1a17e0b1a69195b5b2dd6c81ed8b87b7d5) (secondary\_key\_type && secondary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**find**](structeosio_1_1multi__index_1_1index_aa6eb920454428f1bdbc3b35e8a1907ee.md#1aa6eb920454428f1bdbc3b35e8a1907ee) (const secondary\_key\_type & secondary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**require\_find**](structeosio_1_1multi__index_1_1index_a57327224628a5a875cd116568a9275e4.md#1a57327224628a5a875cd116568a9275e4) (secondary\_key\_type && secondary, const char \* error\_msg = "unable to **[find](classeosio_1_1multi__index_a40a65cdfcc298b85e0e4ddf4c3581c1c.md#1a40a65cdfcc298b85e0e4ddf4c3581c1c)** secondary key") const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**require\_find**](structeosio_1_1multi__index_1_1index_a2456a37fb65fa72f666adf74659f8177.md#1a2456a37fb65fa72f666adf74659f8177) (const secondary\_key\_type & secondary, const char \* error\_msg = "unable to **[find](classeosio_1_1multi__index_a40a65cdfcc298b85e0e4ddf4c3581c1c.md#1a40a65cdfcc298b85e0e4ddf4c3581c1c)** secondary key") const |
|const T &|[**get**](structeosio_1_1multi__index_1_1index_ac1b7fe4bd83241550b8ad605caa383cd.md#1ac1b7fe4bd83241550b8ad605caa383cd) (secondary\_key\_type && secondary, const char \* error\_msg = "unable to **[find](classeosio_1_1multi__index_a40a65cdfcc298b85e0e4ddf4c3581c1c.md#1a40a65cdfcc298b85e0e4ddf4c3581c1c)** secondary key") const |
|const T &|[**get**](structeosio_1_1multi__index_1_1index_a5800f8274ec86dc27f53e1a3708e0b6c.md#1a5800f8274ec86dc27f53e1a3708e0b6c) (const secondary\_key\_type & secondary, const char \* error\_msg = "unable to **[find](classeosio_1_1multi__index_a40a65cdfcc298b85e0e4ddf4c3581c1c.md#1a40a65cdfcc298b85e0e4ddf4c3581c1c)** secondary key") const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**lower\_bound**](structeosio_1_1multi__index_1_1index_ad074700e6763eed28e5b9b88b30fbad8.md#1ad074700e6763eed28e5b9b88b30fbad8) (secondary\_key\_type && secondary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**lower\_bound**](structeosio_1_1multi__index_1_1index_a21c01dac797e4f767fe8ab24d36c0d73.md#1a21c01dac797e4f767fe8ab24d36c0d73) (const secondary\_key\_type & secondary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**upper\_bound**](structeosio_1_1multi__index_1_1index_a19934ccb4ce8ac6b77b26b7ef8ad528b.md#1a19934ccb4ce8ac6b77b26b7ef8ad528b) (secondary\_key\_type && secondary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**upper\_bound**](structeosio_1_1multi__index_1_1index_ae0f8f9892cda39b37fa850e71377ca55.md#1ae0f8f9892cda39b37fa850e71377ca55) (const secondary\_key\_type & secondary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**iterator\_to**](structeosio_1_1multi__index_1_1index_a7b0ed8e2378ad5a13ecd2b3cf09a4b1f.md#1a7b0ed8e2378ad5a13ecd2b3cf09a4b1f) (const T & obj) |
|void|[**modify**](structeosio_1_1multi__index_1_1index_ae124e86de4d3cc8d34202007a235a29a.md#1ae124e86de4d3cc8d34202007a235a29a) (**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)** itr, **[eosio::name](structeosio_1_1name.md)** payer, Lambda && updater) |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**erase**](structeosio_1_1multi__index_1_1index_a7646329d161840fc5590f24186f61da0.md#1a7646329d161840fc5590f24186f61da0) (**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)** itr) |
|**[eosio::name](structeosio_1_1name.md)**|[**get\_code**](structeosio_1_1multi__index_1_1index_a0b704fc6937b41494c82de218190019a.md#1a0b704fc6937b41494c82de218190019a) () const |
|uint64\_t|[**get\_scope**](structeosio_1_1multi__index_1_1index_ac56321d4dcffcd76b850c788b3c6fbb6.md#1ac56321d4dcffcd76b850c788b3c6fbb6) () const |


## Public Types Documentation

### enum <a id="1a014b97a5206ff7807cf7c62e772b8181" href="#1a014b97a5206ff7807cf7c62e772b8181">constants</a>

```cpp
enum eosio::multi_index::index::constants {
    table_name = static_cast<uint64_t>(TableName),
    index_name = static_cast<uint64_t>(IndexName),
    index_number = Number,
    index_table_name = (static_cast<uint64_t>(TableName) & 0xFFFFFFFFFFFFFFF0ULL)
                                    | (Number & 0x000000000000000FULL),
};
```



### typedef <a id="1a2f3c3f4f02371ab09d995b4ed08772ce" href="#1a2f3c3f4f02371ab09d995b4ed08772ce">secondary\_extractor\_type</a>

```cpp
typedef Extractor eosio::multi_index< TableName, T, Indices >::index< IndexName, Extractor, Number, IsConst >::secondary_extractor_type;
```



### typedef <a id="1a49f2c5517ef496a53beda1c4fa8165b5" href="#1a49f2c5517ef496a53beda1c4fa8165b5">secondary\_key\_type</a>

```cpp
typedef std::decay<decltype( Extractor()(nullptr) )>::type eosio::multi_index< TableName, T, Indices >::index< IndexName, Extractor, Number, IsConst >::secondary_key_type;
```



### typedef <a id="1a710c24cb9fa944ba8e26d14b4d407b6b" href="#1a710c24cb9fa944ba8e26d14b4d407b6b">const\_reverse\_iterator</a>

```cpp
typedef std::reverse_iterator<const_iterator> eosio::multi_index< TableName, T, Indices >::index< IndexName, Extractor, Number, IsConst >::const_reverse_iterator;
```

struct **[multi\_index::index::const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)** 


## Friends Documentation

### friend <a id="1a474ad2ee1333aa717a29bb55a9d5fc44" href="#1a474ad2ee1333aa717a29bb55a9d5fc44">multi\_index</a>

```cpp
friend class multi_index;
```



## Public Static Functions Documentation

### function <a id="1a1961eb9f6802b27c3938b205eba2b4e8" href="#1a1961eb9f6802b27c3938b205eba2b4e8">validate\_index\_name</a>

```cpp
static constexpr bool eosio::multi_index::index::validate_index_name (
    eosio::name n
)
```



### function <a id="1a48e393c34c4c66a2fdeb768b9cd244d8" href="#1a48e393c34c4c66a2fdeb768b9cd244d8">name</a>

```cpp
static constexpr uint64_t eosio::multi_index::index::name ()
```



### function <a id="1a123d2367cc502c2e2bd9d234c0759733" href="#1a123d2367cc502c2e2bd9d234c0759733">number</a>

```cpp
static constexpr uint64_t eosio::multi_index::index::number ()
```



### function <a id="1aa6c304af30c9cd2f5e401e3b8cd415bd" href="#1aa6c304af30c9cd2f5e401e3b8cd415bd">extract\_secondary\_key</a>

```cpp
static auto eosio::multi_index::index::extract_secondary_key (
    const T & obj
)
```



## Public Functions Documentation

### function <a id="1aa34db18061849360328e05fa9eb105f4" href="#1aa34db18061849360328e05fa9eb105f4">cbegin</a>

```cpp
const_iterator eosio::multi_index::index::cbegin () const
```



### function <a id="1ae52505fea2dda6750bc8ee2ce523529b" href="#1ae52505fea2dda6750bc8ee2ce523529b">begin</a>

```cpp
const_iterator eosio::multi_index::index::begin () const
```



### function <a id="1aa62e31d8bec7e50750a0149725d90600" href="#1aa62e31d8bec7e50750a0149725d90600">cend</a>

```cpp
const_iterator eosio::multi_index::index::cend () const
```



### function <a id="1a723113602490d02f1e5dfd336413e312" href="#1a723113602490d02f1e5dfd336413e312">end</a>

```cpp
const_iterator eosio::multi_index::index::end () const
```



### function <a id="1a5182e6d7629fb4277866f219cdeb3f63" href="#1a5182e6d7629fb4277866f219cdeb3f63">crbegin</a>

```cpp
const_reverse_iterator eosio::multi_index::index::crbegin () const
```



### function <a id="1af3283589ad1738f4a254ba3a7347e71e" href="#1af3283589ad1738f4a254ba3a7347e71e">rbegin</a>

```cpp
const_reverse_iterator eosio::multi_index::index::rbegin () const
```



### function <a id="1a772ee8db2ef5a9c447b393cd3b2b8621" href="#1a772ee8db2ef5a9c447b393cd3b2b8621">crend</a>

```cpp
const_reverse_iterator eosio::multi_index::index::crend () const
```



### function <a id="1aa9537b1dcbe7e123e84ec825eb4b4766" href="#1aa9537b1dcbe7e123e84ec825eb4b4766">rend</a>

```cpp
const_reverse_iterator eosio::multi_index::index::rend () const
```



### function <a id="1a17e0b1a69195b5b2dd6c81ed8b87b7d5" href="#1a17e0b1a69195b5b2dd6c81ed8b87b7d5">find (1/2)</a>

```cpp
const_iterator eosio::multi_index::index::find (
    secondary_key_type && secondary
) const
```



### function <a id="1aa6eb920454428f1bdbc3b35e8a1907ee" href="#1aa6eb920454428f1bdbc3b35e8a1907ee">find (2/2)</a>

```cpp
const_iterator eosio::multi_index::index::find (
    const secondary_key_type & secondary
) const
```



### function <a id="1a57327224628a5a875cd116568a9275e4" href="#1a57327224628a5a875cd116568a9275e4">require\_find (1/2)</a>

```cpp
const_iterator eosio::multi_index::index::require_find (
    secondary_key_type && secondary,
    const char * error_msg = "unable to find secondary key"
) const
```



### function <a id="1a2456a37fb65fa72f666adf74659f8177" href="#1a2456a37fb65fa72f666adf74659f8177">require\_find (2/2)</a>

```cpp
const_iterator eosio::multi_index::index::require_find (
    const secondary_key_type & secondary,
    const char * error_msg = "unable to find secondary key"
) const
```



### function <a id="1ac1b7fe4bd83241550b8ad605caa383cd" href="#1ac1b7fe4bd83241550b8ad605caa383cd">get (1/2)</a>

```cpp
const T & eosio::multi_index::index::get (
    secondary_key_type && secondary,
    const char * error_msg = "unable to find secondary key"
) const
```



### function <a id="1a5800f8274ec86dc27f53e1a3708e0b6c" href="#1a5800f8274ec86dc27f53e1a3708e0b6c">get (2/2)</a>

```cpp
const T & eosio::multi_index::index::get (
    const secondary_key_type & secondary,
    const char * error_msg = "unable to find secondary key"
) const
```



### function <a id="1ad074700e6763eed28e5b9b88b30fbad8" href="#1ad074700e6763eed28e5b9b88b30fbad8">lower\_bound (1/2)</a>

```cpp
const_iterator eosio::multi_index::index::lower_bound (
    secondary_key_type && secondary
) const
```



### function <a id="1a21c01dac797e4f767fe8ab24d36c0d73" href="#1a21c01dac797e4f767fe8ab24d36c0d73">lower\_bound (2/2)</a>

```cpp
const_iterator eosio::multi_index::index::lower_bound (
    const secondary_key_type & secondary
) const
```



### function <a id="1a19934ccb4ce8ac6b77b26b7ef8ad528b" href="#1a19934ccb4ce8ac6b77b26b7ef8ad528b">upper\_bound (1/2)</a>

```cpp
const_iterator eosio::multi_index::index::upper_bound (
    secondary_key_type && secondary
) const
```



### function <a id="1ae0f8f9892cda39b37fa850e71377ca55" href="#1ae0f8f9892cda39b37fa850e71377ca55">upper\_bound (2/2)</a>

```cpp
const_iterator eosio::multi_index::index::upper_bound (
    const secondary_key_type & secondary
) const
```



### function <a id="1a7b0ed8e2378ad5a13ecd2b3cf09a4b1f" href="#1a7b0ed8e2378ad5a13ecd2b3cf09a4b1f">iterator\_to</a>

```cpp
const_iterator eosio::multi_index::index::iterator_to (
    const T & obj
)
```



### function <a id="1ae124e86de4d3cc8d34202007a235a29a" href="#1ae124e86de4d3cc8d34202007a235a29a">modify</a>

```cpp
void eosio::multi_index::index::modify (
    const_iterator itr,
    eosio::name payer,
    Lambda && updater
)
```



### function <a id="1a7646329d161840fc5590f24186f61da0" href="#1a7646329d161840fc5590f24186f61da0">erase</a>

```cpp
const_iterator eosio::multi_index::index::erase (
    const_iterator itr
)
```



### function <a id="1a0b704fc6937b41494c82de218190019a" href="#1a0b704fc6937b41494c82de218190019a">get\_code</a>

```cpp
eosio::name eosio::multi_index::index::get_code () const
```



### function <a id="1ac56321d4dcffcd76b850c788b3c6fbb6" href="#1ac56321d4dcffcd76b850c788b3c6fbb6">get\_scope</a>

```cpp
uint64_t eosio::multi_index::index::get_scope () const
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
