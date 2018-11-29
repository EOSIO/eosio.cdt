---
search:
    keywords: ['eosio::extended_symbol', 'extended_symbol', 'extended_symbol', 'get_symbol', 'get_contract', 'print', 'operator==', 'operator!=', 'operator<']
---

# class eosio::extended\_symbol

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**extended\_symbol**](classeosio_1_1extended__symbol.md)


## Public Functions

|Type|Name|
|-----|-----|
|constexpr|[**extended\_symbol**](classeosio_1_1extended__symbol_a56dc99d1a5b4a6edb9366d2b3f64b4f7.md#1a56dc99d1a5b4a6edb9366d2b3f64b4f7) () <br>Construct a new empty **[extended\_symbol](classeosio_1_1extended__symbol.md)** object. |
|constexpr|[**extended\_symbol**](classeosio_1_1extended__symbol_a53adc0870f45a4a6e70fd223f046f201.md#1a53adc0870f45a4a6e70fd223f046f201) (**[symbol](classeosio_1_1symbol.md)** sym, **[name](structeosio_1_1name.md)** con) <br>Construct a new **[symbol\_code](classeosio_1_1symbol__code.md)** object initialising symbol and contract with the passed in symbol and name. |
|constexpr **[symbol](classeosio_1_1symbol.md)**|[**get\_symbol**](classeosio_1_1extended__symbol_a81b0741a1705ad99855aa375fe004b9d.md#1a81b0741a1705ad99855aa375fe004b9d) () const |
|constexpr **[name](structeosio_1_1name.md)**|[**get\_contract**](classeosio_1_1extended__symbol_aa215d71db628ab457c8eff30912eae82.md#1aa215d71db628ab457c8eff30912eae82) () const |
|void|[**print**](classeosio_1_1extended__symbol_a2b1d5fafb307b27aab4274560a09866e.md#1a2b1d5fafb307b27aab4274560a09866e) (bool show\_precision = true) const <br>Print the extended symbol |


## Friends

|Type|Name|
|-----|-----|
|friend constexpr bool|[**operator==**](classeosio_1_1extended__symbol_ad306b3eaac29fba58441578b179398bc.md#1ad306b3eaac29fba58441578b179398bc)<br>Equivalency operator. |
|friend constexpr bool|[**operator!=**](classeosio_1_1extended__symbol_aafb631b64d0586016ca8ea499350820d.md#1aafb631b64d0586016ca8ea499350820d)<br>Inverted equivalency operator. |
|friend constexpr bool|[**operator<**](classeosio_1_1extended__symbol_afeaa211627f6df2a4f4f31aefedc6077.md#1afeaa211627f6df2a4f4f31aefedc6077)<br>Less than operator. |


## Public Functions Documentation

### function <a id="1a56dc99d1a5b4a6edb9366d2b3f64b4f7" href="#1a56dc99d1a5b4a6edb9366d2b3f64b4f7">extended\_symbol (1/2)</a>

```cpp
constexpr eosio::extended_symbol::extended_symbol ()
```

Construct a new empty **[extended\_symbol](classeosio_1_1extended__symbol.md)** object. 

Default constructor, construct a new **[extended\_symbol](classeosio_1_1extended__symbol.md)** 

### function <a id="1a53adc0870f45a4a6e70fd223f046f201" href="#1a53adc0870f45a4a6e70fd223f046f201">extended\_symbol (2/2)</a>

```cpp
constexpr eosio::extended_symbol::extended_symbol (
    symbol sym,
    name con
)
```

Construct a new **[symbol\_code](classeosio_1_1symbol__code.md)** object initialising symbol and contract with the passed in symbol and name. 

Construct a new **[symbol\_code](classeosio_1_1symbol__code.md)** given a symbol and a name.


**Parameters:**


* **sym** - The symbol 
* **con** - The name of the contract 



### function <a id="1a81b0741a1705ad99855aa375fe004b9d" href="#1a81b0741a1705ad99855aa375fe004b9d">get\_symbol</a>

```cpp
constexpr symbol eosio::extended_symbol::get_symbol () const
```


Returns the symbol in the extended\_contract


**Returns:**

symbol 




### function <a id="1aa215d71db628ab457c8eff30912eae82" href="#1aa215d71db628ab457c8eff30912eae82">get\_contract</a>

```cpp
constexpr name eosio::extended_symbol::get_contract () const
```


Returns the name of the contract in the **[extended\_symbol](classeosio_1_1extended__symbol.md)**


**Returns:**

name 




### function <a id="1a2b1d5fafb307b27aab4274560a09866e" href="#1a2b1d5fafb307b27aab4274560a09866e">print</a>

```cpp
void eosio::extended_symbol::print (
    bool show_precision = true
) const
```

Print the extended symbol 

Print the extended symbol 

## Friends Documentation

### friend <a id="1ad306b3eaac29fba58441578b179398bc" href="#1ad306b3eaac29fba58441578b179398bc">operator==</a>

```cpp
friend constexpr bool eosio::extended_symbol::operator== (
    const extended_symbol & a,
    const extended_symbol & b
)
```

Equivalency operator. 

Equivalency operator. Returns true if a == b (are the same)


**Returns:**

boolean - true if both provided extended\_symbols are the same 




### friend <a id="1aafb631b64d0586016ca8ea499350820d" href="#1aafb631b64d0586016ca8ea499350820d">operator!=</a>

```cpp
friend constexpr bool eosio::extended_symbol::operator!= (
    const extended_symbol & a,
    const extended_symbol & b
)
```

Inverted equivalency operator. 

Inverted equivalency operator. Returns true if a != b (are different)


**Returns:**

boolean - true if both provided extended\_symbols are not the same 




### friend <a id="1afeaa211627f6df2a4f4f31aefedc6077" href="#1afeaa211627f6df2a4f4f31aefedc6077">operator<</a>

```cpp
friend constexpr bool eosio::extended_symbol::operator< (
    const extended_symbol & a,
    const extended_symbol & b
)
```

Less than operator. 

Less than operator. Returns true if a < b. 

**Returns:**

boolean - true if **[extended\_symbol](classeosio_1_1extended__symbol.md)** `a` is less than `b` 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/symbol.hpp`
