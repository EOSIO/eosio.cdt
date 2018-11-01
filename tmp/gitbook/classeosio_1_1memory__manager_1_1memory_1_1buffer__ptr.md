---
search:
    keywords: ['eosio::memory_manager::memory::buffer_ptr', 'buffer_ptr', 'buffer_ptr', 'size', 'next_ptr', 'size', 'end', 'ptr', 'mark_alloc', 'mark_free', 'is_alloc', 'merge_contiguous_if_available', 'merge_contiguous']
---

# class eosio::memory\_manager::memory::buffer\_ptr

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**memory\_manager**](classeosio_1_1memory__manager.md) **::** [**memory**](classeosio_1_1memory__manager_1_1memory.md) **::** [**buffer\_ptr**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr.md)


## Public Functions

|Type|Name|
|-----|-----|
||[**buffer\_ptr**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_ac5456120e436769c2b2942f70e4ac25f.md#1ac5456120e436769c2b2942f70e4ac25f) (void \* ptr, const char \*const heap\_end) |
||[**buffer\_ptr**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_a1ed3dec5960241e58f5596f504d48564.md#1a1ed3dec5960241e58f5596f504d48564) (void \* ptr, uint32\_t buff\_size, const char \*const heap\_end) |
|uint32\_t|[**size**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_a6d3faad0f9dd8b428e08003db54a4534.md#1a6d3faad0f9dd8b428e08003db54a4534) () const |
|char \*|[**next\_ptr**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_a654082fb0667d60c52cee9ba9c37bde2.md#1a654082fb0667d60c52cee9ba9c37bde2) () const |
|void|[**size**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_a92c13d6fcb75e4471275d8c480d29d4e.md#1a92c13d6fcb75e4471275d8c480d29d4e) (uint32\_t val) |
|char \*|[**end**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_a5aee1ccf724a3b6b1085707f8684b961.md#1a5aee1ccf724a3b6b1085707f8684b961) () const |
|char \*|[**ptr**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_a74c2399ffda524cbc52bce8e6d7a10d2.md#1a74c2399ffda524cbc52bce8e6d7a10d2) () const |
|void|[**mark\_alloc**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_ac67096ce1051b46b23d105dda3a0beab.md#1ac67096ce1051b46b23d105dda3a0beab) () |
|void|[**mark\_free**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_ab175f38d0934fa8ee4781a18dffe9bd8.md#1ab175f38d0934fa8ee4781a18dffe9bd8) () |
|bool|[**is\_alloc**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_a1a30813caab9470accb4d39d4d61b170.md#1a1a30813caab9470accb4d39d4d61b170) () const |
|bool|[**merge\_contiguous\_if\_available**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_af70f3f1e34ef94c65a74c95397b22a5a.md#1af70f3f1e34ef94c65a74c95397b22a5a) (uint32\_t needed\_size) |
|bool|[**merge\_contiguous**](classeosio_1_1memory__manager_1_1memory_1_1buffer__ptr_af15531da7d6fa587d88a23a4beb84148.md#1af15531da7d6fa587d88a23a4beb84148) (uint32\_t needed\_size) |


## Public Functions Documentation

### function <a id="1ac5456120e436769c2b2942f70e4ac25f" href="#1ac5456120e436769c2b2942f70e4ac25f">buffer\_ptr (1/2)</a>

```cpp
eosio::memory_manager::memory::buffer_ptr::buffer_ptr (
    void * ptr,
    const char *const heap_end
)
```



### function <a id="1a1ed3dec5960241e58f5596f504d48564" href="#1a1ed3dec5960241e58f5596f504d48564">buffer\_ptr (2/2)</a>

```cpp
eosio::memory_manager::memory::buffer_ptr::buffer_ptr (
    void * ptr,
    uint32_t buff_size,
    const char *const heap_end
)
```



### function <a id="1a6d3faad0f9dd8b428e08003db54a4534" href="#1a6d3faad0f9dd8b428e08003db54a4534">size (1/2)</a>

```cpp
uint32_t eosio::memory_manager::memory::buffer_ptr::size () const
```



### function <a id="1a654082fb0667d60c52cee9ba9c37bde2" href="#1a654082fb0667d60c52cee9ba9c37bde2">next\_ptr</a>

```cpp
char * eosio::memory_manager::memory::buffer_ptr::next_ptr () const
```



### function <a id="1a92c13d6fcb75e4471275d8c480d29d4e" href="#1a92c13d6fcb75e4471275d8c480d29d4e">size (2/2)</a>

```cpp
void eosio::memory_manager::memory::buffer_ptr::size (
    uint32_t val
)
```



### function <a id="1a5aee1ccf724a3b6b1085707f8684b961" href="#1a5aee1ccf724a3b6b1085707f8684b961">end</a>

```cpp
char * eosio::memory_manager::memory::buffer_ptr::end () const
```



### function <a id="1a74c2399ffda524cbc52bce8e6d7a10d2" href="#1a74c2399ffda524cbc52bce8e6d7a10d2">ptr</a>

```cpp
char * eosio::memory_manager::memory::buffer_ptr::ptr () const
```



### function <a id="1ac67096ce1051b46b23d105dda3a0beab" href="#1ac67096ce1051b46b23d105dda3a0beab">mark\_alloc</a>

```cpp
void eosio::memory_manager::memory::buffer_ptr::mark_alloc ()
```



### function <a id="1ab175f38d0934fa8ee4781a18dffe9bd8" href="#1ab175f38d0934fa8ee4781a18dffe9bd8">mark\_free</a>

```cpp
void eosio::memory_manager::memory::buffer_ptr::mark_free ()
```



### function <a id="1a1a30813caab9470accb4d39d4d61b170" href="#1a1a30813caab9470accb4d39d4d61b170">is\_alloc</a>

```cpp
bool eosio::memory_manager::memory::buffer_ptr::is_alloc () const
```



### function <a id="1af70f3f1e34ef94c65a74c95397b22a5a" href="#1af70f3f1e34ef94c65a74c95397b22a5a">merge\_contiguous\_if\_available</a>

```cpp
bool eosio::memory_manager::memory::buffer_ptr::merge_contiguous_if_available (
    uint32_t needed_size
)
```



### function <a id="1af15531da7d6fa587d88a23a4beb84148" href="#1af15531da7d6fa587d88a23a4beb84148">merge\_contiguous (1/2)</a>

```cpp
bool eosio::memory_manager::memory::buffer_ptr::merge_contiguous (
    uint32_t needed_size
)
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/eosiolib.cpp`
