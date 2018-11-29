---
search:
    keywords: ['eosio::fixed_point64', 'val', 'fixed_point64', 'fixed_point64', 'fixed_point64', 'int_part', 'frac_part', 'print', 'operator=', 'operator=', 'operator+', 'operator-', 'operator*', 'operator/', 'operator==', 'operator>', 'operator<', 'operator+', 'operator-']
---

# struct eosio::fixed\_point64

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**fixed\_point64**](structeosio_1_1fixed__point64.md)


## Public Attributes

|Type|Name|
|-----|-----|
|int64\_t|[**val**](structeosio_1_1fixed__point64_adf1b4e4d25279c532d9ac3ff56d9af21.md#1adf1b4e4d25279c532d9ac3ff56d9af21)|


## Public Functions

|Type|Name|
|-----|-----|
||[**fixed\_point64**](structeosio_1_1fixed__point64_ab9f53ce12bd0a23708ec0b67ec7848cb.md#1ab9f53ce12bd0a23708ec0b67ec7848cb) (int64\_t v = 0) |
||[**fixed\_point64**](structeosio_1_1fixed__point64_a0fcb07196ce3b1ee19897d2edd1f362b.md#1a0fcb07196ce3b1ee19897d2edd1f362b) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
||[**fixed\_point64**](structeosio_1_1fixed__point64_a02a221576feae394dc006a6f65ba66c2.md#1a02a221576feae394dc006a6f65ba66c2) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
|int64\_t|[**int\_part**](structeosio_1_1fixed__point64_a713dad1088c1c4a95ed446e055f0a523.md#1a713dad1088c1c4a95ed446e055f0a523) () const |
|uint64\_t|[**frac\_part**](structeosio_1_1fixed__point64_a616163b00e9c4ff8e237139bb96ad6b1.md#1a616163b00e9c4ff8e237139bb96ad6b1) () const |
|void|[**print**](structeosio_1_1fixed__point64_a85bae51f6eb5071be1e0388e8f0c44af.md#1a85bae51f6eb5071be1e0388e8f0c44af) () const <br>Prints the fixed point value. |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)** &|[**operator=**](structeosio_1_1fixed__point64_a36ebd8f73c42839443039dbef6462ff4.md#1a36ebd8f73c42839443039dbef6462ff4) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)** &|[**operator=**](structeosio_1_1fixed__point64_a4f8cc600e51217711153481052e0002c.md#1a4f8cc600e51217711153481052e0002c) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)**<(Q >QR)?Q:QR >|[**operator+**](structeosio_1_1fixed__point64_a91f5e8b0aad593b8f92d8ed9b8f287d0.md#1a91f5e8b0aad593b8f92d8ed9b8f287d0) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) const |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)**<(Q >QR)?Q:QR >|[**operator-**](structeosio_1_1fixed__point64_a9b0a83b256f3a7c2dac7a2ba838d8397.md#1a9b0a83b256f3a7c2dac7a2ba838d8397) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) const |
|**[fixed\_point128](structeosio_1_1fixed__point128.md)**< Q+QR >|[**operator\***](structeosio_1_1fixed__point64_af88630db8a6e5f2f026abe4b6fc953dc.md#1af88630db8a6e5f2f026abe4b6fc953dc) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) const <br>Multiplication operator for **[fixed\_point64](structeosio_1_1fixed__point64.md)**. The result goes to **[fixed\_point64](structeosio_1_1fixed__point64.md)**. |
|**[fixed\_point128](structeosio_1_1fixed__point128.md)**< Q+64-QR >|[**operator/**](structeosio_1_1fixed__point64_a810e2397dd4645ec575430ffd54b5d3f.md#1a810e2397dd4645ec575430ffd54b5d3f) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) const |
|bool|[**operator==**](structeosio_1_1fixed__point64_ad2fb0230d48ccc90795e278e1ef12df6.md#1ad2fb0230d48ccc90795e278e1ef12df6) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
|bool|[**operator>**](structeosio_1_1fixed__point64_a63ee72acb0a3307d54283dba95dc623b.md#1a63ee72acb0a3307d54283dba95dc623b) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
|bool|[**operator<**](structeosio_1_1fixed__point64_a2c28bc89073dfa8c6f81ee2594a3272c.md#1a2c28bc89073dfa8c6f81ee2594a3272c) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)**<(Q >QR)?Q:QR >|[**operator+**](structeosio_1_1fixed__point64_a949ad7f0149535736353c9283797468b.md#1a949ad7f0149535736353c9283797468b) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & rhs) const <br>Addition between two **[fixed\_point64](structeosio_1_1fixed__point64.md)** variables and the result goes to **[fixed\_point64](structeosio_1_1fixed__point64.md)**. |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)**<(Q >QR)?Q:QR >|[**operator-**](structeosio_1_1fixed__point64_a49301a70e323c7b6fa7e8b4d4cd1cce2.md#1a49301a70e323c7b6fa7e8b4d4cd1cce2) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & rhs) const <br>Subtraction between two **[fixed\_point64](structeosio_1_1fixed__point64.md)** variables and the result goes to **[fixed\_point64](structeosio_1_1fixed__point64.md)**. |


## Detailed Description

64 bits representation of Fixed Point class.
Example: 
```cpp
fixed_point64<6> a(123232.455667233)
fixed_point64<0> a(123424)
fixed_point64<18> c = a*b;
fixed_point64<24> d = a+b+c;
fixed_point64<24> e = b/a;
```

 
## Public Attributes Documentation

### variable <a id="1adf1b4e4d25279c532d9ac3ff56d9af21" href="#1adf1b4e4d25279c532d9ac3ff56d9af21">val</a>

```cpp
int64_t eosio::fixed_point64< Q >::val;
```


Value of the fixed point represented as int64\_t 

## Public Functions Documentation

### function <a id="1ab9f53ce12bd0a23708ec0b67ec7848cb" href="#1ab9f53ce12bd0a23708ec0b67ec7848cb">fixed\_point64 (1/3)</a>

```cpp
eosio::fixed_point64::fixed_point64 (
    int64_t v = 0
)
```


Construct a new fixed point64 object from int64\_t


**Parameters:**


* **v** - int64\_t representation of the fixed point value 



### function <a id="1a0fcb07196ce3b1ee19897d2edd1f362b" href="#1a0fcb07196ce3b1ee19897d2edd1f362b">fixed\_point64 (2/3)</a>

```cpp
eosio::fixed_point64::fixed_point64 (
    const fixed_point64< QR > & r
)
```


Construct a new fixed point64 object from another **[fixed\_point64](structeosio_1_1fixed__point64.md)**


**Parameters:**


* **r** - Another **[fixed\_point64](structeosio_1_1fixed__point64.md)** as source 



### function <a id="1a02a221576feae394dc006a6f65ba66c2" href="#1a02a221576feae394dc006a6f65ba66c2">fixed\_point64 (3/3)</a>

```cpp
eosio::fixed_point64::fixed_point64 (
    const fixed_point32< QR > & r
)
```


Construct a new fixed point64 object from another **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Parameters:**


* **r** - **[fixed\_point64](structeosio_1_1fixed__point64.md)** as source 



### function <a id="1a713dad1088c1c4a95ed446e055f0a523" href="#1a713dad1088c1c4a95ed446e055f0a523">int\_part</a>

```cpp
int64_t eosio::fixed_point64::int_part () const
```


Get the integer part of the 64 bit fixed number


**Returns:**

Returns integer part of the fixed number


Example: 
```cpp
fixed_point64<18> a(1234.455667)
std::cout << a.int_part(); // Output: 1234
```

 

### function <a id="1a616163b00e9c4ff8e237139bb96ad6b1" href="#1a616163b00e9c4ff8e237139bb96ad6b1">frac\_part</a>

```cpp
uint64_t eosio::fixed_point64::frac_part () const
```


Get the decimal part of the 64 bit fixed number


**Returns:**

Returns decimal part of the fixed number


Example: 
```cpp
fixed64<3> a(1234.455667)
std::cout << a.decimal_part(); // Output: 455
```

 

### function <a id="1a85bae51f6eb5071be1e0388e8f0c44af" href="#1a85bae51f6eb5071be1e0388e8f0c44af">print</a>

```cpp
void eosio::fixed_point64::print () const
```

Prints the fixed point value. 

Prints the fixed point value 

### function <a id="1a36ebd8f73c42839443039dbef6462ff4" href="#1a36ebd8f73c42839443039dbef6462ff4">operator= (1/2)</a>

```cpp
fixed_point64 & eosio::fixed_point64::operator= (
    const fixed_point32< QR > & r
)
```


Assignment operator. Assign **[fixed\_point32](structeosio_1_1fixed__point32.md)** to **[fixed\_point64](structeosio_1_1fixed__point64.md)**


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

**[fixed\_point64](structeosio_1_1fixed__point64.md)**& - Reference to this object 




### function <a id="1a4f8cc600e51217711153481052e0002c" href="#1a4f8cc600e51217711153481052e0002c">operator= (2/2)</a>

```cpp
fixed_point64 & eosio::fixed_point64::operator= (
    const fixed_point64< QR > & r
)
```


Assignment operator. Assign **[fixed\_point64](structeosio_1_1fixed__point64.md)** to **[fixed\_point64](structeosio_1_1fixed__point64.md)**


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

**[fixed\_point64](structeosio_1_1fixed__point64.md)**& - Reference to this object 




### function <a id="1a91f5e8b0aad593b8f92d8ed9b8f287d0" href="#1a91f5e8b0aad593b8f92d8ed9b8f287d0">operator+ (1/2)</a>

```cpp
fixed_point64<(Q >QR)?Q:QR > eosio::fixed_point64::operator+ (
    const fixed_point64< QR > & r
) const
```


Addition operator


**Template parameters:**


* **QR** - Precision of the second addend 



**Parameters:**


* **r** - Second addend 



**Returns:**

- The result of addition 




### function <a id="1a9b0a83b256f3a7c2dac7a2ba838d8397" href="#1a9b0a83b256f3a7c2dac7a2ba838d8397">operator- (1/2)</a>

```cpp
fixed_point64<(Q >QR)?Q:QR > eosio::fixed_point64::operator- (
    const fixed_point64< QR > & r
) const
```


Subtraction operator


**Template parameters:**


* **QR** - Precision of the minuend 



**Parameters:**


* **r** - Minuend 



**Returns:**

- The result of subtraction 




### function <a id="1af88630db8a6e5f2f026abe4b6fc953dc" href="#1af88630db8a6e5f2f026abe4b6fc953dc">operator\*</a>

```cpp
fixed_point128< Q+QR > eosio::fixed_point64::operator* (
    const fixed_point64< QR > & r
) const
```

Multiplication operator for **[fixed\_point64](structeosio_1_1fixed__point64.md)**. The result goes to **[fixed\_point64](structeosio_1_1fixed__point64.md)**. 

Multiplication operator


**Template parameters:**


* **QR** - Precision of the multiplier 



**Parameters:**


* **r** - Multiplier 



**Returns:**

- The result of multiplication


Multiplication operator for **[fixed\_point64](structeosio_1_1fixed__point64.md)**. The result goes to **[fixed\_point128](structeosio_1_1fixed__point128.md)** 

**Note:**

Number of decimal on result will be sum of number of decimals of lhs and rhs


Example: 
```cpp
fixed_point128<33> result = fixed_point64<0>(131313) / fixed_point64<0>(2323)
```

 

### function <a id="1a810e2397dd4645ec575430ffd54b5d3f" href="#1a810e2397dd4645ec575430ffd54b5d3f">operator/</a>

```cpp
fixed_point128< Q+64-QR > eosio::fixed_point64::operator/ (
    const fixed_point64< QR > & r
) const
```


Division operator


**Template parameters:**


* **QR** - Precision of the divisor 



**Parameters:**


* **r** - Divisor 



**Returns:**

- The result of division


Division of two **[fixed\_point64](structeosio_1_1fixed__point64.md)** result will be stored in **[fixed\_point128](structeosio_1_1fixed__point128.md)**
Example: 
```cpp
fixed_point128<33> result = fixed_point64<0>(131313) / fixed_point64<0>(2323)
```

 

### function <a id="1ad2fb0230d48ccc90795e278e1ef12df6" href="#1ad2fb0230d48ccc90795e278e1ef12df6">operator==</a>

```cpp
bool eosio::fixed_point64::operator== (
    const fixed_point64< QR > & r
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




### function <a id="1a63ee72acb0a3307d54283dba95dc623b" href="#1a63ee72acb0a3307d54283dba95dc623b">operator></a>

```cpp
bool eosio::fixed_point64::operator> (
    const fixed_point64< QR > & r
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




### function <a id="1a2c28bc89073dfa8c6f81ee2594a3272c" href="#1a2c28bc89073dfa8c6f81ee2594a3272c">operator<</a>

```cpp
bool eosio::fixed_point64::operator< (
    const fixed_point64< QR > & r
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




### function <a id="1a949ad7f0149535736353c9283797468b" href="#1a949ad7f0149535736353c9283797468b">operator+ (2/2)</a>

```cpp
fixed_point64<(Q >QR)?Q:QR > eosio::fixed_point64::operator+ (
    const fixed_point64< QR > & rhs
) const
```

Addition between two **[fixed\_point64](structeosio_1_1fixed__point64.md)** variables and the result goes to **[fixed\_point64](structeosio_1_1fixed__point64.md)**. 

Addition between two **[fixed\_point64](structeosio_1_1fixed__point64.md)** variables Number of decimal on result will be max of decimals of lhs and rhs 

### function <a id="1a49301a70e323c7b6fa7e8b4d4cd1cce2" href="#1a49301a70e323c7b6fa7e8b4d4cd1cce2">operator- (2/2)</a>

```cpp
fixed_point64<(Q >QR)?Q:QR > eosio::fixed_point64::operator- (
    const fixed_point64< QR > & rhs
) const
```

Subtraction between two **[fixed\_point64](structeosio_1_1fixed__point64.md)** variables and the result goes to **[fixed\_point64](structeosio_1_1fixed__point64.md)**. 

Subtraction between two **[fixed\_point64](structeosio_1_1fixed__point64.md)** variables Number of decimal on result will be max of decimals of lhs and rhs 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/fixedpoint.hpp`
