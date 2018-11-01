---
search:
    keywords: ['eosio::datastream< size_t >', 'datastream', 'skip', 'write', 'put', 'valid', 'seekp', 'tellp', 'remaining']
---

# class eosio::datastream< size\_t >

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**datastream< size\_t >**](classeosio_1_1datastream_3_01size__t_01_4.md)


Specialization of datastream used to help determine the final size of a serialized value. Specialization of datastream used to help determine the final size of a serialized value. 
## Public Functions

|Type|Name|
|-----|-----|
||[**datastream**](classeosio_1_1datastream_3_01size__t_01_4_abf5e113ace4a6bfa309fcecf3fc49f63.md#1abf5e113ace4a6bfa309fcecf3fc49f63) (size\_t init\_size = 0) <br>Construct a new specialized datastream object. |
|bool|[**skip**](classeosio_1_1datastream_3_01size__t_01_4_ad9d7da49e77a5db9c80c256845b26894.md#1ad9d7da49e77a5db9c80c256845b26894) (size\_t s) <br>Increase the size by s. |
|bool|[**write**](classeosio_1_1datastream_3_01size__t_01_4_a3c2ca245c9f33549505ea7886a1d3ecf.md#1a3c2ca245c9f33549505ea7886a1d3ecf) (const char \*, size\_t s) <br>Increase the size by s. |
|bool|[**put**](classeosio_1_1datastream_3_01size__t_01_4_a4a1a95751dafd76aea580d80c84bf8bb.md#1a4a1a95751dafd76aea580d80c84bf8bb) (char) <br>Increase the size by one. |
|bool|[**valid**](classeosio_1_1datastream_3_01size__t_01_4_a46f1fc6f14d7e79c2181f4e3a2ae5b12.md#1a46f1fc6f14d7e79c2181f4e3a2ae5b12) () const <br>Check validity. |
|bool|[**seekp**](classeosio_1_1datastream_3_01size__t_01_4_ac4720cb82d256a41f27bbf389b37790b.md#1ac4720cb82d256a41f27bbf389b37790b) (size\_t p) <br>Set new size. |
|size\_t|[**tellp**](classeosio_1_1datastream_3_01size__t_01_4_aadcdabf95d8b142bd9b43bbcdc4e0d9f.md#1aadcdabf95d8b142bd9b43bbcdc4e0d9f) () const <br>Get the size. |
|size\_t|[**remaining**](classeosio_1_1datastream_3_01size__t_01_4_a345f36fa8492917662d6185db631042d.md#1a345f36fa8492917662d6185db631042d) () const <br>Always returns 0. |


## Public Functions Documentation

### function <a id="1abf5e113ace4a6bfa309fcecf3fc49f63" href="#1abf5e113ace4a6bfa309fcecf3fc49f63">datastream</a>

```cpp
eosio::datastream< size_t >::datastream (
    size_t init_size = 0
)
```

Construct a new specialized datastream object. 

Construct a new specialized datastream object given the initial size


**Parameters:**


* **init\_size** - The initial size 



### function <a id="1ad9d7da49e77a5db9c80c256845b26894" href="#1ad9d7da49e77a5db9c80c256845b26894">skip</a>

```cpp
bool eosio::datastream< size_t >::skip (
    size_t s
)
```

Increase the size by s. 

Increment the size by s. This behaves the same as **[write( const char\* ,size\_t s )](classeosio_1_1datastream_3_01size__t_01_4_a3c2ca245c9f33549505ea7886a1d3ecf.md#1a3c2ca245c9f33549505ea7886a1d3ecf)**.


**Parameters:**


* **s** - The amount of size to increase 



**Returns:**

true 




### function <a id="1a3c2ca245c9f33549505ea7886a1d3ecf" href="#1a3c2ca245c9f33549505ea7886a1d3ecf">write</a>

```cpp
bool eosio::datastream< size_t >::write (
    const char * ,
    size_t s
)
```

Increase the size by s. 

Increment the size by s. This behaves the same as **[skip( size\_t s )](classeosio_1_1datastream_3_01size__t_01_4_ad9d7da49e77a5db9c80c256845b26894.md#1ad9d7da49e77a5db9c80c256845b26894)**


**Parameters:**


* **s** - The amount of size to increase 



**Returns:**

true 




### function <a id="1a4a1a95751dafd76aea580d80c84bf8bb" href="#1a4a1a95751dafd76aea580d80c84bf8bb">put</a>

```cpp
bool eosio::datastream< size_t >::put (
    char 
)
```

Increase the size by one. 

Increment the size by one


**Returns:**

true 




### function <a id="1a46f1fc6f14d7e79c2181f4e3a2ae5b12" href="#1a46f1fc6f14d7e79c2181f4e3a2ae5b12">valid</a>

```cpp
bool eosio::datastream< size_t >::valid () const
```

Check validity. 

Check validity. It's always valid


**Returns:**

true 




### function <a id="1ac4720cb82d256a41f27bbf389b37790b" href="#1ac4720cb82d256a41f27bbf389b37790b">seekp</a>

```cpp
bool eosio::datastream< size_t >::seekp (
    size_t p
)
```

Set new size. 

Set new size


**Parameters:**


* **p** - The new size 



**Returns:**

true 




### function <a id="1aadcdabf95d8b142bd9b43bbcdc4e0d9f" href="#1aadcdabf95d8b142bd9b43bbcdc4e0d9f">tellp</a>

```cpp
size_t eosio::datastream< size_t >::tellp () const
```

Get the size. 

Get the size


**Returns:**

size\_t - The size 




### function <a id="1a345f36fa8492917662d6185db631042d" href="#1a345f36fa8492917662d6185db631042d">remaining</a>

```cpp
size_t eosio::datastream< size_t >::remaining () const
```

Always returns 0. 

Always returns 0


**Returns:**

size\_t - 0 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/datastream.hpp`
