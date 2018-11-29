---
search:
    keywords: ['eosio::name', 'raw', 'value', 'name', 'name', 'name', 'name', 'length', 'suffix', 'operator raw', 'operator bool', 'write_as_string', 'to_string', 'char_to_value', 'operator==', 'operator!=', 'operator<']
---

# struct eosio::name

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**name**](structeosio_1_1name.md)


**[EOSIO](struct_e_o_s_i_o.md)** Types. [More...](#detailed-description)
## Public Types

|Type|Name|
|-----|-----|
|enum|[**raw**](structeosio_1_1name_a1d269720fd7434429216833187173656.md#1a1d269720fd7434429216833187173656) {  } |


## Public Attributes

|Type|Name|
|-----|-----|
|uint64\_t|[**value**](structeosio_1_1name_adb309a6e132467ef2c53e953b56ebb52.md#1adb309a6e132467ef2c53e953b56ebb52)|


## Public Functions

|Type|Name|
|-----|-----|
|constexpr|[**name**](structeosio_1_1name_a3e05c2f8c5eb5fa35dc1a99918d57c19.md#1a3e05c2f8c5eb5fa35dc1a99918d57c19) () <br>Construct a new name object defaulting to a value of 0. |
|constexpr|[**name**](structeosio_1_1name_abc1f35add0a585ea60d6c7333b87f595.md#1abc1f35add0a585ea60d6c7333b87f595) (uint64\_t v) <br>Construct a new name object initialising value with v. |
|constexpr|[**name**](structeosio_1_1name_a4679582f6b187de123c5167db945b0fe.md#1a4679582f6b187de123c5167db945b0fe) (**[name::raw](structeosio_1_1name_a1d269720fd7434429216833187173656.md#1a1d269720fd7434429216833187173656)** r) <br>Construct a new name object initialising value with r. |
|constexpr|[**name**](structeosio_1_1name_a0ce912cd61e77f223ac1d252deec0d04.md#1a0ce912cd61e77f223ac1d252deec0d04) (std::string\_view str) <br>Construct a new name object initialising value with str. |
|constexpr uint8\_t|[**length**](structeosio_1_1name_a757935f0931b482c554686c2ea2a42b2.md#1a757935f0931b482c554686c2ea2a42b2) () const |
|constexpr **[name](structeosio_1_1name.md)**|[**suffix**](structeosio_1_1name_a806f77788fe466215adeb2b78553b6f8.md#1a806f77788fe466215adeb2b78553b6f8) () const |
|constexpr|[**operator raw**](structeosio_1_1name_afd3799cbd1bc276a81dba035599f535c.md#1afd3799cbd1bc276a81dba035599f535c) () const |
|constexpr|[**operator bool**](structeosio_1_1name_a667462189a277a9f11f66fcd88af05df.md#1a667462189a277a9f11f66fcd88af05df) () const |
|char \*|[**write\_as\_string**](structeosio_1_1name_adf844a1e7846f23bcf1d2a9ba282f0cb.md#1adf844a1e7846f23bcf1d2a9ba282f0cb) (char \* begin, char \* end) const |
|std::string|[**to\_string**](structeosio_1_1name_ae9b8c13d32a86f9ff355b6dce3955ab6.md#1ae9b8c13d32a86f9ff355b6dce3955ab6) () const <br>Returns the name value as a string by calling **[write\_as\_string()](structeosio_1_1name_adf844a1e7846f23bcf1d2a9ba282f0cb.md#1adf844a1e7846f23bcf1d2a9ba282f0cb)** and returning the buffer produced by **[write\_as\_string()](structeosio_1_1name_adf844a1e7846f23bcf1d2a9ba282f0cb.md#1adf844a1e7846f23bcf1d2a9ba282f0cb)** |


## Public Static Functions

|Type|Name|
|-----|-----|
|static constexpr uint8\_t|[**char\_to\_value**](structeosio_1_1name_a051cf40d16909e5c352cf85edb15fc9c.md#1a051cf40d16909e5c352cf85edb15fc9c) (char c) |


## Friends

|Type|Name|
|-----|-----|
|friend constexpr bool|[**operator==**](structeosio_1_1name_a7815a04748b0d6b1e28f1bb11e668595.md#1a7815a04748b0d6b1e28f1bb11e668595)|
|friend constexpr bool|[**operator!=**](structeosio_1_1name_af1df898dfc723d4873fe9356cc70eacb.md#1af1df898dfc723d4873fe9356cc70eacb)|
|friend constexpr bool|[**operator<**](structeosio_1_1name_a743497f458dd217e1eccf368d060bbd5.md#1a743497f458dd217e1eccf368d060bbd5)|


## Detailed Description

/\* Wraps a uint64\_t to ensure it is only passed to methods that expect a name. Ensures value is only passed to methods that expect a name and that no mathematical operations occur. Also enables specialization of print 
## Public Types Documentation

### enum <a id="1a1d269720fd7434429216833187173656" href="#1a1d269720fd7434429216833187173656">raw</a>

```cpp
enum eosio::name::raw {
};
```



## Public Attributes Documentation

### variable <a id="1adb309a6e132467ef2c53e953b56ebb52" href="#1adb309a6e132467ef2c53e953b56ebb52">value</a>

```cpp
uint64_t eosio::name::value;
```



## Public Functions Documentation

### function <a id="1a3e05c2f8c5eb5fa35dc1a99918d57c19" href="#1a3e05c2f8c5eb5fa35dc1a99918d57c19">name (1/4)</a>

```cpp
constexpr eosio::name::name ()
```

Construct a new name object defaulting to a value of 0. 

Construct a new name 

### function <a id="1abc1f35add0a585ea60d6c7333b87f595" href="#1abc1f35add0a585ea60d6c7333b87f595">name (2/4)</a>

```cpp
explicit constexpr eosio::name::name (
    uint64_t v
)
```

Construct a new name object initialising value with v. 

Construct a new name given a unit64\_t value


**Parameters:**


* **v** - The unit64\_t value 



### function <a id="1a4679582f6b187de123c5167db945b0fe" href="#1a4679582f6b187de123c5167db945b0fe">name (3/4)</a>

```cpp
explicit constexpr eosio::name::name (
    name::raw r
)
```

Construct a new name object initialising value with r. 

Construct a new name given a scoped enumerated type of raw (uint64\_t).


**Parameters:**


* **r** - The raw value which is a scoped enumerated type of unit64\_t 



### function <a id="1a0ce912cd61e77f223ac1d252deec0d04" href="#1a0ce912cd61e77f223ac1d252deec0d04">name (4/4)</a>

```cpp
explicit constexpr eosio::name::name (
    std::string_view str
)
```

Construct a new name object initialising value with str. 

Construct a new name given an string.


**Parameters:**


* **str** - The string value which validated then converted to unit64\_t 



### function <a id="1a757935f0931b482c554686c2ea2a42b2" href="#1a757935f0931b482c554686c2ea2a42b2">length</a>

```cpp
constexpr uint8_t eosio::name::length () const
```


Returns the length of the name 

### function <a id="1a806f77788fe466215adeb2b78553b6f8" href="#1a806f77788fe466215adeb2b78553b6f8">suffix</a>

```cpp
constexpr name eosio::name::suffix () const
```


Returns the suffix of the name 

### function <a id="1afd3799cbd1bc276a81dba035599f535c" href="#1afd3799cbd1bc276a81dba035599f535c">operator raw</a>

```cpp
constexpr eosio::name::operator raw () const
```


Casts a name to raw


**Returns:**

Returns an instance of raw based on the value of a name 




### function <a id="1a667462189a277a9f11f66fcd88af05df" href="#1a667462189a277a9f11f66fcd88af05df">operator bool</a>

```cpp
explicit constexpr eosio::name::operator bool () const
```


Explicit cast to bool of the uint64\_t value of the name


**Returns:**

Returns true if the name is set to the default value of 0 else true. 




### function <a id="1adf844a1e7846f23bcf1d2a9ba282f0cb" href="#1adf844a1e7846f23bcf1d2a9ba282f0cb">write\_as\_string</a>

```cpp
char * eosio::name::write_as_string (
    char * begin,
    char * end
) const
```


Writes the name as a string to the provided char buffer


**Precondition:**

Appropriate Size Precondition: (begin + 13) <= end and (begin + 13) does not overflow 




**Precondition:**

Valid Memory Region Precondition: The range [begin, end) must be a valid range of memory to write to. 




**Parameters:**


* **begin** - The start of the char buffer 
* **end** - Just past the end of the char buffer 



**Returns:**

char\* - Just past the end of the last character written (returns begin if the Appropriate Size Precondition is not satisfied) 




**Post**

If the Appropriate Size Precondition is satisfied, the range [begin, returned pointer) contains the string representation of the name. 




### function <a id="1ae9b8c13d32a86f9ff355b6dce3955ab6" href="#1ae9b8c13d32a86f9ff355b6dce3955ab6">to\_string</a>

```cpp
std::string eosio::name::to_string () const
```

Returns the name value as a string by calling **[write\_as\_string()](structeosio_1_1name_adf844a1e7846f23bcf1d2a9ba282f0cb.md#1adf844a1e7846f23bcf1d2a9ba282f0cb)** and returning the buffer produced by **[write\_as\_string()](structeosio_1_1name_adf844a1e7846f23bcf1d2a9ba282f0cb.md#1adf844a1e7846f23bcf1d2a9ba282f0cb)** 

Returns the name as a string. 

## Public Static Functions Documentation

### function <a id="1a051cf40d16909e5c352cf85edb15fc9c" href="#1a051cf40d16909e5c352cf85edb15fc9c">char\_to\_value</a>

```cpp
static constexpr uint8_t eosio::name::char_to_value (
    char c
)
```


Converts a name Base32 symbol into its corresponding value


**Parameters:**


* **c** - Character to be converted 



**Returns:**

constexpr char - Converted value 




## Friends Documentation

### friend <a id="1a7815a04748b0d6b1e28f1bb11e668595" href="#1a7815a04748b0d6b1e28f1bb11e668595">operator==</a>

```cpp
friend constexpr bool eosio::name::operator== (
    const name & a,
    const name & b
)
```


Equivalency operator. Returns true if a == b (are the same)


**Returns:**

boolean - true if both provided name values are the same 




### friend <a id="1af1df898dfc723d4873fe9356cc70eacb" href="#1af1df898dfc723d4873fe9356cc70eacb">operator!=</a>

```cpp
friend constexpr bool eosio::name::operator!= (
    const name & a,
    const name & b
)
```


Inverted equivalency operator. Returns true if a != b (are different)


**Returns:**

boolean - true if both provided name values are not the same 




### friend <a id="1a743497f458dd217e1eccf368d060bbd5" href="#1a743497f458dd217e1eccf368d060bbd5">operator<</a>

```cpp
friend constexpr bool eosio::name::operator< (
    const name & a,
    const name & b
)
```


Less than operator. Returns true if a < b.


**Returns:**

boolean - true if name `a` is less than `b` 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/name.hpp`
