---
search:
    keywords: ['eosio::fixed_point128', 'val', 'fixed_point128', 'fixed_point128', 'fixed_point128', 'fixed_point128', 'int_part', 'frac_part', 'print', 'operator=', 'operator=', 'operator=', 'operator==', 'operator>', 'operator<']
---

# struct eosio::fixed\_point128

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**fixed\_point128**](structeosio_1_1fixed__point128.md)


## Public Attributes

|Type|Name|
|-----|-----|
|int128\_t|[**val**](structeosio_1_1fixed__point128_aca06ae4275f148e87dcd527cbdaf87c5.md#1aca06ae4275f148e87dcd527cbdaf87c5)|


## Public Functions

|Type|Name|
|-----|-----|
||[**fixed\_point128**](structeosio_1_1fixed__point128_a35bb5e4ffc953d845551dadf79455ab8.md#1a35bb5e4ffc953d845551dadf79455ab8) (int128\_t v = 0) |
||[**fixed\_point128**](structeosio_1_1fixed__point128_a9a6e26a69471e1d42ca0f6b41b673734.md#1a9a6e26a69471e1d42ca0f6b41b673734) (const **[fixed\_point128](structeosio_1_1fixed__point128.md)**< qr > & r) |
||[**fixed\_point128**](structeosio_1_1fixed__point128_ac03c87c2b4a19daf60e5f9c56c4584f0.md#1ac03c87c2b4a19daf60e5f9c56c4584f0) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< qr > & r) |
||[**fixed\_point128**](structeosio_1_1fixed__point128_ab26dee4e941063197fb98f2d71d9c8ce.md#1ab26dee4e941063197fb98f2d71d9c8ce) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< qr > & r) <br>Construct a new fixed point128 object from another **[fixed\_point32](structeosio_1_1fixed__point32.md)**. |
|int128\_t|[**int\_part**](structeosio_1_1fixed__point128_a1790a1e9c8858987870440b7d0221702.md#1a1790a1e9c8858987870440b7d0221702) () const |
|uint128\_t|[**frac\_part**](structeosio_1_1fixed__point128_a039ff620c0ccbde2438615df5d6d3ab8.md#1a039ff620c0ccbde2438615df5d6d3ab8) () const |
|void|[**print**](structeosio_1_1fixed__point128_ae8064361384dfea9d6d7984719ef4b01.md#1ae8064361384dfea9d6d7984719ef4b01) () const |
|**[fixed\_point128](structeosio_1_1fixed__point128.md)** &|[**operator=**](structeosio_1_1fixed__point128_ad93deaa805f79fa8547f76039ef9c24d.md#1ad93deaa805f79fa8547f76039ef9c24d) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< qr > & r) |
|**[fixed\_point128](structeosio_1_1fixed__point128.md)** &|[**operator=**](structeosio_1_1fixed__point128_ac6221bff5864db61cce02e21d4fa9159.md#1ac6221bff5864db61cce02e21d4fa9159) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< qr > & r) |
|**[fixed\_point128](structeosio_1_1fixed__point128.md)** &|[**operator=**](structeosio_1_1fixed__point128_a30180ac339ef359d5d553c7e5eb1af83.md#1a30180ac339ef359d5d553c7e5eb1af83) (const **[fixed\_point128](structeosio_1_1fixed__point128.md)**< qr > & r) |
|bool|[**operator==**](structeosio_1_1fixed__point128_aec57d31fe7d5ef08eac57ac793aefa83.md#1aec57d31fe7d5ef08eac57ac793aefa83) (const **[fixed\_point128](structeosio_1_1fixed__point128.md)**< qr > & r) |
|bool|[**operator>**](structeosio_1_1fixed__point128_a062d76e3f824db70e6007dbe34f24874.md#1a062d76e3f824db70e6007dbe34f24874) (const **[fixed\_point128](structeosio_1_1fixed__point128.md)**< qr > & r) |
|bool|[**operator<**](structeosio_1_1fixed__point128_a0f5ea9645922ee31ea90cc3c9bbb58ae.md#1a0f5ea9645922ee31ea90cc3c9bbb58ae) (const **[fixed\_point128](structeosio_1_1fixed__point128.md)**< qr > & r) |


## Detailed Description

The template param Q represents the Q Factor i.e number of decimals
Example: 
```cpp
fixed_point128<6> a(123232.455667233)
fixed_point128<0> a(123424)
fixed_point128<18> c = a*b;
fixed_point128<24> d = a+b+c;
fixed_point128<24> e = b/a;
```

 
## Public Attributes Documentation

### variable <a id="1aca06ae4275f148e87dcd527cbdaf87c5" href="#1aca06ae4275f148e87dcd527cbdaf87c5">val</a>

```cpp
int128_t eosio::fixed_point128< Q >::val;
```


Value of the fixed point represented as int128\_t 

## Public Functions Documentation

### function <a id="1a35bb5e4ffc953d845551dadf79455ab8" href="#1a35bb5e4ffc953d845551dadf79455ab8">fixed\_point128 (1/4)</a>

```cpp
eosio::fixed_point128::fixed_point128 (
    int128_t v = 0
)
```


Various constructors for **[fixed\_point128](structeosio_1_1fixed__point128.md)**. Can create **[fixed\_point128](structeosio_1_1fixed__point128.md)** instance from an int128\_t, **[fixed\_point128](structeosio_1_1fixed__point128.md)**,64,32 instance
Example: 
```cpp
fixed_point64<18> a(1234.455667);
fixed_point128<3> b(a);
fixed_point32<6> b(13324.32323);
fixed_point128<5> c(a);
```

 Construct a new fixed point128 object from int128\_t


**Parameters:**


* **v** - int128\_t representation of the fixed point value 



### function <a id="1a9a6e26a69471e1d42ca0f6b41b673734" href="#1a9a6e26a69471e1d42ca0f6b41b673734">fixed\_point128 (2/4)</a>

```cpp
eosio::fixed_point128::fixed_point128 (
    const fixed_point128< qr > & r
)
```


Construct a new fixed point128 object from another **[fixed\_point128](structeosio_1_1fixed__point128.md)**


**Parameters:**


* **r** - Another **[fixed\_point128](structeosio_1_1fixed__point128.md)** as source 



### function <a id="1ac03c87c2b4a19daf60e5f9c56c4584f0" href="#1ac03c87c2b4a19daf60e5f9c56c4584f0">fixed\_point128 (3/4)</a>

```cpp
eosio::fixed_point128::fixed_point128 (
    const fixed_point64< qr > & r
)
```


Construct a new fixed point128 object from another **[fixed\_point64](structeosio_1_1fixed__point64.md)**


**Parameters:**


* **r** -fixed\_point64 as source 



### function <a id="1ab26dee4e941063197fb98f2d71d9c8ce" href="#1ab26dee4e941063197fb98f2d71d9c8ce">fixed\_point128 (4/4)</a>

```cpp
eosio::fixed_point128::fixed_point128 (
    const fixed_point32< qr > & r
)
```

Construct a new fixed point128 object from another **[fixed\_point32](structeosio_1_1fixed__point32.md)**. 

Construct a new fixed point128 object from another **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Parameters:**


* **r** -fixed\_point32 as source 



### function <a id="1a1790a1e9c8858987870440b7d0221702" href="#1a1790a1e9c8858987870440b7d0221702">int\_part</a>

```cpp
int128_t eosio::fixed_point128::int_part () const
```


Get the integer part of the 64 bit fixed number


**Returns:**

Returns integer part of the fixed number


Example: 
```cpp
fixed_point64<5> a(1234.455667)
std::cout << a.int_part(); // Output: 1234
```

 

### function <a id="1a039ff620c0ccbde2438615df5d6d3ab8" href="#1a039ff620c0ccbde2438615df5d6d3ab8">frac\_part</a>

```cpp
uint128_t eosio::fixed_point128::frac_part () const
```


Get the decimal part of the 64 bit fixed number


**Returns:**

Returns decimal part of the fixed number


Example: 
```cpp
fixed_point128<3> a(1234.455667)
std::cout << a.decimal_part(); // Output: 455
```

 

### function <a id="1ae8064361384dfea9d6d7984719ef4b01" href="#1ae8064361384dfea9d6d7984719ef4b01">print</a>

```cpp
void eosio::fixed_point128::print () const
```


Prints the fixed point value 

### function <a id="1ad93deaa805f79fa8547f76039ef9c24d" href="#1ad93deaa805f79fa8547f76039ef9c24d">operator= (1/3)</a>

```cpp
fixed_point128 & eosio::fixed_point128::operator= (
    const fixed_point32< qr > & r
)
```


Assignment operator. Assign **[fixed\_point32](structeosio_1_1fixed__point32.md)** to **[fixed\_point128](structeosio_1_1fixed__point128.md)**


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

**[fixed\_point128](structeosio_1_1fixed__point128.md)**& - Reference to this object 




### function <a id="1ac6221bff5864db61cce02e21d4fa9159" href="#1ac6221bff5864db61cce02e21d4fa9159">operator= (2/3)</a>

```cpp
fixed_point128 & eosio::fixed_point128::operator= (
    const fixed_point64< qr > & r
)
```


Assignment operator. Assign **[fixed\_point32](structeosio_1_1fixed__point32.md)** to **[fixed\_point64](structeosio_1_1fixed__point64.md)**


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

**[fixed\_point128](structeosio_1_1fixed__point128.md)**& - Reference to this object 




### function <a id="1a30180ac339ef359d5d553c7e5eb1af83" href="#1a30180ac339ef359d5d553c7e5eb1af83">operator= (3/3)</a>

```cpp
fixed_point128 & eosio::fixed_point128::operator= (
    const fixed_point128< qr > & r
)
```


Assignment operator. Assign **[fixed\_point32](structeosio_1_1fixed__point32.md)** to **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

**[fixed\_point128](structeosio_1_1fixed__point128.md)**& - Reference to this object 




### function <a id="1aec57d31fe7d5ef08eac57ac793aefa83" href="#1aec57d31fe7d5ef08eac57ac793aefa83">operator==</a>

```cpp
bool eosio::fixed_point128::operator== (
    const fixed_point128< qr > & r
)
```


Equality operator


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

true - if equal 




**Returns:**

false - otherwise 




### function <a id="1a062d76e3f824db70e6007dbe34f24874" href="#1a062d76e3f824db70e6007dbe34f24874">operator></a>

```cpp
bool eosio::fixed_point128::operator> (
    const fixed_point128< qr > & r
)
```


Greater than operator


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

true - if equal 




**Returns:**

false - otherwise 




### function <a id="1a0f5ea9645922ee31ea90cc3c9bbb58ae" href="#1a0f5ea9645922ee31ea90cc3c9bbb58ae">operator<</a>

```cpp
bool eosio::fixed_point128::operator< (
    const fixed_point128< qr > & r
)
```


Less than operator


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

true - if equal 




**Returns:**

false - otherwise 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/fixedpoint.hpp`
