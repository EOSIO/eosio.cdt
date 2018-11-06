---
search:
    keywords: ['eosio::indexed_by', 'constants', 'secondary_extractor_type']
---

# struct eosio::indexed\_by

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**indexed\_by**](structeosio_1_1indexed__by.md)


The **[indexed\_by](structeosio_1_1indexed__by.md)** struct is used to instantiate the indices for the Multi-Index table. In **[EOSIO](struct_e_o_s_i_o.md)**, up to 16 secondary indices can be specified. [More...](#detailed-description)
## Public Types

|Type|Name|
|-----|-----|
|enum|[**constants**](structeosio_1_1indexed__by_ad04dd8d771430edeb499c9d03bc9bb60.md#1ad04dd8d771430edeb499c9d03bc9bb60) { **index\_name** = static\_cast<uint64\_t>(IndexName) } |
|typedef Extractor|[**secondary\_extractor\_type**](structeosio_1_1indexed__by_affd7e285b6b9435c7c2b0104a14b5791.md#1affd7e285b6b9435c7c2b0104a14b5791)|


## Detailed Description

The **[indexed\_by](structeosio_1_1indexed__by.md)** struct is used to instantiate the indices for the Multi-Index table. In **[EOSIO](struct_e_o_s_i_o.md)**, up to 16 secondary indices can be specified. 

**Template parameters:**


* **IndexName** - is the name of the index. The name must be provided as an **[EOSIO](struct_e_o_s_i_o.md)** base32 encoded 64-bit integer and must conform to the **[EOSIO](struct_e_o_s_i_o.md)** naming requirements of a maximum of 13 characters, the first twelve from the lowercase characters a-z, digits 0-5, and ".", and if there is a 13th character, it is restricted to lowercase characters a-p and ".". 
* **Extractor** - is a function call operator that takes a const reference to the table object type and returns either a secondary key type or a reference to a secondary key type. It is recommended to use the `` template, which is a type alias to the `boost::multi\_index::const\_mem\_fun`. See the documentation for the Boost `` key extractor for more details.

Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
class mycontract: eosio::contract {
  struct record {
     uint64_t    primary;
     uint128_t   secondary;
     uint64_t primary_key() const { return primary; }
     uint64_t get_secondary() const { return secondary; }
     EOSLIB_SERIALIZE( record, (primary)(secondary) )
   };
  public:
    mycontract(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds){}
    void myaction() {
      auto code = _self;
      auto scope = _self;
      multi_index<name("mytable"), record,
                 indexed_by< name("bysecondary"), const_mem_fun<record, uint128_t, &record::get_secondary> > > table( code, scope.value);
    }
}
EOSIO_DISPATCH( mycontract, (myaction) )
```

 
## Public Types Documentation

### enum <a id="1ad04dd8d771430edeb499c9d03bc9bb60" href="#1ad04dd8d771430edeb499c9d03bc9bb60">constants</a>

```cpp
enum eosio::indexed_by::constants {
    index_name = static_cast<uint64_t>(IndexName),
};
```



### typedef <a id="1affd7e285b6b9435c7c2b0104a14b5791" href="#1affd7e285b6b9435c7c2b0104a14b5791">secondary\_extractor\_type</a>

```cpp
typedef Extractor eosio::indexed_by< IndexName, Extractor >::secondary_extractor_type;
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
