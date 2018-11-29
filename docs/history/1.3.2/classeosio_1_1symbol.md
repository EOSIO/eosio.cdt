---
search:
    keywords: ['eosio::symbol', 'symbol', 'symbol', 'symbol', 'symbol', 'is_valid', 'precision', 'code', 'raw', 'operator bool', 'print', 'operator==', 'operator!=', 'operator<']
---

# class eosio::symbol

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**symbol**](classeosio_1_1symbol.md)


## Public Functions

|Type|Name|
|-----|-----|
|constexpr|[**symbol**](classeosio_1_1symbol_a6604372bb0cc9d3c70a3b77973517e41.md#1a6604372bb0cc9d3c70a3b77973517e41) () <br>Construct a new symbol object defaulting to a value of 0. |
|constexpr|[**symbol**](classeosio_1_1symbol_abcbc25794d1a3e0477be40441be6b210.md#1abcbc25794d1a3e0477be40441be6b210) (uint64\_t raw) <br>Construct a new symbol object initialising value with raw. |
|constexpr|[**symbol**](classeosio_1_1symbol_a990e99879ecbebb194e5de43932ec59b.md#1a990e99879ecbebb194e5de43932ec59b) (**[symbol\_code](classeosio_1_1symbol__code.md)** sc, uint8\_t precision) <br>Construct a new symbol object initialising value with a symbol maximum of 7 characters, packs the symbol and precision into the uint64\_t value. |
|constexpr|[**symbol**](classeosio_1_1symbol_afc5e4be9944b51deac6484128deb2ce2.md#1afc5e4be9944b51deac6484128deb2ce2) (std::string\_view ss, uint8\_t precision) <br>Construct a new symbol object initialising value with a symbol maximum of 7 characters, packs the symbol and precision into the uint64\_t value. |
|constexpr bool|[**is\_valid**](classeosio_1_1symbol_af52f86a82390abe6be09e9745df30815.md#1af52f86a82390abe6be09e9745df30815) () const |
|constexpr uint8\_t|[**precision**](classeosio_1_1symbol_af2153bf9cfc5bd79c3f1787f8028646a.md#1af2153bf9cfc5bd79c3f1787f8028646a) () const |
|constexpr **[symbol\_code](classeosio_1_1symbol__code.md)**|[**code**](classeosio_1_1symbol_a559c818f42e9d0df1a4c4823d7c6ecd8.md#1a559c818f42e9d0df1a4c4823d7c6ecd8) () const |
|constexpr uint64\_t|[**raw**](classeosio_1_1symbol_a9cb96662a0965ac03e1bf5acfbbda011.md#1a9cb96662a0965ac03e1bf5acfbbda011) () const |
|constexpr|[**operator bool**](classeosio_1_1symbol_a6c974f93f06131859376b75c4df03a53.md#1a6c974f93f06131859376b75c4df03a53) () const |
|void|[**print**](classeosio_1_1symbol_a9f8042d110a6fff15ea1813987c0fbe6.md#1a9f8042d110a6fff15ea1813987c0fbe6) (bool show\_precision = true) const <br>Print the symbol |


## Friends

|Type|Name|
|-----|-----|
|friend constexpr bool|[**operator==**](classeosio_1_1symbol_aec9fbc4e4aff91276e8c1f71f113c494.md#1aec9fbc4e4aff91276e8c1f71f113c494)<br>Equivalency operator. |
|friend constexpr bool|[**operator!=**](classeosio_1_1symbol_a62adc71177b7a104162f7618cb0bac44.md#1a62adc71177b7a104162f7618cb0bac44)<br>Inverted equivalency operator. |
|friend constexpr bool|[**operator<**](classeosio_1_1symbol_a5ffd3dd1dfad342b30aed2c53993c548.md#1a5ffd3dd1dfad342b30aed2c53993c548)<br>Less than operator. |


## Public Functions Documentation

### function <a id="1a6604372bb0cc9d3c70a3b77973517e41" href="#1a6604372bb0cc9d3c70a3b77973517e41">symbol (1/4)</a>

```cpp
constexpr eosio::symbol::symbol ()
```

Construct a new symbol object defaulting to a value of 0. 

Default constructor, construct a new symbol 

### function <a id="1abcbc25794d1a3e0477be40441be6b210" href="#1abcbc25794d1a3e0477be40441be6b210">symbol (2/4)</a>

```cpp
explicit constexpr eosio::symbol::symbol (
    uint64_t raw
)
```

Construct a new symbol object initialising value with raw. 

Construct a new symbol given a scoped enumerated type of raw (uint64\_t).


**Parameters:**


* **raw** - The raw value which is a scoped enumerated type of unit64\_t 



### function <a id="1a990e99879ecbebb194e5de43932ec59b" href="#1a990e99879ecbebb194e5de43932ec59b">symbol (3/4)</a>

```cpp
constexpr eosio::symbol::symbol (
    symbol_code sc,
    uint8_t precision
)
```

Construct a new symbol object initialising value with a symbol maximum of 7 characters, packs the symbol and precision into the uint64\_t value. 

Construct a new symbol given a **[symbol\_code](classeosio_1_1symbol__code.md)** and a uint8\_t precision.


**Parameters:**


* **sc** - The **[symbol\_code](classeosio_1_1symbol__code.md)** 
* **precision** - The number of decimal places used for the symbol 



### function <a id="1afc5e4be9944b51deac6484128deb2ce2" href="#1afc5e4be9944b51deac6484128deb2ce2">symbol (4/4)</a>

```cpp
constexpr eosio::symbol::symbol (
    std::string_view ss,
    uint8_t precision
)
```

Construct a new symbol object initialising value with a symbol maximum of 7 characters, packs the symbol and precision into the uint64\_t value. 

Construct a new symbol given a string and a uint8\_t precision.


**Parameters:**


* **ss** - The string containing the symbol 
* **precision** - The number of decimal places used for the symbol 



### function <a id="1af52f86a82390abe6be09e9745df30815" href="#1af52f86a82390abe6be09e9745df30815">is\_valid</a>

```cpp
constexpr bool eosio::symbol::is_valid () const
```


Is this symbol valid 

### function <a id="1af2153bf9cfc5bd79c3f1787f8028646a" href="#1af2153bf9cfc5bd79c3f1787f8028646a">precision</a>

```cpp
constexpr uint8_t eosio::symbol::precision () const
```


This symbol's precision 

### function <a id="1a559c818f42e9d0df1a4c4823d7c6ecd8" href="#1a559c818f42e9d0df1a4c4823d7c6ecd8">code</a>

```cpp
constexpr symbol_code eosio::symbol::code () const
```


Returns representation of symbol name 

### function <a id="1a9cb96662a0965ac03e1bf5acfbbda011" href="#1a9cb96662a0965ac03e1bf5acfbbda011">raw</a>

```cpp
constexpr uint64_t eosio::symbol::raw () const
```


Returns uint64\_t repreresentation of the symbol 

### function <a id="1a6c974f93f06131859376b75c4df03a53" href="#1a6c974f93f06131859376b75c4df03a53">operator bool</a>

```cpp
explicit constexpr eosio::symbol::operator bool () const
```



### function <a id="1a9f8042d110a6fff15ea1813987c0fbe6" href="#1a9f8042d110a6fff15ea1813987c0fbe6">print</a>

```cpp
void eosio::symbol::print (
    bool show_precision = true
) const
```

Print the symbol 

Print the symbol 

## Friends Documentation

### friend <a id="1aec9fbc4e4aff91276e8c1f71f113c494" href="#1aec9fbc4e4aff91276e8c1f71f113c494">operator==</a>

```cpp
friend constexpr bool eosio::symbol::operator== (
    const symbol & a,
    const symbol & b
)
```

Equivalency operator. 

Equivalency operator. Returns true if a == b (are the same)


**Returns:**

boolean - true if both provided symbols are the same 




### friend <a id="1a62adc71177b7a104162f7618cb0bac44" href="#1a62adc71177b7a104162f7618cb0bac44">operator!=</a>

```cpp
friend constexpr bool eosio::symbol::operator!= (
    const symbol & a,
    const symbol & b
)
```

Inverted equivalency operator. 

Inverted equivalency operator. Returns true if a != b (are different)


**Returns:**

boolean - true if both provided symbols are not the same 




### friend <a id="1a5ffd3dd1dfad342b30aed2c53993c548" href="#1a5ffd3dd1dfad342b30aed2c53993c548">operator<</a>

```cpp
friend constexpr bool eosio::symbol::operator< (
    const symbol & a,
    const symbol & b
)
```

Less than operator. 

Less than operator. Returns true if a < b. 

**Returns:**

boolean - true if symbol `a` is less than `b` 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/symbol.hpp`
