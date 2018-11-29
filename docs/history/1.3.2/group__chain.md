---
search:
    keywords: ['chain', 'get_active_producers']
---

# group chain

Defines C API for querying internal chain state. 
## Functions Documentation

### function <a id="gafb1878d4d55d275a1729f44ecc72848a" href="#gafb1878d4d55d275a1729f44ecc72848a">get\_active\_producers</a>

```cpp
uint32_t chain::get_active_producers (
    capi_name * producers,
    uint32_t datalen
)
```


Gets the set of active producers.


**Parameters:**


* **producers** - Pointer to a buffer of account names 
* **datalen** - Byte length of buffer, when passed 0 will return the size required to store full output.



**Returns:**

uint32\_t - Number of bytes actually populated 




**Precondition:**

`producers` is a pointer to a range of memory at least `datalen` bytes long 




**Post**

the passed in `producers` pointer gets the array of active producers.


Example:

```cpp
capi_name producers[21];
uint32_t bytes_populated = get_active_producers(producers, sizeof(capi_name)*21);
```

 

