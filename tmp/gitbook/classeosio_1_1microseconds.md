---
search:
    keywords: ['eosio::microseconds', '_count', 'time_point', 'operator+', 'operator-', 'operator>', 'operator>=', 'operator<', 'operator<=', 'microseconds', 'operator==', 'operator!=', 'operator+=', 'operator-=', 'count', 'to_seconds', 'maximum']
---

# class eosio::microseconds

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**microseconds**](classeosio_1_1microseconds.md)


## Public Attributes

|Type|Name|
|-----|-----|
|int64\_t|[**\_count**](classeosio_1_1microseconds_af2faf1e08a77aafe6485d0e62813354c.md#1af2faf1e08a77aafe6485d0e62813354c)|


## Friends

|Type|Name|
|-----|-----|
|friend class|[**time\_point**](classeosio_1_1microseconds_aaba1d7b327cc33fcbaf19e870d0c1733.md#1aaba1d7b327cc33fcbaf19e870d0c1733)|
|friend **[microseconds](classeosio_1_1microseconds.md)**|[**operator+**](classeosio_1_1microseconds_a4aefc6bb1acd38167eb501722616714d.md#1a4aefc6bb1acd38167eb501722616714d)|
|friend **[microseconds](classeosio_1_1microseconds.md)**|[**operator-**](classeosio_1_1microseconds_ada590d04fee1d36be15cdda38b581531.md#1ada590d04fee1d36be15cdda38b581531)|
|friend bool|[**operator>**](classeosio_1_1microseconds_afce366cbc2e650931e2d1eb856c1339e.md#1afce366cbc2e650931e2d1eb856c1339e)|
|friend bool|[**operator>=**](classeosio_1_1microseconds_ad4531296ec947832cc4b3f5dfbc9fc0b.md#1ad4531296ec947832cc4b3f5dfbc9fc0b)|
|friend bool|[**operator<**](classeosio_1_1microseconds_aa1877d2e6b18621502415bb755c51108.md#1aa1877d2e6b18621502415bb755c51108)|
|friend bool|[**operator<=**](classeosio_1_1microseconds_a01d844cd1e5a8b2108b564ac3b1a8ed4.md#1a01d844cd1e5a8b2108b564ac3b1a8ed4)|


## Public Functions

|Type|Name|
|-----|-----|
||[**microseconds**](classeosio_1_1microseconds_a59aa821c27e2073ede1ddf759e26317f.md#1a59aa821c27e2073ede1ddf759e26317f) (int64\_t c = 0) |
|bool|[**operator==**](classeosio_1_1microseconds_aecd2e119c3bd9254a29dd2667da37ff0.md#1aecd2e119c3bd9254a29dd2667da37ff0) (const **[microseconds](classeosio_1_1microseconds.md)** & c) const |
|bool|[**operator!=**](classeosio_1_1microseconds_a2c2cb714e79a3f4d984b068702496533.md#1a2c2cb714e79a3f4d984b068702496533) (const **[microseconds](classeosio_1_1microseconds.md)** & c) const |
|**[microseconds](classeosio_1_1microseconds.md)** &|[**operator+=**](classeosio_1_1microseconds_ae23f9854f69a44225d8f96092cdc032e.md#1ae23f9854f69a44225d8f96092cdc032e) (const **[microseconds](classeosio_1_1microseconds.md)** & c) |
|**[microseconds](classeosio_1_1microseconds.md)** &|[**operator-=**](classeosio_1_1microseconds_a91086401e39a026d06ab5d2a41a27f22.md#1a91086401e39a026d06ab5d2a41a27f22) (const **[microseconds](classeosio_1_1microseconds.md)** & c) |
|int64\_t|[**count**](classeosio_1_1microseconds_a0d847f20b3d17f6743e39773ea0431b2.md#1a0d847f20b3d17f6743e39773ea0431b2) () const |
|int64\_t|[**to\_seconds**](classeosio_1_1microseconds_aa9396d5bd773c4169ff6aca7344988a6.md#1aa9396d5bd773c4169ff6aca7344988a6) () const |


## Public Static Functions

|Type|Name|
|-----|-----|
|static **[microseconds](classeosio_1_1microseconds.md)**|[**maximum**](classeosio_1_1microseconds_a9b6c52f413afce012f9bfebe66b4b6a1.md#1a9b6c52f413afce012f9bfebe66b4b6a1) () |


## Public Attributes Documentation

### variable <a id="1af2faf1e08a77aafe6485d0e62813354c" href="#1af2faf1e08a77aafe6485d0e62813354c">\_count</a>

```cpp
int64_t eosio::microseconds::_count;
```



## Friends Documentation

### friend <a id="1aaba1d7b327cc33fcbaf19e870d0c1733" href="#1aaba1d7b327cc33fcbaf19e870d0c1733">time\_point</a>

```cpp
friend class time_point;
```



### friend <a id="1a4aefc6bb1acd38167eb501722616714d" href="#1a4aefc6bb1acd38167eb501722616714d">operator+</a>

```cpp
friend microseconds eosio::microseconds::operator+ (
    const microseconds & l,
    const microseconds & r
)
```



### friend <a id="1ada590d04fee1d36be15cdda38b581531" href="#1ada590d04fee1d36be15cdda38b581531">operator-</a>

```cpp
friend microseconds eosio::microseconds::operator- (
    const microseconds & l,
    const microseconds & r
)
```



### friend <a id="1afce366cbc2e650931e2d1eb856c1339e" href="#1afce366cbc2e650931e2d1eb856c1339e">operator></a>

```cpp
friend bool eosio::microseconds::operator> (
    const microseconds & a,
    const microseconds & b
)
```



### friend <a id="1ad4531296ec947832cc4b3f5dfbc9fc0b" href="#1ad4531296ec947832cc4b3f5dfbc9fc0b">operator>=</a>

```cpp
friend bool eosio::microseconds::operator>= (
    const microseconds & a,
    const microseconds & b
)
```



### friend <a id="1aa1877d2e6b18621502415bb755c51108" href="#1aa1877d2e6b18621502415bb755c51108">operator<</a>

```cpp
friend bool eosio::microseconds::operator< (
    const microseconds & a,
    const microseconds & b
)
```



### friend <a id="1a01d844cd1e5a8b2108b564ac3b1a8ed4" href="#1a01d844cd1e5a8b2108b564ac3b1a8ed4">operator<=</a>

```cpp
friend bool eosio::microseconds::operator<= (
    const microseconds & a,
    const microseconds & b
)
```



## Public Functions Documentation

### function <a id="1a59aa821c27e2073ede1ddf759e26317f" href="#1a59aa821c27e2073ede1ddf759e26317f">microseconds</a>

```cpp
explicit eosio::microseconds::microseconds (
    int64_t c = 0
)
```



### function <a id="1aecd2e119c3bd9254a29dd2667da37ff0" href="#1aecd2e119c3bd9254a29dd2667da37ff0">operator==</a>

```cpp
bool eosio::microseconds::operator== (
    const microseconds & c
) const
```



### function <a id="1a2c2cb714e79a3f4d984b068702496533" href="#1a2c2cb714e79a3f4d984b068702496533">operator!=</a>

```cpp
bool eosio::microseconds::operator!= (
    const microseconds & c
) const
```



### function <a id="1ae23f9854f69a44225d8f96092cdc032e" href="#1ae23f9854f69a44225d8f96092cdc032e">operator+=</a>

```cpp
microseconds & eosio::microseconds::operator+= (
    const microseconds & c
)
```



### function <a id="1a91086401e39a026d06ab5d2a41a27f22" href="#1a91086401e39a026d06ab5d2a41a27f22">operator-=</a>

```cpp
microseconds & eosio::microseconds::operator-= (
    const microseconds & c
)
```



### function <a id="1a0d847f20b3d17f6743e39773ea0431b2" href="#1a0d847f20b3d17f6743e39773ea0431b2">count</a>

```cpp
int64_t eosio::microseconds::count () const
```



### function <a id="1aa9396d5bd773c4169ff6aca7344988a6" href="#1aa9396d5bd773c4169ff6aca7344988a6">to\_seconds</a>

```cpp
int64_t eosio::microseconds::to_seconds () const
```



## Public Static Functions Documentation

### function <a id="1a9b6c52f413afce012f9bfebe66b4b6a1" href="#1a9b6c52f413afce012f9bfebe66b4b6a1">maximum</a>

```cpp
static microseconds eosio::microseconds::maximum ()
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/time.hpp`
