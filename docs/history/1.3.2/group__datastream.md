---
search:
    keywords: ['datastream', 'unpack', 'unpack', 'pack_size', 'pack']
---

# group datastream

## Detailed Description

Defines data stream for reading and writing data in the form of bytes 
## Functions Documentation

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




