---
search:
    keywords: ['datastream', 'eosio::datastream', 'eosio::datastream< size_t >', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator<<', 'operator>>', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'operator<<', 'operator>>', 'unpack', 'unpack', 'pack_size', 'pack', 'operator<<', 'operator>>', 'operator<<', 'operator>>']
---

# group datastream

Defines data stream for reading and writing data in the form of bytes. 
## Classes

|Type|Name|
|-----|-----|
|class|[**eosio::datastream**](classeosio_1_1datastream.md)|
|class|[**eosio::datastream< size\_t >**](classeosio_1_1datastream_3_01size__t_01_4.md)|


## Functions Documentation

### function <a id="ga1e49e445971a761be9607c1a3eb85bde" href="#ga1e49e445971a761be9607c1a3eb85bde">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const std::optional< T > & opt
)
```

Serialize an optional. 

Serialize an optional into a stream


**Parameters:**


* **ds** - The stream to write 
* **opt** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga9e230ddd4f3d8ff314410dc9832829fd" href="#ga9e230ddd4f3d8ff314410dc9832829fd">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    std::optional< T > & opt
)
```

Deserialize an optional. 

Deserialize an optional from a stream


**Parameters:**


* **ds** - The stream to read 
* **opt** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga5413cb7eba4e557a70d93ffa9f39b725" href="#ga5413cb7eba4e557a70d93ffa9f39b725">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const eosio::symbol_code sym_code
)
```

Serialize a **[symbol\_code](classeosio_1_1symbol__code.md)**. 

Serialize a **[symbol\_code](classeosio_1_1symbol__code.md)** into a stream


**Parameters:**


* **ds** - The stream to write 
* **sym** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga647b59d0f66be8cbb0bac116d7d14a9e" href="#ga647b59d0f66be8cbb0bac116d7d14a9e">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    eosio::symbol_code & sym_code
)
```

Deserialize a **[symbol\_code](classeosio_1_1symbol__code.md)**. 

Deserialize a **[symbol\_code](classeosio_1_1symbol__code.md)** from a stream


**Parameters:**


* **ds** - The stream to read 
* **symbol** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga4f20e587315c1c625ec8c7092f9d8465" href="#ga4f20e587315c1c625ec8c7092f9d8465">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const eosio::symbol sym
)
```

Serialize a symbol. 

Serialize a symbol into a stream


**Parameters:**


* **ds** - The stream to write 
* **sym** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga015cd9edd44cc1af80641c39b4816f13" href="#ga015cd9edd44cc1af80641c39b4816f13">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    eosio::symbol & sym
)
```

Deserialize a symbol. 

Deserialize a symbol from a stream


**Parameters:**


* **ds** - The stream to read 
* **symbol** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga344108ff96bcee67d40711dacdd42845" href="#ga344108ff96bcee67d40711dacdd42845">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const ::eosio::ignore_wrapper< T > & val
)
```

Serialize ignored\_wrapper<T>'s T value. 

Serialize an ignored\_wrapper type into a stream


**Parameters:**


* **ds** - The stream to write 
* **val** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga05879a71914d293c8a30b1a6710f8e51" href="#ga05879a71914d293c8a30b1a6710f8e51">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const ::eosio::ignore< T > & val
)
```

Serialize an ignored type. 

Serialize an ignored type into a stream


**Parameters:**


* **ds** - The stream to write 
* **ignore** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="gafea0491fecd3df192a3f245e8105a026" href="#gafea0491fecd3df192a3f245e8105a026">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    ::eosio::ignore< T > 
)
```

Deserialize an ignored type. 

Deserialize an ignored type from a stream


**Parameters:**


* **ds** - The stream to read 
* **ignored** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="gaeb6280d5e7942fcf839024442bc5c814" href="#gaeb6280d5e7942fcf839024442bc5c814">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const capi_public_key pubkey
)
```

Serialize a **[public\_key](structeosio_1_1public__key.md)**. 

Serialize a **[public\_key](structeosio_1_1public__key.md)** into a stream


**Parameters:**


* **ds** - The stream to write 
* **pubkey** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga6f1b392200e9452c7f410ffd8fdfafe1" href="#ga6f1b392200e9452c7f410ffd8fdfafe1">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    capi_public_key & pubkey
)
```

Deserialize a **[public\_key](structeosio_1_1public__key.md)**. 

Deserialize a **[public\_key](structeosio_1_1public__key.md)** from a stream


**Parameters:**


* **ds** - The stream to read 
* **pubkey** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="gaed3bc5bd372251b9cb90aff4cf85be5f" href="#gaed3bc5bd372251b9cb90aff4cf85be5f">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const key256 & d
)
```

Serialize a key256. 

Serialize a key256 into a stream


**Parameters:**


* **ds** - The stream to write 
* **d** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga74974be2214a4e50e463ae866f9297d4" href="#ga74974be2214a4e50e463ae866f9297d4">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    key256 & d
)
```

Deserialize a key256. 

Deserialize a key256 from a stream


**Parameters:**


* **ds** - The stream to read 
* **d** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga7875cabe436fe6756c4b30eec326fe16" href="#ga7875cabe436fe6756c4b30eec326fe16">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const bool & d
)
```

Serialize a bool into a stream. 

Serialize a bool into a stream


**Parameters:**


* **ds** - The stream to read 
* **d** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga860a2cac0f4f00dacdade421c303ea9c" href="#ga860a2cac0f4f00dacdade421c303ea9c">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    bool & d
)
```

Deserialize a bool. 

Deserialize a bool from a stream


**Parameters:**


* **ds** - The stream to read 
* **d** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga69731c7e836d6521c2aa7d67ad1dbc4d" href="#ga69731c7e836d6521c2aa7d67ad1dbc4d">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const capi_checksum256 & d
)
```

Serialize a checksum256. 

Serialize a checksum256 into a stream


**Parameters:**


* **ds** - The stream to write 
* **d** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="gaf18798375dd9ed43bea5888101883b9c" href="#gaf18798375dd9ed43bea5888101883b9c">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    capi_checksum256 & d
)
```

Deserialize a checksum256. 

Deserialize a checksum256 from a stream


**Parameters:**


* **ds** - The stream to read 
* **d** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="gab1dffa510122bd62ce3975e7b302c2c1" href="#gab1dffa510122bd62ce3975e7b302c2c1">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const std::string & v
)
```

Serialize a string. 

Serialize a string into a stream


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="gaa02846796253d9db1064e43fadbecb8e" href="#gaa02846796253d9db1064e43fadbecb8e">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    std::string & v
)
```

Deserialize a string. 

Deserialize a string from a stream


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga86c994b49d0af580c72f4d966baca75b" href="#ga86c994b49d0af580c72f4d966baca75b">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const std::array< T, N > & v
)
```

Serialize a fixed size array. 

Serialize a fixed size array into a stream


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the object contained in the array 
* **N** - Size of the array 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga60d3eb6bab6da17bd2986a051126d471" href="#ga60d3eb6bab6da17bd2986a051126d471">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    std::array< T, N > & v
)
```

Deserialize a fixed size array. 

Deserialize a fixed size array from a stream


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the object contained in the array 
* **N** - Size of the array 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="gac4ea18eb72ec93e5d06fff5a83623581" href="#gac4ea18eb72ec93e5d06fff5a83623581">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    T 
)
```

Deserialize a a pointer. 

Pointer should not be serialized, so this function will always throws an error


**Parameters:**


* **ds** - The stream to read 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the pointer 



**Returns:**

DataStream& - Reference to the datastream 




**Post**

Throw an exception if it is a pointer 




### function <a id="ga0c8758a01d2cdcd99922c268f2f4e4e9" href="#ga0c8758a01d2cdcd99922c268f2f4e4e9">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const T(&) v
)
```

Serialize a fixed size array of non-primitive and non-pointer type. 

Serialize a fixed size array of non-primitive type.
Serialize a fixed size array of non-primitive and non-pointer type


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the pointer 



**Returns:**

DataStream& - Reference to the datastream


Serialize a fixed size array of non-primitive type


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the pointer 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga9ddfcc1002e9cd4c267ca0986753c686" href="#ga9ddfcc1002e9cd4c267ca0986753c686">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    T(&) v
)
```

Deserialize a fixed size array of non-primitive and non-pointer type. 

Deserialize a fixed size array of non-primitive type.
Deserialize a fixed size array of non-primitive and non-pointer type


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **T** - Type of the object contained in the array 
* **N** - Size of the array 
* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream


Deserialize a fixed size array of non-primitive type


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **T** - Type of the object contained in the array 
* **N** - Size of the array 
* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga5cb28ef1142e7ec5ba5c6672265efd47" href="#ga5cb28ef1142e7ec5ba5c6672265efd47">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const std::vector< char > & v
)
```

Serialize a vector of char. 

Serialize a vector of char


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga53f4e770b9b3c9f1e92c2fa3fa0ec73a" href="#ga53f4e770b9b3c9f1e92c2fa3fa0ec73a">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const std::vector< T > & v
)
```

Serialize a vector. 

Serialize a vector


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the object contained in the vector 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="gad202e8fcf8f49970c1710410c6059cde" href="#gad202e8fcf8f49970c1710410c6059cde">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    std::vector< char > & v
)
```

Deserialize a vector of char. 

Deserialize a vector of char


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="gaedee239cff148b596f19a3c2c6c1adf1" href="#gaedee239cff148b596f19a3c2c6c1adf1">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    std::vector< T > & v
)
```

Deserialize a vector. 

Deserialize a vector


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the object contained in the vector 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="gaf98424a2563d883b3883a14692aa195a" href="#gaf98424a2563d883b3883a14692aa195a">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const std::set< T > & s
)
```



### function <a id="gae2a202ccccf405468ad654676bb41d40" href="#gae2a202ccccf405468ad654676bb41d40">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    std::set< T > & s
)
```



### function <a id="ga0bb2cdf062825f96551ba663f3d74e17" href="#ga0bb2cdf062825f96551ba663f3d74e17">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const std::map< K, V > & m
)
```

Serialize a map. 

Serialize a map


**Parameters:**


* **ds** - The stream to write 
* **m** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **K** - Type of the key contained in the map 
* **V** - Type of the value contained in the map 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga63b7d3e0c99e4f049574fd366a1c528a" href="#ga63b7d3e0c99e4f049574fd366a1c528a">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    std::map< K, V > & m
)
```

Deserialize a map. 

Deserialize a map


**Parameters:**


* **ds** - The stream to read 
* **m** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 
* **K** - Type of the key contained in the map 
* **V** - Type of the value contained in the map 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="gab7049fdaa7895e6c31a482deba846434" href="#gab7049fdaa7895e6c31a482deba846434">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const boost::container::flat_set< T > & s
)
```



### function <a id="ga5f545f70b39155d1cd1da85f0e00b742" href="#ga5f545f70b39155d1cd1da85f0e00b742">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    boost::container::flat_set< T > & s
)
```



### function <a id="ga85b47a27bb41a2854d2b8daea9e7b9f0" href="#ga85b47a27bb41a2854d2b8daea9e7b9f0">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const boost::container::flat_map< K, V > & m
)
```

Serialize a flat map. 

Serialize a flat map


**Parameters:**


* **ds** - The stream to write 
* **m** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **K** - Type of the key contained in the flat map 
* **V** - Type of the value contained in the flat map 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga3b973878181cef2bdad7139958bd6ecf" href="#ga3b973878181cef2bdad7139958bd6ecf">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    boost::container::flat_map< K, V > & m
)
```

Deserialize a flat map. 

Deserialize a flat map


**Parameters:**


* **ds** - The stream to read 
* **m** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 
* **K** - Type of the key contained in the flat map 
* **V** - Type of the value contained in the flat map 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga124a00ba4d603044ef25be84a9fade48" href="#ga124a00ba4d603044ef25be84a9fade48">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const std::tuple< Args... > & t
)
```

Serialize a tuple. 

Serialize a tuple


**Parameters:**


* **ds** - The stream to write 
* **t** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **Args** - Type of the objects contained in the tuple 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="gac2e5487f43a496d7c263071e2502b986" href="#gac2e5487f43a496d7c263071e2502b986">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    std::tuple< Args... > & t
)
```

Deserialize a tuple. 

Deserialize a tuple


**Parameters:**


* **ds** - The stream to read 
* **t** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 
* **Args** - Type of the objects contained in the tuple 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga59511f8552dbf4a2aced270041c17f04" href="#ga59511f8552dbf4a2aced270041c17f04">operator<<</a>

```cpp
DataStream & datastream::operator<< (
    DataStream & ds,
    const T & v
)
```

Serialize a class. 

Serialize a primitive type.
Serialize a class


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of class 



**Returns:**

DataStream& - Reference to the datastream


Serialize a primitive type


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the primitive type 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga7d64e2f48bca13601cf6606934e6e8e6" href="#ga7d64e2f48bca13601cf6606934e6e8e6">operator>></a>

```cpp
DataStream & datastream::operator>> (
    DataStream & ds,
    T & v
)
```

Deserialize a class. 

Deserialize a primitive type.
Deserialize a class


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of class 



**Returns:**

DataStream& - Reference to the datastream


Deserialize a primitive type


**Parameters:**


* **ds** - The stream to read 
* **v** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 
* **T** - Type of the primitive type 



**Returns:**

DataStream& - Reference to the datastream 




### function <a id="ga2ffaa5bd18836b7d40db601fb20965bb" href="#ga2ffaa5bd18836b7d40db601fb20965bb">unpack</a>

```cpp
T datastream::unpack (
    const char * buffer,
    size_t len
)
```

Unpack data inside a fixed size buffer as T. 

Unpack data inside a fixed size buffer as T


**Template parameters:**


* **T** - Type of the unpacked data 



**Parameters:**


* **buffer** - Pointer to the buffer 
* **len** - Length of the buffer 



**Returns:**

T - The unpacked data 




### function <a id="ga666b4d583161e4db595e31565fbb6a3d" href="#ga666b4d583161e4db595e31565fbb6a3d">unpack</a>

```cpp
T datastream::unpack (
    const std::vector< char > & bytes
)
```

Unpack data inside a variable size buffer as T. 

Unpack data inside a variable size buffer as T


**Template parameters:**


* **T** - Type of the unpacked data 



**Parameters:**


* **bytes** - Buffer 



**Returns:**

T - The unpacked data 




### function <a id="ga30aa7d1c92792afc7f78af37372d68b7" href="#ga30aa7d1c92792afc7f78af37372d68b7">pack\_size</a>

```cpp
size_t datastream::pack_size (
    const T & value
)
```

Get the size of the packed data. 

Get the size of the packed data


**Template parameters:**


* **T** - Type of the data to be packed 



**Parameters:**


* **value** - Data to be packed 



**Returns:**

size\_t - Size of the packed data 




### function <a id="ga16e657110c5a8e1ac34056bc473460a8" href="#ga16e657110c5a8e1ac34056bc473460a8">pack</a>

```cpp
std::vector< char > datastream::pack (
    const T & value
)
```

Get packed data. 

Get packed data


**Template parameters:**


* **T** - Type of the data to be packed 



**Parameters:**


* **value** - Data to be packed 



**Returns:**

bytes - The packed data 




### function <a id="gac006ab398b288a0582a2a59b991bf37c" href="#gac006ab398b288a0582a2a59b991bf37c">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const capi_checksum160 & cs
)
```

Serializea capi\_checksum160 type. 

Serialize a capi\_checksum160 type


**Parameters:**


* **ds** - The stream to write 
* **cs** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga2cbcd06b5aae0e8ffd8b1550b0f39983" href="#ga2cbcd06b5aae0e8ffd8b1550b0f39983">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    capi_checksum160 & cs
)
```

Deserialize a capi\_checksum160 type. 

Deserialize a capi\_checksum160 type


**Parameters:**


* **ds** - The stream to read 
* **cs** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="gafd1c5d3fad33115c2ba6a65f8f4b5792" href="#gafd1c5d3fad33115c2ba6a65f8f4b5792">operator<<</a>

```cpp
datastream< Stream > & datastream::operator<< (
    datastream< Stream > & ds,
    const capi_checksum512 & cs
)
```

Serialize a capi\_checksum512 type. 

Serialize a capi\_checksum512 type


**Parameters:**


* **ds** - The stream to write 
* **cs** - The value to serialize 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




### function <a id="ga14d1988dcb3235cf059d90c01a2b7d74" href="#ga14d1988dcb3235cf059d90c01a2b7d74">operator>></a>

```cpp
datastream< Stream > & datastream::operator>> (
    datastream< Stream > & ds,
    capi_checksum512 & cs
)
```

Deserialize a capi\_checksum512 type. 

Deserialize a capi\_checksum512 type


**Parameters:**


* **ds** - The stream to read 
* **cs** - The destination for deserialized value 



**Template parameters:**


* **Stream** - Type of datastream buffer 



**Returns:**

datastream<Stream>& - Reference to the datastream 




