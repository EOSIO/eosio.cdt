---
search:
    keywords: ['console', 'eosio::iostream', 'cout', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print', 'print_f', 'print_f', 'print', 'operator<<']
---

# group console

Defines C++ wrapper to log/print text messages. [More...](#detailed-description)
## Classes

|Type|Name|
|-----|-----|
|class|[**eosio::iostream**](classeosio_1_1iostream.md)|


## Variables

|Type|Name|
|-----|-----|
|static iostream|[**cout**](group__console.md#gaed9b7380e114191d3dac1857b35e3da2)|


## Detailed Description

This API uses C++ variadic templates and type detection to make it easy to print any native type. You can even overload the `` method for your own custom types.
**Example:** 
```cpp
print( "hello world, this is a number: ", 5 );
```


## Variables Documentation

### variable <a id="gaed9b7380e114191d3dac1857b35e3da2" href="#gaed9b7380e114191d3dac1857b35e3da2">cout</a>

```cpp
iostream eosio::cout;
```



## Functions Documentation

### function <a id="gaa0a56fe314d032d027a1565d93a68b21" href="#gaa0a56fe314d032d027a1565d93a68b21">print</a>

```cpp
void console::print (
    const char * ptr
)
```


Prints string


**Parameters:**


* **ptr** - a null terminated string 



### function <a id="gabe1c02d3a4a65108ff3bc59db64485a0" href="#gabe1c02d3a4a65108ff3bc59db64485a0">print</a>

```cpp
void console::print (
    const std::string & s
)
```

Prints string. 

Prints string


**Parameters:**


* **s** - a const std::string 



### function <a id="ga6daff3c05760581064c390a27a79fd30" href="#ga6daff3c05760581064c390a27a79fd30">print</a>

```cpp
void console::print (
    std::string & s
)
```

Prints string. 

Prints string


**Parameters:**


* **s** - a std::string 



### function <a id="ga514f168270d72ecae0083dd2968f7329" href="#ga514f168270d72ecae0083dd2968f7329">print</a>

```cpp
void console::print (
    const char c
)
```

Prints string. 

Prints string


**Parameters:**


* **c** - a const char 



### function <a id="gad0342d58fa17a7bea359607c6cfcf0cf" href="#gad0342d58fa17a7bea359607c6cfcf0cf">print</a>

```cpp
void console::print (
    int num
)
```


Prints signed integer as a 64 bit signed integer


**Parameters:**


* **num** to be printed 



### function <a id="ga4412b37ef401946605e5cef937c3f8dc" href="#ga4412b37ef401946605e5cef937c3f8dc">print</a>

```cpp
void console::print (
    int32_t num
)
```


Prints 32 bit signed integer as a 64 bit signed integer


**Parameters:**


* **num** to be printed 



### function <a id="gaad90d9aa7b7ba103d2e943bf38c91072" href="#gaad90d9aa7b7ba103d2e943bf38c91072">print</a>

```cpp
void console::print (
    int64_t num
)
```


Prints 64 bit signed integer as a 64 bit signed integer


**Parameters:**


* **num** to be printed 



### function <a id="ga3c85835d3c6e556b5da524030e70e971" href="#ga3c85835d3c6e556b5da524030e70e971">print</a>

```cpp
void console::print (
    unsigned int num
)
```


Prints unsigned integer as a 64 bit unsigned integer


**Parameters:**


* **num** to be printed 



### function <a id="ga0c95ab36e2c3f91a627270c09ff9140f" href="#ga0c95ab36e2c3f91a627270c09ff9140f">print</a>

```cpp
void console::print (
    uint32_t num
)
```


Prints 32 bit unsigned integer as a 64 bit unsigned integer


**Parameters:**


* **num** to be printed 



### function <a id="ga9bdf0bd18006f43820d73ec9f8cc50b8" href="#ga9bdf0bd18006f43820d73ec9f8cc50b8">print</a>

```cpp
void console::print (
    uint64_t num
)
```


Prints 64 bit unsigned integer as a 64 bit unsigned integer


**Parameters:**


* **num** to be printed 



### function <a id="ga925806f986898c01e8de34234f951bdc" href="#ga925806f986898c01e8de34234f951bdc">print</a>

```cpp
void console::print (
    int128_t num
)
```


Prints 128 bit signed integer


**Parameters:**


* **num** to be printed 



### function <a id="gacbb116ac61f216fafa100011bce090ee" href="#gacbb116ac61f216fafa100011bce090ee">print</a>

```cpp
void console::print (
    uint128_t num
)
```


Prints 128 bit unsigned integer


**Parameters:**


* **num** to be printed 



### function <a id="ga02d168bc05492d64cefea469dd114c12" href="#ga02d168bc05492d64cefea469dd114c12">print</a>

```cpp
void console::print (
    float num
)
```


Prints single-precision floating point number (i.e. float)


**Parameters:**


* **num** to be printed 



### function <a id="ga8753f6a7c7d5c518f6369cfebf5be1bb" href="#ga8753f6a7c7d5c518f6369cfebf5be1bb">print</a>

```cpp
void console::print (
    double num
)
```


Prints double-precision floating point number (i.e. double)


**Parameters:**


* **num** to be printed 



### function <a id="ga2a3eea270ff4dbe4a15c2269a5c2cfec" href="#ga2a3eea270ff4dbe4a15c2269a5c2cfec">print</a>

```cpp
void console::print (
    long double num
)
```


Prints quadruple-precision floating point number (i.e. long double)


**Parameters:**


* **num** to be printed 



### function <a id="ga2785282e4babb79499ba03e719e20aa1" href="#ga2785282e4babb79499ba03e719e20aa1">print</a>

```cpp
void console::print (
    const fixed_key< Size > & val
)
```


Prints **[fixed\_key](classeosio_1_1fixed__key.md)** as a hexidecimal string


**Parameters:**


* **val** to be printed 



### function <a id="gaa47b5b44cf50be5fe5ca837ef7e6d583" href="#gaa47b5b44cf50be5fe5ca837ef7e6d583">print</a>

```cpp
void console::print (
    fixed_key< Size > & val
)
```


Prints **[fixed\_key](classeosio_1_1fixed__key.md)** as a hexidecimal string


**Parameters:**


* **val** to be printed 



### function <a id="gaddacdc539ef6e6fb4f55bf04d1a0d22c" href="#gaddacdc539ef6e6fb4f55bf04d1a0d22c">print</a>

```cpp
void console::print (
    name name
)
```


Prints a 64 bit names as base32 encoded string


**Parameters:**


* **name** 64 bit name to be printed 



### function <a id="gaeb64d709bf057338af83ea1b95a0d16d" href="#gaeb64d709bf057338af83ea1b95a0d16d">print</a>

```cpp
void console::print (
    eosio::symbol_code sym_code
)
```


Prints a **[symbol\_code](classeosio_1_1symbol__code.md)**


**Parameters:**


* **sym\_code** symbol code to be printed 



### function <a id="ga3b7dd9bbca5a20bad3acb94b6b7354be" href="#ga3b7dd9bbca5a20bad3acb94b6b7354be">print</a>

```cpp
void console::print (
    bool val
)
```


Prints bool


**Parameters:**


* **val** to be printed 



### function <a id="gaa09d15641c76813eea61b876d8266400" href="#gaa09d15641c76813eea61b876d8266400">print</a>

```cpp
void console::print (
    T && t
)
```


Prints class object


**Parameters:**


* **t** to be printed 



**Precondition:**

T must implements **[print()](group__console_gaa0a56fe314d032d027a1565d93a68b21.md#gaa0a56fe314d032d027a1565d93a68b21)** function 




### function <a id="gab9715bf002b919701d38e3ff931feabf" href="#gab9715bf002b919701d38e3ff931feabf">print\_f</a>

```cpp
void console::print_f (
    const char * s
)
```


Prints null terminated string


**Parameters:**


* **s** null terminated string to be printed 



### function <a id="gafcfae01c6d714474da8407a1c195c836" href="#gafcfae01c6d714474da8407a1c195c836">print\_f</a>

```cpp
void console::print_f (
    const char * s,
    Arg val,
    Args... rest
)
```


Prints formatted string. It behaves similar to C printf/


**Template parameters:**


* **Arg** - Type of the value used to replace the format specifier 
* **Args** - Type of the value used to replace the format specifier 



**Parameters:**


* **s** - Null terminated string with to be printed (it can contains format specifier) 
* **val** - The value used to replace the format specifier 
* **rest** - The values used to replace the format specifier

Example: 
```cpp
print_f("Number of apples: %", 10);
```

 

### function <a id="gab5232cf7452d463948eac1179f31d9f0" href="#gab5232cf7452d463948eac1179f31d9f0">print</a>

```cpp
void console::print (
    Arg && a,
    Args &&... args
)
```


Print out value / list of values


**Template parameters:**


* **Arg** - Type of the value used to replace the format specifier 
* **Args** - Type of the value used to replace the format specifier 



**Parameters:**


* **a** - The value to be printed 
* **args** - The other values to be printed

Example:

```cpp
const char *s = "Hello World!";
uint64_t unsigned_64_bit_int = 1e+18;
uint128_t unsigned_128_bit_int (87654323456);
uint64_t string_as_unsigned_64_bit = N(abcde);
print(s , unsigned_64_bit_int, unsigned_128_bit_int, string_as_unsigned_64_bit);
// Ouput: Hello World!100000000000000000087654323456abcde
```

 

### function <a id="ga827f0aac6315e7963bcd52656cd7eeed" href="#ga827f0aac6315e7963bcd52656cd7eeed">operator<<</a>

```cpp
iostream & console::operator<< (
    iostream & out,
    const T & v
)
```


Overload c++ iostream


**Template parameters:**


* **Arg** - Type of the value used to replace the format specifier 
* **Args** - Type of the value used to replace the format specifier 



**Parameters:**


* **out** - Output strem 
* **v** - The value to be printed 



**Returns:**

iostream& - Reference to the input output stream


Example:

```cpp
const char *s = "Hello World!";
uint64_t unsigned_64_bit_int = 1e+18;
uint128_t unsigned_128_bit_int (87654323456);
uint64_t string_as_unsigned_64_bit = N(abcde);
std::out << s << " " << unsigned_64_bit_int << " "  << unsigned_128_bit_int << " " << string_as_unsigned_64_bit;
// Output: Hello World! 1000000000000000000 87654323456 abcde
```

 

