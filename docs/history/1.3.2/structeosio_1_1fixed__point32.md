---
search:
    keywords: ['eosio::fixed_point32', 'val', 'fixed_point32', 'fixed_point32', 'fixed_point32', 'int_part', 'frac_part', 'print', 'operator=', 'operator=', 'operator+', 'operator-', 'operator*', 'operator/', 'operator==', 'operator>', 'operator<', 'operator=', 'operator=', 'operator+', 'operator-']
---

# struct eosio::fixed\_point32

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**fixed\_point32**](structeosio_1_1fixed__point32.md)


32 bits representation of Fixed Point class. [More...](#detailed-description)
## Public Attributes

|Type|Name|
|-----|-----|
|int32\_t|[**val**](structeosio_1_1fixed__point32_adc0fb8c31ec9e07e6e3a4a7e04fe2471.md#1adc0fb8c31ec9e07e6e3a4a7e04fe2471)|


## Public Functions

|Type|Name|
|-----|-----|
||[**fixed\_point32**](structeosio_1_1fixed__point32_a81c6f339a55c8327935ef972ebdb278d.md#1a81c6f339a55c8327935ef972ebdb278d) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
||[**fixed\_point32**](structeosio_1_1fixed__point32_a7ad1f82c49b206575fccf8b736815d7c.md#1a7ad1f82c49b206575fccf8b736815d7c) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
||[**fixed\_point32**](structeosio_1_1fixed__point32_af7b7646354bc934510228ca1bd53e6b1.md#1af7b7646354bc934510228ca1bd53e6b1) (int32\_t param = 0) |
|int32\_t|[**int\_part**](structeosio_1_1fixed__point32_ad877eb1cdb682c9c1412204a304c1300.md#1ad877eb1cdb682c9c1412204a304c1300) () const |
|uint32\_t|[**frac\_part**](structeosio_1_1fixed__point32_a41ff73528b152e09796cde085fac502d.md#1a41ff73528b152e09796cde085fac502d) () const |
|void|[**print**](structeosio_1_1fixed__point32_a464c1dfdc7c689f18fadcd1de6de1359.md#1a464c1dfdc7c689f18fadcd1de6de1359) () const |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)** &|[**operator=**](structeosio_1_1fixed__point32_a58d7f252b05e4d5af1376741f8f6f80b.md#1a58d7f252b05e4d5af1376741f8f6f80b) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)** &|[**operator=**](structeosio_1_1fixed__point32_a3746c182de95876fcbf5784b620d4aed.md#1a3746c182de95876fcbf5784b620d4aed) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)**<(Q >QR)?Q:QR >|[**operator+**](structeosio_1_1fixed__point32_a031984309854b00133383caa360dfaef.md#1a031984309854b00133383caa360dfaef) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) const |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)**<(Q >QR)?Q:QR >|[**operator-**](structeosio_1_1fixed__point32_a8b853041cdc62f50f21067198ea5d20c.md#1a8b853041cdc62f50f21067198ea5d20c) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) const |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)**< Q+QR >|[**operator\***](structeosio_1_1fixed__point32_a6e18faa394f294b64783e6b46b5d88d6.md#1a6e18faa394f294b64783e6b46b5d88d6) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) const |
|**[fixed\_point64](structeosio_1_1fixed__point64.md)**< Q+32-QR >|[**operator/**](structeosio_1_1fixed__point32_abeb73cfe2cf17016a4ad843f3a8a229c.md#1abeb73cfe2cf17016a4ad843f3a8a229c) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) const |
|bool|[**operator==**](structeosio_1_1fixed__point32_a7a0bf6847a222bf4055736839675a92c.md#1a7a0bf6847a222bf4055736839675a92c) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
|bool|[**operator>**](structeosio_1_1fixed__point32_a6dcd57ee7c9f558a74d8ce561666745d.md#1a6dcd57ee7c9f558a74d8ce561666745d) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
|bool|[**operator<**](structeosio_1_1fixed__point32_a971f9af6f6dd127c726c9238dcf95aa1.md#1a971f9af6f6dd127c726c9238dcf95aa1) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)**< Q > &|[**operator=**](structeosio_1_1fixed__point32_a7e9ed1f23b132f0a8c7cca091321807f.md#1a7e9ed1f23b132f0a8c7cca091321807f) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & r) |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)**< Q > &|[**operator=**](structeosio_1_1fixed__point32_a346f1bad63f2779ddcfc1fb4bbb533f7.md#1a346f1bad63f2779ddcfc1fb4bbb533f7) (const **[fixed\_point64](structeosio_1_1fixed__point64.md)**< QR > & r) |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)**<(Q >QR)?Q:QR >|[**operator+**](structeosio_1_1fixed__point32_a6eefa850d217b820020e76752abfc3eb.md#1a6eefa850d217b820020e76752abfc3eb) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & rhs) const |
|**[fixed\_point32](structeosio_1_1fixed__point32.md)**<(Q >QR)?Q:QR >|[**operator-**](structeosio_1_1fixed__point32_a2c2084d8e4a80f4e75346174b23e78aa.md#1a2c2084d8e4a80f4e75346174b23e78aa) (const **[fixed\_point32](structeosio_1_1fixed__point32.md)**< QR > & rhs) const |


## Detailed Description

This class is implemented to replace the floating point variables and can resolve floating point undetermenistic related issues
Example: 
```cpp
fixed_point32<17> b(9.654);
fixed_point32<18> c = a*b;
fixed_point32<24> d = a+b+c;
fixed_point32<24> e = b/a;
```

 
## Public Attributes Documentation

### variable <a id="1adc0fb8c31ec9e07e6e3a4a7e04fe2471" href="#1adc0fb8c31ec9e07e6e3a4a7e04fe2471">val</a>

```cpp
int32_t eosio::fixed_point32< Q >::val;
```


Value of the fixed point represented as int32\_t 

## Public Functions Documentation

### function <a id="1a81c6f339a55c8327935ef972ebdb278d" href="#1a81c6f339a55c8327935ef972ebdb278d">fixed\_point32 (1/3)</a>

```cpp
eosio::fixed_point32::fixed_point32 (
    const fixed_point32< QR > & r
)
```


Construct a new fixed point32 object from another **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Parameters:**


* **r** - Another **[fixed\_point32](structeosio_1_1fixed__point32.md)** as source 



### function <a id="1a7ad1f82c49b206575fccf8b736815d7c" href="#1a7ad1f82c49b206575fccf8b736815d7c">fixed\_point32 (2/3)</a>

```cpp
eosio::fixed_point32::fixed_point32 (
    const fixed_point64< QR > & r
)
```


Construct a new fixed point32 object from another **[fixed\_point64](structeosio_1_1fixed__point64.md)**. It will be truncated.


**Parameters:**


* **r** - Another **[fixed\_point32](structeosio_1_1fixed__point32.md)** as source 



### function <a id="1af7b7646354bc934510228ca1bd53e6b1" href="#1af7b7646354bc934510228ca1bd53e6b1">fixed\_point32 (3/3)</a>

```cpp
eosio::fixed_point32::fixed_point32 (
    int32_t param = 0
)
```


Construct a new fixed point32 object from int32\_t


**Parameters:**


* **v** - int32\_t representation of the fixed point value 



### function <a id="1ad877eb1cdb682c9c1412204a304c1300" href="#1ad877eb1cdb682c9c1412204a304c1300">int\_part</a>

```cpp
int32_t eosio::fixed_point32::int_part () const
```


Get the integer part of the 64 bit fixed number


**Returns:**

Returns integer part of the fixed number


Example: 
```cpp
fixed_point32<18> a(1234.455667)
std::cout << a.int_part(); // Output: 1234
```

 

### function <a id="1a41ff73528b152e09796cde085fac502d" href="#1a41ff73528b152e09796cde085fac502d">frac\_part</a>

```cpp
uint32_t eosio::fixed_point32::frac_part () const
```



### function <a id="1a464c1dfdc7c689f18fadcd1de6de1359" href="#1a464c1dfdc7c689f18fadcd1de6de1359">print</a>

```cpp
void eosio::fixed_point32::print () const
```


Prints the fixed point value 

### function <a id="1a58d7f252b05e4d5af1376741f8f6f80b" href="#1a58d7f252b05e4d5af1376741f8f6f80b">operator= (1/4)</a>

```cpp
fixed_point32 & eosio::fixed_point32::operator= (
    const fixed_point32< QR > & r
)
```


Assignment operator. Assign **[fixed\_point32](structeosio_1_1fixed__point32.md)** to **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

**[fixed\_point32](structeosio_1_1fixed__point32.md)**& - Reference to this object 




### function <a id="1a3746c182de95876fcbf5784b620d4aed" href="#1a3746c182de95876fcbf5784b620d4aed">operator= (2/4)</a>

```cpp
fixed_point32 & eosio::fixed_point32::operator= (
    const fixed_point64< QR > & r
)
```


Assignment operator. Assign **[fixed\_point64](structeosio_1_1fixed__point64.md)** to **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Template parameters:**


* **qr** - Precision of the source 



**Parameters:**


* **r** - Source 



**Returns:**

**[fixed\_point32](structeosio_1_1fixed__point32.md)**& - Reference to this object 




### function <a id="1a031984309854b00133383caa360dfaef" href="#1a031984309854b00133383caa360dfaef">operator+ (1/2)</a>

```cpp
fixed_point32<(Q >QR)?Q:QR > eosio::fixed_point32::operator+ (
    const fixed_point32< QR > & r
) const
```


Addition operator


**Template parameters:**


* **QR** - Precision of the second addend 



**Parameters:**


* **r** - Second addend 



**Returns:**

- The result of addition 




### function <a id="1a8b853041cdc62f50f21067198ea5d20c" href="#1a8b853041cdc62f50f21067198ea5d20c">operator- (1/2)</a>

```cpp
fixed_point32<(Q >QR)?Q:QR > eosio::fixed_point32::operator- (
    const fixed_point32< QR > & r
) const
```


Subtraction operator


**Template parameters:**


* **QR** - Precision of the minuend 



**Parameters:**


* **r** - Minuend 



**Returns:**

- The result of subtraction 




### function <a id="1a6e18faa394f294b64783e6b46b5d88d6" href="#1a6e18faa394f294b64783e6b46b5d88d6">operator\*</a>

```cpp
fixed_point64< Q+QR > eosio::fixed_point32::operator* (
    const fixed_point32< QR > & r
) const
```


Multiplication operator


**Template parameters:**


* **QR** - Precision of the multiplier 



**Parameters:**


* **r** - Multiplier 



**Returns:**

- The result of multiplication


Multiplication operator for **[fixed\_point32](structeosio_1_1fixed__point32.md)**. The result goes to **[fixed\_point64](structeosio_1_1fixed__point64.md)**


**Note:**

Number of decimal on result will be sum of number of decimals of lhs and rhs


Example: 
```cpp
fixed_point64<33> result = fixed_point32<0>(131313) / fixed_point32<0>(2323)
```

 

### function <a id="1abeb73cfe2cf17016a4ad843f3a8a229c" href="#1abeb73cfe2cf17016a4ad843f3a8a229c">operator/</a>

```cpp
fixed_point64< Q+32-QR > eosio::fixed_point32::operator/ (
    const fixed_point32< QR > & r
) const
```


Division operator


**Template parameters:**


* **QR** - Precision of the divisor 



**Parameters:**


* **r** - Divisor 



**Returns:**

- The result of division


Division of two **[fixed\_point32](structeosio_1_1fixed__point32.md)** result will be stored in **[fixed\_point64](structeosio_1_1fixed__point64.md)**
Example: 
```cpp
fixed_point64<33> result = fixed_point32<0>(131313) / fixed_point32<0>(2323)
```

 

### function <a id="1a7a0bf6847a222bf4055736839675a92c" href="#1a7a0bf6847a222bf4055736839675a92c">operator==</a>

```cpp
bool eosio::fixed_point32::operator== (
    const fixed_point32< QR > & r
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




### function <a id="1a6dcd57ee7c9f558a74d8ce561666745d" href="#1a6dcd57ee7c9f558a74d8ce561666745d">operator></a>

```cpp
bool eosio::fixed_point32::operator> (
    const fixed_point32< QR > & r
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




### function <a id="1a971f9af6f6dd127c726c9238dcf95aa1" href="#1a971f9af6f6dd127c726c9238dcf95aa1">operator<</a>

```cpp
bool eosio::fixed_point32::operator< (
    const fixed_point32< QR > & r
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




### function <a id="1a7e9ed1f23b132f0a8c7cca091321807f" href="#1a7e9ed1f23b132f0a8c7cca091321807f">operator= (3/4)</a>

```cpp
fixed_point32< Q > & eosio::fixed_point32::operator= (
    const fixed_point32< QR > & r
)
```



### function <a id="1a346f1bad63f2779ddcfc1fb4bbb533f7" href="#1a346f1bad63f2779ddcfc1fb4bbb533f7">operator= (4/4)</a>

```cpp
fixed_point32< Q > & eosio::fixed_point32::operator= (
    const fixed_point64< QR > & r
)
```



### function <a id="1a6eefa850d217b820020e76752abfc3eb" href="#1a6eefa850d217b820020e76752abfc3eb">operator+ (2/2)</a>

```cpp
fixed_point32<(Q >QR)?Q:QR > eosio::fixed_point32::operator+ (
    const fixed_point32< QR > & rhs
) const
```


Addition between two **[fixed\_point32](structeosio_1_1fixed__point32.md)** variables and the result goes to **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Note:**

Number of decimal on result will be max of decimals of lhs and rhs 




### function <a id="1a2c2084d8e4a80f4e75346174b23e78aa" href="#1a2c2084d8e4a80f4e75346174b23e78aa">operator- (2/2)</a>

```cpp
fixed_point32<(Q >QR)?Q:QR > eosio::fixed_point32::operator- (
    const fixed_point32< QR > & rhs
) const
```


Subtraction between two **[fixed\_point32](structeosio_1_1fixed__point32.md)** variables and the result goes to **[fixed\_point32](structeosio_1_1fixed__point32.md)**


**Note:**

Number of decimal on result will be max of decimals of lhs and rhs 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/fixedpoint.hpp`
