---
search:
    keywords: ['eosio::asset', 'amount', 'symbol', 'max_amount', 'asset', 'asset', 'is_amount_within_range', 'is_valid', 'set_amount', 'operator-', 'operator-=', 'operator+=', 'operator*=', 'operator/=', 'print', 'operator+', 'operator-', 'operator*', 'operator*', 'operator/', 'operator/', 'operator==', 'operator!=', 'operator<', 'operator<=', 'operator>', 'operator>=']
---

# struct eosio::asset

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**asset**](structeosio_1_1asset.md)


## Public Attributes

|Type|Name|
|-----|-----|
|int64\_t|[**amount**](structeosio_1_1asset_ac76c16a58851550706c6709698ed7255.md#1ac76c16a58851550706c6709698ed7255)|
|**[symbol](classeosio_1_1symbol.md)**|[**symbol**](structeosio_1_1asset_a374520d98a22f5a89a26025154944756.md#1a374520d98a22f5a89a26025154944756)|


## Public Static Attributes

|Type|Name|
|-----|-----|
|static constexpr int64\_t|[**max\_amount**](structeosio_1_1asset_a478eb6a3bb302652748acfcf85738f5f.md#1a478eb6a3bb302652748acfcf85738f5f)|


## Public Functions

|Type|Name|
|-----|-----|
||[**asset**](structeosio_1_1asset_abdde8d4b53c1b8f1027ab6cb2b13b845.md#1abdde8d4b53c1b8f1027ab6cb2b13b845) () |
||[**asset**](structeosio_1_1asset_abb4cf1f370bb101c7d61910ff45d6b55.md#1abb4cf1f370bb101c7d61910ff45d6b55) (int64\_t a, class **[symbol](classeosio_1_1symbol.md)** s) |
|bool|[**is\_amount\_within\_range**](structeosio_1_1asset_a815b58378c8849fbb2a68f288edf5f31.md#1a815b58378c8849fbb2a68f288edf5f31) () const |
|bool|[**is\_valid**](structeosio_1_1asset_aa2e8739bf534ea99199d121143ca7a0a.md#1aa2e8739bf534ea99199d121143ca7a0a) () const |
|void|[**set\_amount**](structeosio_1_1asset_a0b6c8ba10eedc0129364fc910e840d7f.md#1a0b6c8ba10eedc0129364fc910e840d7f) (int64\_t a) |
|**[asset](structeosio_1_1asset.md)**|[**operator-**](structeosio_1_1asset_a16bee92e23903707b37a1e7c91e1a4f1.md#1a16bee92e23903707b37a1e7c91e1a4f1) () const |
|**[asset](structeosio_1_1asset.md)** &|[**operator-=**](structeosio_1_1asset_ac34e7c2da42b547142cbc7e7e6348e23.md#1ac34e7c2da42b547142cbc7e7e6348e23) (const **[asset](structeosio_1_1asset.md)** & a) |
|**[asset](structeosio_1_1asset.md)** &|[**operator+=**](structeosio_1_1asset_ab94816fdb1ce8d429ccb63d46251e476.md#1ab94816fdb1ce8d429ccb63d46251e476) (const **[asset](structeosio_1_1asset.md)** & a) |
|**[asset](structeosio_1_1asset.md)** &|[**operator\*=**](structeosio_1_1asset_a4875ab6d76c75c195bb3fb725407ab30.md#1a4875ab6d76c75c195bb3fb725407ab30) (int64\_t a) <br>Multiplication assignment operator, with a number. |
|**[asset](structeosio_1_1asset.md)** &|[**operator/=**](structeosio_1_1asset_a5c86834934fe75db077730628608b6d3.md#1a5c86834934fe75db077730628608b6d3) (int64\_t a) <br>Division assignment operator, with a number. |
|void|[**print**](structeosio_1_1asset_a111a52097d1761898d5641536c308416.md#1a111a52097d1761898d5641536c308416) () const |


## Friends

|Type|Name|
|-----|-----|
|friend **[asset](structeosio_1_1asset.md)**|[**operator+**](structeosio_1_1asset_a606c72fe4e28b359ba977a6043f56300.md#1a606c72fe4e28b359ba977a6043f56300)|
|friend **[asset](structeosio_1_1asset.md)**|[**operator-**](structeosio_1_1asset_a101f03e631cf8c72d8094b91a5524207.md#1a101f03e631cf8c72d8094b91a5524207)|
|friend **[asset](structeosio_1_1asset.md)**|[**operator\***](structeosio_1_1asset_aad999a1c807bf5b87b6af18c7deb7383.md#1aad999a1c807bf5b87b6af18c7deb7383)<br>Multiplication operator, with a number proceeding. |
|friend **[asset](structeosio_1_1asset.md)**|[**operator\***](structeosio_1_1asset_a2680e36287438fb993924afb89203d0f.md#1a2680e36287438fb993924afb89203d0f)|
|friend **[asset](structeosio_1_1asset.md)**|[**operator/**](structeosio_1_1asset_ae07741c290b600e5d2891c47cc04d371.md#1ae07741c290b600e5d2891c47cc04d371)|
|friend int64\_t|[**operator/**](structeosio_1_1asset_a3ed38863be7162b18973870814c26346.md#1a3ed38863be7162b18973870814c26346)|
|friend bool|[**operator==**](structeosio_1_1asset_a8c79ce3d3e367c6345a8c5cb75bab48d.md#1a8c79ce3d3e367c6345a8c5cb75bab48d)|
|friend bool|[**operator!=**](structeosio_1_1asset_adb0577649f2f48023169390b0b78e234.md#1adb0577649f2f48023169390b0b78e234)|
|friend bool|[**operator<**](structeosio_1_1asset_a4c5d8977b23e534426a070f568524d02.md#1a4c5d8977b23e534426a070f568524d02)|
|friend bool|[**operator<=**](structeosio_1_1asset_a9d586e9b54232432779a91e2d6d8b78e.md#1a9d586e9b54232432779a91e2d6d8b78e)|
|friend bool|[**operator>**](structeosio_1_1asset_a88d1352c1cf2b7377292377449a4ced8.md#1a88d1352c1cf2b7377292377449a4ced8)|
|friend bool|[**operator>=**](structeosio_1_1asset_aa5ae6feb2c7097211820fc31cf24b5f5.md#1aa5ae6feb2c7097211820fc31cf24b5f5)|


## Public Attributes Documentation

### variable <a id="1ac76c16a58851550706c6709698ed7255" href="#1ac76c16a58851550706c6709698ed7255">amount</a>

```cpp
int64_t eosio::asset::amount;
```


The amount of the asset 

### variable <a id="1a374520d98a22f5a89a26025154944756" href="#1a374520d98a22f5a89a26025154944756">symbol</a>

```cpp
symbol eosio::asset::symbol;
```


The symbol name of the asset 

## Public Static Attributes Documentation

### variable <a id="1a478eb6a3bb302652748acfcf85738f5f" href="#1a478eb6a3bb302652748acfcf85738f5f">max\_amount</a>

```cpp
constexpr int64_t eosio::asset::max_amount;
```


Maximum amount possible for this asset. It's capped to 2^62 - 1 

## Public Functions Documentation

### function <a id="1abdde8d4b53c1b8f1027ab6cb2b13b845" href="#1abdde8d4b53c1b8f1027ab6cb2b13b845">asset (1/2)</a>

```cpp
eosio::asset::asset ()
```



### function <a id="1abb4cf1f370bb101c7d61910ff45d6b55" href="#1abb4cf1f370bb101c7d61910ff45d6b55">asset (2/2)</a>

```cpp
eosio::asset::asset (
    int64_t a,
    class symbol s
)
```


Construct a new asset given the symbol name and the amount


**Parameters:**


* **a** - The amount of the asset 
* **s** - The name of the symbol 



### function <a id="1a815b58378c8849fbb2a68f288edf5f31" href="#1a815b58378c8849fbb2a68f288edf5f31">is\_amount\_within\_range</a>

```cpp
bool eosio::asset::is_amount_within_range () const
```


Check if the amount doesn't exceed the max amount


**Returns:**

true - if the amount doesn't exceed the max amount 




**Returns:**

false - otherwise 




### function <a id="1aa2e8739bf534ea99199d121143ca7a0a" href="#1aa2e8739bf534ea99199d121143ca7a0a">is\_valid</a>

```cpp
bool eosio::asset::is_valid () const
```


Check if the asset is valid. A valid asset has its amount <= max\_amount and its symbol name valid


**Returns:**

true - if the asset is valid 




**Returns:**

false - otherwise 




### function <a id="1a0b6c8ba10eedc0129364fc910e840d7f" href="#1a0b6c8ba10eedc0129364fc910e840d7f">set\_amount</a>

```cpp
void eosio::asset::set_amount (
    int64_t a
)
```


Set the amount of the asset


**Parameters:**


* **a** - New amount for the asset 



### function <a id="1a16bee92e23903707b37a1e7c91e1a4f1" href="#1a16bee92e23903707b37a1e7c91e1a4f1">operator- (1/2)</a>

```cpp
asset eosio::asset::operator- () const
```


Unary minus operator


**Returns:**

asset - New asset with its amount is the negative amount of this asset 




### function <a id="1ac34e7c2da42b547142cbc7e7e6348e23" href="#1ac34e7c2da42b547142cbc7e7e6348e23">operator-=</a>

```cpp
asset & eosio::asset::operator-= (
    const asset & a
)
```


Subtraction assignment operator


**Parameters:**


* **a** - Another asset to subtract this asset with 



**Returns:**

asset& - Reference to this asset 




**Post**

The amount of this asset is subtracted by the amount of asset a 




### function <a id="1ab94816fdb1ce8d429ccb63d46251e476" href="#1ab94816fdb1ce8d429ccb63d46251e476">operator+=</a>

```cpp
asset & eosio::asset::operator+= (
    const asset & a
)
```


Addition Assignment operator


**Parameters:**


* **a** - Another asset to subtract this asset with 



**Returns:**

asset& - Reference to this asset 




**Post**

The amount of this asset is added with the amount of asset a 




### function <a id="1a4875ab6d76c75c195bb3fb725407ab30" href="#1a4875ab6d76c75c195bb3fb725407ab30">operator\*=</a>

```cpp
asset & eosio::asset::operator*= (
    int64_t a
)
```

Multiplication assignment operator, with a number. 

Multiplication assignment operator. Multiply the amount of this asset with a number and then assign the value to itself. 

**Parameters:**


* **a** - The multiplier for the asset's amount 



**Returns:**

asset - Reference to this asset 




**Post**

The amount of this asset is multiplied by a 




### function <a id="1a5c86834934fe75db077730628608b6d3" href="#1a5c86834934fe75db077730628608b6d3">operator/=</a>

```cpp
asset & eosio::asset::operator/= (
    int64_t a
)
```

Division assignment operator, with a number. 

Division assignment operator. Divide the amount of this asset with a number and then assign the value to itself. 

**Parameters:**


* **a** - The divisor for the asset's amount 



**Returns:**

asset - Reference to this asset 




**Post**

The amount of this asset is divided by a 




### function <a id="1a111a52097d1761898d5641536c308416" href="#1a111a52097d1761898d5641536c308416">print</a>

```cpp
void eosio::asset::print () const
```


Print the asset 

## Friends Documentation

### friend <a id="1a606c72fe4e28b359ba977a6043f56300" href="#1a606c72fe4e28b359ba977a6043f56300">operator+</a>

```cpp
friend asset eosio::asset::operator+ (
    const asset & a,
    const asset & b
)
```


Addition operator


**Parameters:**


* **a** - The first asset to be added 
* **b** - The second asset to be added 



**Returns:**

asset - New asset as the result of addition 




### friend <a id="1a101f03e631cf8c72d8094b91a5524207" href="#1a101f03e631cf8c72d8094b91a5524207">operator- (2/2)</a>

```cpp
friend asset eosio::asset::operator- (
    const asset & a,
    const asset & b
)
```


Subtraction operator


**Parameters:**


* **a** - The asset to be subtracted 
* **b** - The asset used to subtract 



**Returns:**

asset - New asset as the result of subtraction of a with b 




### friend <a id="1aad999a1c807bf5b87b6af18c7deb7383" href="#1aad999a1c807bf5b87b6af18c7deb7383">operator\* (1/2)</a>

```cpp
friend asset eosio::asset::operator* (
    const asset & a,
    int64_t b
)
```

Multiplication operator, with a number proceeding. 

Multiplication operator, with a number proceeding


**Parameters:**


* **a** - The asset to be multiplied 
* **b** - The multiplier for the asset's amount 



**Returns:**

asset - New asset as the result of multiplication 




### friend <a id="1a2680e36287438fb993924afb89203d0f" href="#1a2680e36287438fb993924afb89203d0f">operator\* (2/2)</a>

```cpp
friend asset eosio::asset::operator* (
    int64_t b,
    const asset & a
)
```


Multiplication operator, with a number preceeding


**Parameters:**


* **a** - The multiplier for the asset's amount 
* **b** - The asset to be multiplied 



**Returns:**

asset - New asset as the result of multiplication 




### friend <a id="1ae07741c290b600e5d2891c47cc04d371" href="#1ae07741c290b600e5d2891c47cc04d371">operator/ (1/2)</a>

```cpp
friend asset eosio::asset::operator/ (
    const asset & a,
    int64_t b
)
```


Division operator, with a number proceeding


**Parameters:**


* **a** - The asset to be divided 
* **b** - The divisor for the asset's amount 



**Returns:**

asset - New asset as the result of division 




### friend <a id="1a3ed38863be7162b18973870814c26346" href="#1a3ed38863be7162b18973870814c26346">operator/ (2/2)</a>

```cpp
friend int64_t eosio::asset::operator/ (
    const asset & a,
    const asset & b
)
```


Division operator, with another asset


**Parameters:**


* **a** - The asset which amount acts as the dividend 
* **b** - The asset which amount acts as the divisor 



**Returns:**

int64\_t - the resulted amount after the division 




**Precondition:**

Both asset must have the same symbol 




### friend <a id="1a8c79ce3d3e367c6345a8c5cb75bab48d" href="#1a8c79ce3d3e367c6345a8c5cb75bab48d">operator==</a>

```cpp
friend bool eosio::asset::operator== (
    const asset & a,
    const asset & b
)
```


Equality operator


**Parameters:**


* **a** - The first asset to be compared 
* **b** - The second asset to be compared 



**Returns:**

true - if both asset has the same amount 




**Returns:**

false - otherwise 




**Precondition:**

Both asset must have the same symbol 




### friend <a id="1adb0577649f2f48023169390b0b78e234" href="#1adb0577649f2f48023169390b0b78e234">operator!=</a>

```cpp
friend bool eosio::asset::operator!= (
    const asset & a,
    const asset & b
)
```


Inequality operator


**Parameters:**


* **a** - The first asset to be compared 
* **b** - The second asset to be compared 



**Returns:**

true - if both asset doesn't have the same amount 




**Returns:**

false - otherwise 




**Precondition:**

Both asset must have the same symbol 




### friend <a id="1a4c5d8977b23e534426a070f568524d02" href="#1a4c5d8977b23e534426a070f568524d02">operator<</a>

```cpp
friend bool eosio::asset::operator< (
    const asset & a,
    const asset & b
)
```


Less than operator


**Parameters:**


* **a** - The first asset to be compared 
* **b** - The second asset to be compared 



**Returns:**

true - if the first asset's amount is less than the second asset amount 




**Returns:**

false - otherwise 




**Precondition:**

Both asset must have the same symbol 




### friend <a id="1a9d586e9b54232432779a91e2d6d8b78e" href="#1a9d586e9b54232432779a91e2d6d8b78e">operator<=</a>

```cpp
friend bool eosio::asset::operator<= (
    const asset & a,
    const asset & b
)
```


Less or equal to operator


**Parameters:**


* **a** - The first asset to be compared 
* **b** - The second asset to be compared 



**Returns:**

true - if the first asset's amount is less or equal to the second asset amount 




**Returns:**

false - otherwise 




**Precondition:**

Both asset must have the same symbol 




### friend <a id="1a88d1352c1cf2b7377292377449a4ced8" href="#1a88d1352c1cf2b7377292377449a4ced8">operator></a>

```cpp
friend bool eosio::asset::operator> (
    const asset & a,
    const asset & b
)
```


Greater than operator


**Parameters:**


* **a** - The first asset to be compared 
* **b** - The second asset to be compared 



**Returns:**

true - if the first asset's amount is greater than the second asset amount 




**Returns:**

false - otherwise 




**Precondition:**

Both asset must have the same symbol 




### friend <a id="1aa5ae6feb2c7097211820fc31cf24b5f5" href="#1aa5ae6feb2c7097211820fc31cf24b5f5">operator>=</a>

```cpp
friend bool eosio::asset::operator>= (
    const asset & a,
    const asset & b
)
```


Greater or equal to operator


**Parameters:**


* **a** - The first asset to be compared 
* **b** - The second asset to be compared 



**Returns:**

true - if the first asset's amount is greater or equal to the second asset amount 




**Returns:**

false - otherwise 




**Precondition:**

Both asset must have the same symbol 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/asset.hpp`
