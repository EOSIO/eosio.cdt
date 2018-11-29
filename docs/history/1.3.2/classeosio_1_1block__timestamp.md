---
search:
    keywords: ['eosio::block_timestamp', 'slot', 'block_interval_ms', 'block_timestamp_epoch', 'block_timestamp', 'block_timestamp', 'block_timestamp', 'next', 'to_time_point', 'operator time_point', 'operator=', 'operator>', 'operator>=', 'operator<', 'operator<=', 'operator==', 'operator!=', 'maximum', 'min']
---

# class eosio::block\_timestamp

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**block\_timestamp**](classeosio_1_1block__timestamp.md)


## Public Attributes

|Type|Name|
|-----|-----|
|uint32\_t|[**slot**](classeosio_1_1block__timestamp_a80ffae898b495e10882b9b0c516acbbd.md#1a80ffae898b495e10882b9b0c516acbbd)|


## Public Static Attributes

|Type|Name|
|-----|-----|
|static constexpr int32\_t|[**block\_interval\_ms**](classeosio_1_1block__timestamp_afd46fd13c5b26916f1ee04f9e6d653cf.md#1afd46fd13c5b26916f1ee04f9e6d653cf)|
|static constexpr int64\_t|[**block\_timestamp\_epoch**](classeosio_1_1block__timestamp_a4565cdbb18f05a1f6ca75ea72a1c2c25.md#1a4565cdbb18f05a1f6ca75ea72a1c2c25)|


## Public Functions

|Type|Name|
|-----|-----|
||[**block\_timestamp**](classeosio_1_1block__timestamp_aa719feec777894d3612305fcdf579d1e.md#1aa719feec777894d3612305fcdf579d1e) (uint32\_t s = 0) |
||[**block\_timestamp**](classeosio_1_1block__timestamp_a72934b4290a476785f852983608263b0.md#1a72934b4290a476785f852983608263b0) (const **[time\_point](classeosio_1_1time__point.md)** & t) |
||[**block\_timestamp**](classeosio_1_1block__timestamp_aae1af00ad48d215fbd72d3e2339655bf.md#1aae1af00ad48d215fbd72d3e2339655bf) (const **[time\_point\_sec](classeosio_1_1time__point__sec.md)** & t) |
|**[block\_timestamp](classeosio_1_1block__timestamp.md)**|[**next**](classeosio_1_1block__timestamp_a07825f36097aa846aecaa221239874d6.md#1a07825f36097aa846aecaa221239874d6) () const |
|**[time\_point](classeosio_1_1time__point.md)**|[**to\_time\_point**](classeosio_1_1block__timestamp_ac645cbc95a573b9ddc33439bf7953e83.md#1ac645cbc95a573b9ddc33439bf7953e83) () const |
||[**operator time\_point**](classeosio_1_1block__timestamp_abc0874625ff948fb8f23dd41644aee23.md#1abc0874625ff948fb8f23dd41644aee23) () const |
|void|[**operator=**](classeosio_1_1block__timestamp_acf6395d8b163ec1b0c87e81758ca1c4f.md#1acf6395d8b163ec1b0c87e81758ca1c4f) (const **[time\_point](classeosio_1_1time__point.md)** & t) |
|bool|[**operator>**](classeosio_1_1block__timestamp_a2fd610a1b45ad9ca6f51625e7236b9d2.md#1a2fd610a1b45ad9ca6f51625e7236b9d2) (const **[block\_timestamp](classeosio_1_1block__timestamp.md)** & t) const |
|bool|[**operator>=**](classeosio_1_1block__timestamp_af7b8bd34ae4576f6829402d4f09e6da7.md#1af7b8bd34ae4576f6829402d4f09e6da7) (const **[block\_timestamp](classeosio_1_1block__timestamp.md)** & t) const |
|bool|[**operator<**](classeosio_1_1block__timestamp_aa8ab75a684478d9879e4bf2f0ff63432.md#1aa8ab75a684478d9879e4bf2f0ff63432) (const **[block\_timestamp](classeosio_1_1block__timestamp.md)** & t) const |
|bool|[**operator<=**](classeosio_1_1block__timestamp_a937c773b3117b920a737b7349b630c53.md#1a937c773b3117b920a737b7349b630c53) (const **[block\_timestamp](classeosio_1_1block__timestamp.md)** & t) const |
|bool|[**operator==**](classeosio_1_1block__timestamp_afe243beb6b6d4c34a7f8a5620b641ead.md#1afe243beb6b6d4c34a7f8a5620b641ead) (const **[block\_timestamp](classeosio_1_1block__timestamp.md)** & t) const |
|bool|[**operator!=**](classeosio_1_1block__timestamp_a4c83752575fffc12ddac3186080ca885.md#1a4c83752575fffc12ddac3186080ca885) (const **[block\_timestamp](classeosio_1_1block__timestamp.md)** & t) const |


## Public Static Functions

|Type|Name|
|-----|-----|
|static **[block\_timestamp](classeosio_1_1block__timestamp.md)**|[**maximum**](classeosio_1_1block__timestamp_a30ae0cff751d3f0ff01010f4a4f6d052.md#1a30ae0cff751d3f0ff01010f4a4f6d052) () |
|static **[block\_timestamp](classeosio_1_1block__timestamp.md)**|[**min**](classeosio_1_1block__timestamp_a97cc3a05e64223e6144fde0ccfac1abd.md#1a97cc3a05e64223e6144fde0ccfac1abd) () |


## Detailed Description

This class is used in the block headers to represent the block time It is a parameterised class that takes an Epoch in milliseconds and and an interval in milliseconds and computes the number of slots. 
## Public Attributes Documentation

### variable <a id="1a80ffae898b495e10882b9b0c516acbbd" href="#1a80ffae898b495e10882b9b0c516acbbd">slot</a>

```cpp
uint32_t eosio::block_timestamp::slot;
```



## Public Static Attributes Documentation

### variable <a id="1afd46fd13c5b26916f1ee04f9e6d653cf" href="#1afd46fd13c5b26916f1ee04f9e6d653cf">block\_interval\_ms</a>

```cpp
constexpr int32_t eosio::block_timestamp::block_interval_ms;
```



### variable <a id="1a4565cdbb18f05a1f6ca75ea72a1c2c25" href="#1a4565cdbb18f05a1f6ca75ea72a1c2c25">block\_timestamp\_epoch</a>

```cpp
constexpr int64_t eosio::block_timestamp::block_timestamp_epoch;
```



## Public Functions Documentation

### function <a id="1aa719feec777894d3612305fcdf579d1e" href="#1aa719feec777894d3612305fcdf579d1e">block\_timestamp (1/3)</a>

```cpp
explicit eosio::block_timestamp::block_timestamp (
    uint32_t s = 0
)
```



### function <a id="1a72934b4290a476785f852983608263b0" href="#1a72934b4290a476785f852983608263b0">block\_timestamp (2/3)</a>

```cpp
eosio::block_timestamp::block_timestamp (
    const time_point & t
)
```



### function <a id="1aae1af00ad48d215fbd72d3e2339655bf" href="#1aae1af00ad48d215fbd72d3e2339655bf">block\_timestamp (3/3)</a>

```cpp
eosio::block_timestamp::block_timestamp (
    const time_point_sec & t
)
```



### function <a id="1a07825f36097aa846aecaa221239874d6" href="#1a07825f36097aa846aecaa221239874d6">next</a>

```cpp
block_timestamp eosio::block_timestamp::next () const
```



### function <a id="1ac645cbc95a573b9ddc33439bf7953e83" href="#1ac645cbc95a573b9ddc33439bf7953e83">to\_time\_point</a>

```cpp
time_point eosio::block_timestamp::to_time_point () const
```



### function <a id="1abc0874625ff948fb8f23dd41644aee23" href="#1abc0874625ff948fb8f23dd41644aee23">operator time\_point</a>

```cpp
eosio::block_timestamp::operator time_point () const
```



### function <a id="1acf6395d8b163ec1b0c87e81758ca1c4f" href="#1acf6395d8b163ec1b0c87e81758ca1c4f">operator=</a>

```cpp
void eosio::block_timestamp::operator= (
    const time_point & t
)
```



### function <a id="1a2fd610a1b45ad9ca6f51625e7236b9d2" href="#1a2fd610a1b45ad9ca6f51625e7236b9d2">operator></a>

```cpp
bool eosio::block_timestamp::operator> (
    const block_timestamp & t
) const
```



### function <a id="1af7b8bd34ae4576f6829402d4f09e6da7" href="#1af7b8bd34ae4576f6829402d4f09e6da7">operator>=</a>

```cpp
bool eosio::block_timestamp::operator>= (
    const block_timestamp & t
) const
```



### function <a id="1aa8ab75a684478d9879e4bf2f0ff63432" href="#1aa8ab75a684478d9879e4bf2f0ff63432">operator<</a>

```cpp
bool eosio::block_timestamp::operator< (
    const block_timestamp & t
) const
```



### function <a id="1a937c773b3117b920a737b7349b630c53" href="#1a937c773b3117b920a737b7349b630c53">operator<=</a>

```cpp
bool eosio::block_timestamp::operator<= (
    const block_timestamp & t
) const
```



### function <a id="1afe243beb6b6d4c34a7f8a5620b641ead" href="#1afe243beb6b6d4c34a7f8a5620b641ead">operator==</a>

```cpp
bool eosio::block_timestamp::operator== (
    const block_timestamp & t
) const
```



### function <a id="1a4c83752575fffc12ddac3186080ca885" href="#1a4c83752575fffc12ddac3186080ca885">operator!=</a>

```cpp
bool eosio::block_timestamp::operator!= (
    const block_timestamp & t
) const
```



## Public Static Functions Documentation

### function <a id="1a30ae0cff751d3f0ff01010f4a4f6d052" href="#1a30ae0cff751d3f0ff01010f4a4f6d052">maximum</a>

```cpp
static block_timestamp eosio::block_timestamp::maximum ()
```



### function <a id="1a97cc3a05e64223e6144fde0ccfac1abd" href="#1a97cc3a05e64223e6144fde0ccfac1abd">min</a>

```cpp
static block_timestamp eosio::block_timestamp::min ()
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/time.hpp`
