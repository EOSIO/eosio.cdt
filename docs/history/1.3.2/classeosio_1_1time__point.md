---
search:
    keywords: ['eosio::time_point', 'elapsed', 'time_point', 'time_since_epoch', 'sec_since_epoch', 'operator>', 'operator>=', 'operator<', 'operator<=', 'operator==', 'operator!=', 'operator+=', 'operator-=', 'operator+', 'operator+', 'operator-', 'operator-']
---

# class eosio::time\_point

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**time\_point**](classeosio_1_1time__point.md)


## Public Attributes

|Type|Name|
|-----|-----|
|**[microseconds](classeosio_1_1microseconds.md)**|[**elapsed**](classeosio_1_1time__point_ab27eaf8e2d1f09470fbfb9332115e22a.md#1ab27eaf8e2d1f09470fbfb9332115e22a)|


## Public Functions

|Type|Name|
|-----|-----|
||[**time\_point**](classeosio_1_1time__point_a307b17c8475291a97892a1f9cd76fa36.md#1a307b17c8475291a97892a1f9cd76fa36) (**[microseconds](classeosio_1_1microseconds.md)** e = **[microseconds](classeosio_1_1microseconds.md)**()) |
|const **[microseconds](classeosio_1_1microseconds.md)** &|[**time\_since\_epoch**](classeosio_1_1time__point_a0ace41dcb1cd4579bf9df284b34435ae.md#1a0ace41dcb1cd4579bf9df284b34435ae) () const |
|uint32\_t|[**sec\_since\_epoch**](classeosio_1_1time__point_aedda68d4c06c12b9dcf27a8964f2c6a9.md#1aedda68d4c06c12b9dcf27a8964f2c6a9) () const |
|bool|[**operator>**](classeosio_1_1time__point_ae195795ab46c28061aba38fb7f373796.md#1ae195795ab46c28061aba38fb7f373796) (const **[time\_point](classeosio_1_1time__point.md)** & t) const |
|bool|[**operator>=**](classeosio_1_1time__point_acdbdefb8449e14bb0eadbfbc01a9b00b.md#1acdbdefb8449e14bb0eadbfbc01a9b00b) (const **[time\_point](classeosio_1_1time__point.md)** & t) const |
|bool|[**operator<**](classeosio_1_1time__point_a908072e64586f7d21a897090da7f8fa0.md#1a908072e64586f7d21a897090da7f8fa0) (const **[time\_point](classeosio_1_1time__point.md)** & t) const |
|bool|[**operator<=**](classeosio_1_1time__point_ab33b62e5d591dc6285141020de6cad0d.md#1ab33b62e5d591dc6285141020de6cad0d) (const **[time\_point](classeosio_1_1time__point.md)** & t) const |
|bool|[**operator==**](classeosio_1_1time__point_a86d3524598f9f0f00d1f4f2440684350.md#1a86d3524598f9f0f00d1f4f2440684350) (const **[time\_point](classeosio_1_1time__point.md)** & t) const |
|bool|[**operator!=**](classeosio_1_1time__point_a54d756d8704d82cf9416a14243f755f9.md#1a54d756d8704d82cf9416a14243f755f9) (const **[time\_point](classeosio_1_1time__point.md)** & t) const |
|**[time\_point](classeosio_1_1time__point.md)** &|[**operator+=**](classeosio_1_1time__point_a0ac241c9e0ed72797f7acec49db681ea.md#1a0ac241c9e0ed72797f7acec49db681ea) (const **[microseconds](classeosio_1_1microseconds.md)** & m) |
|**[time\_point](classeosio_1_1time__point.md)** &|[**operator-=**](classeosio_1_1time__point_a124d983527e88de1dedee84fe4befd6e.md#1a124d983527e88de1dedee84fe4befd6e) (const **[microseconds](classeosio_1_1microseconds.md)** & m) |
|**[time\_point](classeosio_1_1time__point.md)**|[**operator+**](classeosio_1_1time__point_a361fac9d2b22ad32d562c308ab32f204.md#1a361fac9d2b22ad32d562c308ab32f204) (const **[microseconds](classeosio_1_1microseconds.md)** & m) const |
|**[time\_point](classeosio_1_1time__point.md)**|[**operator+**](classeosio_1_1time__point_aca0f3dc8b64085ad2e9f2c2cb98b5697.md#1aca0f3dc8b64085ad2e9f2c2cb98b5697) (const **[time\_point](classeosio_1_1time__point.md)** & m) const |
|**[time\_point](classeosio_1_1time__point.md)**|[**operator-**](classeosio_1_1time__point_a3b4cc7e277ea59460b963c4d1b026fea.md#1a3b4cc7e277ea59460b963c4d1b026fea) (const **[microseconds](classeosio_1_1microseconds.md)** & m) const |
|**[microseconds](classeosio_1_1microseconds.md)**|[**operator-**](classeosio_1_1time__point_a8fa1704a4bf3732790bee43925a4f192.md#1a8fa1704a4bf3732790bee43925a4f192) (const **[time\_point](classeosio_1_1time__point.md)** & m) const |


## Public Attributes Documentation

### variable <a id="1ab27eaf8e2d1f09470fbfb9332115e22a" href="#1ab27eaf8e2d1f09470fbfb9332115e22a">elapsed</a>

```cpp
microseconds eosio::time_point::elapsed;
```



## Public Functions Documentation

### function <a id="1a307b17c8475291a97892a1f9cd76fa36" href="#1a307b17c8475291a97892a1f9cd76fa36">time\_point</a>

```cpp
explicit eosio::time_point::time_point (
    microseconds e = microseconds()
)
```



### function <a id="1a0ace41dcb1cd4579bf9df284b34435ae" href="#1a0ace41dcb1cd4579bf9df284b34435ae">time\_since\_epoch</a>

```cpp
const microseconds & eosio::time_point::time_since_epoch () const
```



### function <a id="1aedda68d4c06c12b9dcf27a8964f2c6a9" href="#1aedda68d4c06c12b9dcf27a8964f2c6a9">sec\_since\_epoch</a>

```cpp
uint32_t eosio::time_point::sec_since_epoch () const
```



### function <a id="1ae195795ab46c28061aba38fb7f373796" href="#1ae195795ab46c28061aba38fb7f373796">operator></a>

```cpp
bool eosio::time_point::operator> (
    const time_point & t
) const
```



### function <a id="1acdbdefb8449e14bb0eadbfbc01a9b00b" href="#1acdbdefb8449e14bb0eadbfbc01a9b00b">operator>=</a>

```cpp
bool eosio::time_point::operator>= (
    const time_point & t
) const
```



### function <a id="1a908072e64586f7d21a897090da7f8fa0" href="#1a908072e64586f7d21a897090da7f8fa0">operator<</a>

```cpp
bool eosio::time_point::operator< (
    const time_point & t
) const
```



### function <a id="1ab33b62e5d591dc6285141020de6cad0d" href="#1ab33b62e5d591dc6285141020de6cad0d">operator<=</a>

```cpp
bool eosio::time_point::operator<= (
    const time_point & t
) const
```



### function <a id="1a86d3524598f9f0f00d1f4f2440684350" href="#1a86d3524598f9f0f00d1f4f2440684350">operator==</a>

```cpp
bool eosio::time_point::operator== (
    const time_point & t
) const
```



### function <a id="1a54d756d8704d82cf9416a14243f755f9" href="#1a54d756d8704d82cf9416a14243f755f9">operator!=</a>

```cpp
bool eosio::time_point::operator!= (
    const time_point & t
) const
```



### function <a id="1a0ac241c9e0ed72797f7acec49db681ea" href="#1a0ac241c9e0ed72797f7acec49db681ea">operator+=</a>

```cpp
time_point & eosio::time_point::operator+= (
    const microseconds & m
)
```



### function <a id="1a124d983527e88de1dedee84fe4befd6e" href="#1a124d983527e88de1dedee84fe4befd6e">operator-=</a>

```cpp
time_point & eosio::time_point::operator-= (
    const microseconds & m
)
```



### function <a id="1a361fac9d2b22ad32d562c308ab32f204" href="#1a361fac9d2b22ad32d562c308ab32f204">operator+ (1/2)</a>

```cpp
time_point eosio::time_point::operator+ (
    const microseconds & m
) const
```



### function <a id="1aca0f3dc8b64085ad2e9f2c2cb98b5697" href="#1aca0f3dc8b64085ad2e9f2c2cb98b5697">operator+ (2/2)</a>

```cpp
time_point eosio::time_point::operator+ (
    const time_point & m
) const
```



### function <a id="1a3b4cc7e277ea59460b963c4d1b026fea" href="#1a3b4cc7e277ea59460b963c4d1b026fea">operator- (1/2)</a>

```cpp
time_point eosio::time_point::operator- (
    const microseconds & m
) const
```



### function <a id="1a8fa1704a4bf3732790bee43925a4f192" href="#1a8fa1704a4bf3732790bee43925a4f192">operator- (2/2)</a>

```cpp
microseconds eosio::time_point::operator- (
    const time_point & m
) const
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/time.hpp`
