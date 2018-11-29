---
search:
    keywords: ['eosio::datastream', 'datastream', 'skip', 'read', 'write', 'put', 'get', 'get', 'pos', 'valid', 'seekp', 'tellp', 'remaining']
---

# class eosio::datastream

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**datastream**](classeosio_1_1datastream.md)


## Public Functions

|Type|Name|
|-----|-----|
||[**datastream**](classeosio_1_1datastream_a72befc5c1f51a601ad07e7c8e61856f4.md#1a72befc5c1f51a601ad07e7c8e61856f4) (T start, size\_t s) <br>Construct a new datastream object. |
|void|[**skip**](classeosio_1_1datastream_a928fbd3301caee3c64ea620442fb0e64.md#1a928fbd3301caee3c64ea620442fb0e64) (size\_t s) |
|bool|[**read**](classeosio_1_1datastream_ae1d8d46d5cf80745bbb00a6b8fee243a.md#1ae1d8d46d5cf80745bbb00a6b8fee243a) (char \* d, size\_t s) |
|bool|[**write**](classeosio_1_1datastream_a1e42133f6f57a8c89e1db04abe44b342.md#1a1e42133f6f57a8c89e1db04abe44b342) (const char \* d, size\_t s) |
|bool|[**put**](classeosio_1_1datastream_ad0a23aa024810028ce8669627577859d.md#1ad0a23aa024810028ce8669627577859d) (char c) <br>Writes a byte into the stream. |
|bool|[**get**](classeosio_1_1datastream_a001047d70942ab5beb836e498d9f9eab.md#1a001047d70942ab5beb836e498d9f9eab) (unsigned char & c) <br>Reads a byte from the stream. |
|bool|[**get**](classeosio_1_1datastream_a10ab58b7cc695366b42957f4b8ff56eb.md#1a10ab58b7cc695366b42957f4b8ff56eb) (char & c) <br>Reads a byte from the stream. |
|T|[**pos**](classeosio_1_1datastream_a090e80d1bddd21337b16b3fad76c551d.md#1a090e80d1bddd21337b16b3fad76c551d) () const <br>Retrieves the current position of the stream. |
|bool|[**valid**](classeosio_1_1datastream_a69cbb57958a1ec6d66e6aa4bcc39ec23.md#1a69cbb57958a1ec6d66e6aa4bcc39ec23) () const |
|bool|[**seekp**](classeosio_1_1datastream_ac82f1102bb9593764a654dfbedb88328.md#1ac82f1102bb9593764a654dfbedb88328) (size\_t p) <br>Sets the position within the current stream. |
|size\_t|[**tellp**](classeosio_1_1datastream_a32df7a816da793f3183d235c67b042a7.md#1a32df7a816da793f3183d235c67b042a7) () const <br>Gets the position within the current stream. |
|size\_t|[**remaining**](classeosio_1_1datastream_af57bc94263dc08eb60b910a53645a052.md#1af57bc94263dc08eb60b910a53645a052) () const <br>Returns the number of remaining bytes that can be read/skipped. |


## Detailed Description

A data stream for reading and writing data in the form of bytes


**Template parameters:**


* **T** - Type of the datastream buffer 


## Public Functions Documentation

### function <a id="1a72befc5c1f51a601ad07e7c8e61856f4" href="#1a72befc5c1f51a601ad07e7c8e61856f4">datastream</a>

```cpp
eosio::datastream::datastream (
    T start,
    size_t s
)
```

Construct a new datastream object. 

Construct a new datastream object given the size of the buffer and start position of the buffer 

**Parameters:**


* **start** - The start position of the buffer 
* **s** - The size of the buffer 



### function <a id="1a928fbd3301caee3c64ea620442fb0e64" href="#1a928fbd3301caee3c64ea620442fb0e64">skip</a>

```cpp
void eosio::datastream::skip (
    size_t s
)
```


Skips a specified number of bytes from this stream


**Parameters:**


* **s** - The number of bytes to skip 



### function <a id="1ae1d8d46d5cf80745bbb00a6b8fee243a" href="#1ae1d8d46d5cf80745bbb00a6b8fee243a">read</a>

```cpp
bool eosio::datastream::read (
    char * d,
    size_t s
)
```


Reads a specified number of bytes from the stream into a buffer


**Parameters:**


* **d** - The pointer to the destination buffer 
* **s** - the number of bytes to read 



**Returns:**

true 




### function <a id="1a1e42133f6f57a8c89e1db04abe44b342" href="#1a1e42133f6f57a8c89e1db04abe44b342">write</a>

```cpp
bool eosio::datastream::write (
    const char * d,
    size_t s
)
```


Writes a specified number of bytes into the stream from a buffer


**Parameters:**


* **d** - The pointer to the source buffer 
* **s** - The number of bytes to write 



**Returns:**

true 




### function <a id="1ad0a23aa024810028ce8669627577859d" href="#1ad0a23aa024810028ce8669627577859d">put</a>

```cpp
bool eosio::datastream::put (
    char c
)
```

Writes a byte into the stream. 

Writes a byte into the stream


**Parameters:**


* **c** byte to write 



**Returns:**

true 




### function <a id="1a001047d70942ab5beb836e498d9f9eab" href="#1a001047d70942ab5beb836e498d9f9eab">get (1/2)</a>

```cpp
bool eosio::datastream::get (
    unsigned char & c
)
```

Reads a byte from the stream. 

Reads a byte from the stream


**Parameters:**


* **c** - The reference to destination byte 



**Returns:**

true 




### function <a id="1a10ab58b7cc695366b42957f4b8ff56eb" href="#1a10ab58b7cc695366b42957f4b8ff56eb">get (2/2)</a>

```cpp
bool eosio::datastream::get (
    char & c
)
```

Reads a byte from the stream. 

Reads a byte from the stream


**Parameters:**


* **c** - The reference to destination byte 



**Returns:**

true 




### function <a id="1a090e80d1bddd21337b16b3fad76c551d" href="#1a090e80d1bddd21337b16b3fad76c551d">pos</a>

```cpp
T eosio::datastream::pos () const
```

Retrieves the current position of the stream. 

Retrieves the current position of the stream


**Returns:**

T - The current position of the stream 




### function <a id="1a69cbb57958a1ec6d66e6aa4bcc39ec23" href="#1a69cbb57958a1ec6d66e6aa4bcc39ec23">valid</a>

```cpp
bool eosio::datastream::valid () const
```



### function <a id="1ac82f1102bb9593764a654dfbedb88328" href="#1ac82f1102bb9593764a654dfbedb88328">seekp</a>

```cpp
bool eosio::datastream::seekp (
    size_t p
)
```

Sets the position within the current stream. 

Sets the position within the current stream


**Parameters:**


* **p** - The offset relative to the origin 



**Returns:**

true if p is within the range 




**Returns:**

false if p is not within the rawnge 




### function <a id="1a32df7a816da793f3183d235c67b042a7" href="#1a32df7a816da793f3183d235c67b042a7">tellp</a>

```cpp
size_t eosio::datastream::tellp () const
```

Gets the position within the current stream. 

Gets the position within the current stream


**Returns:**

p - The position within the current stream 




### function <a id="1af57bc94263dc08eb60b910a53645a052" href="#1af57bc94263dc08eb60b910a53645a052">remaining</a>

```cpp
size_t eosio::datastream::remaining () const
```

Returns the number of remaining bytes that can be read/skipped. 

Returns the number of remaining bytes that can be read/skipped


**Returns:**

size\_t - The number of remaining bytes 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/datastream.hpp`
