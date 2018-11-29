---
search:
    keywords: ['multiindex', 'eosio::multi_index']
---

# group multiindex

Defines **[EOSIO](struct_e_o_s_i_o.md)** Multi Index Table. [More...](#detailed-description)
## Classes

|Type|Name|
|-----|-----|
|class|[**eosio::multi\_index**](classeosio_1_1multi__index.md)|


## Detailed Description

**[EOSIO](struct_e_o_s_i_o.md)** Multi-Index API provides a C++ interface to the **[EOSIO](struct_e_o_s_i_o.md)** database. It is patterned after Boost Multi Index Container. **[EOSIO](struct_e_o_s_i_o.md)** Multi-Index table requires exactly a uint64\_t primary key. For the table to be able to retrieve the primary key, the object stored inside the table is required to have a const member function called primary\_key() that returns uint64\_t. **[EOSIO](struct_e_o_s_i_o.md)** Multi-Index table also supports up to 16 secondary indices. The type of the secondary indices could be any of:
* uint64\_t
* uint128\_t
* uint256\_t
* double
* long double




**Template parameters:**


* **TableName** - name of the table 
* **T** - type of the data stored inside the table 
* **Indices** - secondary indices for the table, up to 16 indices is supported here

Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
class mycontract: contract {
  struct record {
    uint64_t    primary;
    uint64_t    secondary_1;
    uint128_t   secondary_2;
    uint256_t   secondary_3;
    double      secondary_4;
    long double secondary_5;
    uint64_t primary_key() const { return primary; }
    uint64_t get_secondary_1() const { return secondary_1; }
    uint128_t get_secondary_2() const { return secondary_2; }
    uint256_t get_secondary_3() const { return secondary_3; }
    double get_secondary_4() const { return secondary_4; }
    long double get_secondary_5() const { return secondary_5; }
  };
  public:
    mycontract(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds){}
    void myaction() {
      auto code = _self;
      auto scope = _self;
      multi_index<name("mytable"), record,
        indexed_by< name("bysecondary1"), const_mem_fun<record, uint64_t, &record::get_secondary_1> >,
        indexed_by< name("bysecondary2"), const_mem_fun<record, uint128_t, &record::get_secondary_2> >,
        indexed_by< name("bysecondary3"), const_mem_fun<record, uint256_t, &record::get_secondary_3> >,
        indexed_by< name("bysecondary4"), const_mem_fun<record, double, &record::get_secondary_4> >,
        indexed_by< name("bysecondary5"), const_mem_fun<record, long double, &record::get_secondary_5> >
      > table( code, scope.value);
    }
}
EOSIO_DISPATCH( mycontract, (myaction) )
```

 
