---
search:
    keywords: ['eosio::multi_index', 'const_iterator', 'index', 'intc', 'item', 'item_ptr', 'const_reverse_iterator', 'multi_index', 'get_code', 'get_scope', 'cbegin', 'begin', 'cend', 'end', 'crbegin', 'rbegin', 'crend', 'rend', 'lower_bound', 'upper_bound', 'available_primary_key', 'get_index', 'get_index', 'iterator_to', 'emplace', 'modify', 'modify', 'get', 'find', 'require_find', 'erase', 'erase']
---

# class eosio::multi\_index

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**multi\_index**](classeosio_1_1multi__index.md)


## Classes

|Type|Name|
|-----|-----|
|struct|[**const\_iterator**](structeosio_1_1multi__index_1_1const__iterator.md)|
|struct|[**index**](structeosio_1_1multi__index_1_1index.md)|
|struct|[**intc**](structeosio_1_1multi__index_1_1intc.md)|
|struct|[**item**](structeosio_1_1multi__index_1_1item.md)|
|struct|[**item\_ptr**](structeosio_1_1multi__index_1_1item__ptr.md)|


## Public Types

|Type|Name|
|-----|-----|
|typedef std::reverse\_iterator< **[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)** >|[**const\_reverse\_iterator**](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)<br>struct **[multi\_index::const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)** |


## Public Functions

|Type|Name|
|-----|-----|
||[**multi\_index**](classeosio_1_1multi__index_aa081f69f6fa288869d1d5881f0be04dd.md#1aa081f69f6fa288869d1d5881f0be04dd) (**[name](structeosio_1_1name.md)** code, uint64\_t scope) <br>load\_object\_by\_primary\_iterator |
|**[name](structeosio_1_1name.md)**|[**get\_code**](classeosio_1_1multi__index_a6ef0fb806ad6edf50b670a115fc3d5a1.md#1a6ef0fb806ad6edf50b670a115fc3d5a1) () const |
|uint64\_t|[**get\_scope**](classeosio_1_1multi__index_a0690e9e9a30254240fa0cb6bb12bebf3.md#1a0690e9e9a30254240fa0cb6bb12bebf3) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**cbegin**](classeosio_1_1multi__index_a36ad6b1c1013b6f753660b334de96aef.md#1a36ad6b1c1013b6f753660b334de96aef) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**begin**](classeosio_1_1multi__index_a060b08a9da300e3a4ab669b35f07c9bc.md#1a060b08a9da300e3a4ab669b35f07c9bc) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**cend**](classeosio_1_1multi__index_aece639944fa9031debbd984bbd42485a.md#1aece639944fa9031debbd984bbd42485a) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**end**](classeosio_1_1multi__index_a748fcb4d6a08962ad8a7c92da359e3d7.md#1a748fcb4d6a08962ad8a7c92da359e3d7) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**crbegin**](classeosio_1_1multi__index_abe296816f75cdb69e6698724e7b0c29d.md#1abe296816f75cdb69e6698724e7b0c29d) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**rbegin**](classeosio_1_1multi__index_acdc14cc71a43489507341d2709696e7d.md#1acdc14cc71a43489507341d2709696e7d) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**crend**](classeosio_1_1multi__index_a6ce680bcec3a391e0ad89a1cd1143231.md#1a6ce680bcec3a391e0ad89a1cd1143231) () const |
|**[const\_reverse\_iterator](classeosio_1_1multi__index_ab46b850d4d0d3da39cd96e77ec10c51e.md#1ab46b850d4d0d3da39cd96e77ec10c51e)**|[**rend**](classeosio_1_1multi__index_a659b2744975d0a54162c44fbcbc28115.md#1a659b2744975d0a54162c44fbcbc28115) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**lower\_bound**](classeosio_1_1multi__index_aa642a184ce729f91bc70200ba2a55253.md#1aa642a184ce729f91bc70200ba2a55253) (uint64\_t primary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**upper\_bound**](classeosio_1_1multi__index_a5a9f1b25fd64a2bdb163f97ca97eb371.md#1a5a9f1b25fd64a2bdb163f97ca97eb371) (uint64\_t primary) const |
|uint64\_t|[**available\_primary\_key**](classeosio_1_1multi__index_a1b2d26324eb34354f5db2abcf2d87853.md#1a1b2d26324eb34354f5db2abcf2d87853) () const |
|auto|[**get\_index**](classeosio_1_1multi__index_a98fecc3166ebb3746ee810f7f7d9beac.md#1a98fecc3166ebb3746ee810f7f7d9beac) () |
|auto|[**get\_index**](classeosio_1_1multi__index_ae6e01877b9862c8193df27f05c305f2c.md#1ae6e01877b9862c8193df27f05c305f2c) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**iterator\_to**](classeosio_1_1multi__index_ad611e3ff51efdf038b934138d071cf83.md#1ad611e3ff51efdf038b934138d071cf83) (const T & obj) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**emplace**](classeosio_1_1multi__index_ab64fbf4dd04be357ec0cbbf125a3da5e.md#1ab64fbf4dd04be357ec0cbbf125a3da5e) (**[name](structeosio_1_1name.md)** payer, Lambda && constructor) |
|void|[**modify**](classeosio_1_1multi__index_a4b3556ef69c7faa917f185ae33a34442.md#1a4b3556ef69c7faa917f185ae33a34442) (**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)** itr, **[name](structeosio_1_1name.md)** payer, Lambda && updater) |
|void|[**modify**](classeosio_1_1multi__index_ab44fb8b993970f1b3b7ac5f37cd2d2c9.md#1ab44fb8b993970f1b3b7ac5f37cd2d2c9) (const T & obj, **[name](structeosio_1_1name.md)** payer, Lambda && updater) |
|const T &|[**get**](classeosio_1_1multi__index_af05dfe7b56cf37179f76632cf619f0d1.md#1af05dfe7b56cf37179f76632cf619f0d1) (uint64\_t primary, const char \* error\_msg = "unable to **[find](classeosio_1_1multi__index_a40a65cdfcc298b85e0e4ddf4c3581c1c.md#1a40a65cdfcc298b85e0e4ddf4c3581c1c)** key") const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**find**](classeosio_1_1multi__index_a40a65cdfcc298b85e0e4ddf4c3581c1c.md#1a40a65cdfcc298b85e0e4ddf4c3581c1c) (uint64\_t primary) const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**require\_find**](classeosio_1_1multi__index_a8a65dee974002fd6324e72a0a978fddf.md#1a8a65dee974002fd6324e72a0a978fddf) (uint64\_t primary, const char \* error\_msg = "unable to **[find](classeosio_1_1multi__index_a40a65cdfcc298b85e0e4ddf4c3581c1c.md#1a40a65cdfcc298b85e0e4ddf4c3581c1c)** key") const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**erase**](classeosio_1_1multi__index_ad28ac8d91e9af22cbbc12962a805d253.md#1ad28ac8d91e9af22cbbc12962a805d253) (**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)** itr) |
|void|[**erase**](classeosio_1_1multi__index_ac5f84064eacb4265ad44f15cecebb4be.md#1ac5f84064eacb4265ad44f15cecebb4be) (const T & obj) |


## Public Types Documentation

### typedef <a id="1ab46b850d4d0d3da39cd96e77ec10c51e" href="#1ab46b850d4d0d3da39cd96e77ec10c51e">const\_reverse\_iterator</a>

```cpp
typedef std::reverse_iterator<const_iterator> eosio::multi_index< TableName, T, Indices >::const_reverse_iterator;
```

struct **[multi\_index::const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)** 


## Public Functions Documentation

### function <a id="1aa081f69f6fa288869d1d5881f0be04dd" href="#1aa081f69f6fa288869d1d5881f0be04dd">multi\_index</a>

```cpp
eosio::multi_index::multi_index (
    name code,
    uint64_t scope
)
```

load\_object\_by\_primary\_iterator 

Constructs an instance of a Multi-Index table. Constructs an instance of a Multi-Index table.


**Parameters:**


* **code** - Account that owns table 
* **scope** - Scope identifier within the code hierarchy



**Precondition:**

code and scope member properties are initialized 




**Post**

each secondary index table initialized 




**Post**

Secondary indices are updated to refer to the newly added object. If the secondary index tables do not exist, they are created. 




**Post**

The payer is charged for the storage usage of the new object and, if the table (and secondary index tables) must be created, for the overhead of the table creation.


Notes The `` template has template parameters `<`, where:
* `TableName` is the name of the table, maximum 12 characters long, characters in the name from the set of lowercase letters, digits 1 to 5, and the "." (period) character and is converted to a eosio::raw - which wraps uint64\_t;
* `T` is the object type (i.e., row definition);
* `Indices` is a list of up to 16 secondary indices.
* Each must be a default constructable class or struct
* Each must have a function call operator that takes a const reference to the table object type and returns either a secondary key type or a reference to a secondary key type
* It is recommended to use the **[eosio::const\_mem\_fun](structeosio_1_1const__mem__fun.md)** template, which is a type alias to the boost::multi\_index::const\_mem\_fun. See the documentation for the Boost **[const\_mem\_fun](structeosio_1_1const__mem__fun.md)** key extractor for more details.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a6ef0fb806ad6edf50b670a115fc3d5a1" href="#1a6ef0fb806ad6edf50b670a115fc3d5a1">get\_code</a>

```cpp
name eosio::multi_index::get_code () const
```


Returns the `code` member property.


**Returns:**

Account name of the Code that owns the Primary Table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(name("dan"), name("dan").value); // code, scope
      eosio_assert(addresses.get_code() == name("dan"), "Lock arf, Codes don't match.");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a0690e9e9a30254240fa0cb6bb12bebf3" href="#1a0690e9e9a30254240fa0cb6bb12bebf3">get\_scope</a>

```cpp
uint64_t eosio::multi_index::get_scope () const
```


Returns the `scope` member property.


**Returns:**

Scope id of the Scope within the Code of the Current Receiver under which the desired Primary Table instance can be found.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(name("dan"), name("dan").value); // code, scope
      eosio_assert(addresses.get_scope() == name("dan").value, "Lock arf, Scopes don't match");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a36ad6b1c1013b6f753660b334de96aef" href="#1a36ad6b1c1013b6f753660b334de96aef">cbegin</a>

```cpp
const_iterator eosio::multi_index::cbegin () const
```


Returns an iterator pointing to the object\_type with the lowest primary key value in the Multi-Index table.


**Returns:**

An iterator pointing to the object\_type with the lowest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr == addresses.cbegin(), "Lock arf, Only address is not at front.");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a060b08a9da300e3a4ab669b35f07c9bc" href="#1a060b08a9da300e3a4ab669b35f07c9bc">begin</a>

```cpp
const_iterator eosio::multi_index::begin () const
```


Returns an iterator pointing to the object\_type with the lowest primary key value in the Multi-Index table.


**Returns:**

An iterator pointing to the object\_type with the lowest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr == addresses.begin(), "Lock arf, Only address is not at front.");
    }
}
EOSIO_ABI( addressbook, (myaction) )
```

 

### function <a id="1aece639944fa9031debbd984bbd42485a" href="#1aece639944fa9031debbd984bbd42485a">cend</a>

```cpp
const_iterator eosio::multi_index::cend () const
```


Returns an iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


**Returns:**

An iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr != addresses.cend(), "Lock arf, Address for account doesn't exist");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a748fcb4d6a08962ad8a7c92da359e3d7" href="#1a748fcb4d6a08962ad8a7c92da359e3d7">end</a>

```cpp
const_iterator eosio::multi_index::end () const
```


Returns an iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


**Returns:**

An iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr != addresses.end(), "Lock arf, Address for account doesn't exist");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1abe296816f75cdb69e6698724e7b0c29d" href="#1abe296816f75cdb69e6698724e7b0c29d">crbegin</a>

```cpp
const_reverse_iterator eosio::multi_index::crbegin () const
```


Returns a reverse iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


**Returns:**

A reverse iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
      });
      auto itr = addresses.crbegin();
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect Last Record ");
      itr++;
      eosio_assert(itr->account_name == name("brendan"), "Lock arf, Incorrect Second Last Record");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1acdc14cc71a43489507341d2709696e7d" href="#1acdc14cc71a43489507341d2709696e7d">rbegin</a>

```cpp
const_reverse_iterator eosio::multi_index::rbegin () const
```


Returns a reverse iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


**Returns:**

A reverse iterator pointing to the `object\_type` with the highest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
      });
      auto itr = addresses.rbegin();
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect Last Record ");
      itr++;
      eosio_assert(itr->account_name == name("brendan"), "Lock arf, Incorrect Second Last Record");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a6ce680bcec3a391e0ad89a1cd1143231" href="#1a6ce680bcec3a391e0ad89a1cd1143231">crend</a>

```cpp
const_reverse_iterator eosio::multi_index::crend () const
```


Returns an iterator pointing to the `object\_type` with the lowest primary key value in the Multi-Index table.


**Returns:**

An iterator pointing to the `object\_type` with the lowest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
      });
      auto itr = addresses.crend();
      itr--;
      eosio_assert(itr->account_name == name("brendan"), "Lock arf, Incorrect First Record ");
      itr--;
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect Second Record");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a659b2744975d0a54162c44fbcbc28115" href="#1a659b2744975d0a54162c44fbcbc28115">rend</a>

```cpp
const_reverse_iterator eosio::multi_index::rend () const
```


Returns an iterator pointing to the `object\_type` with the lowest primary key value in the Multi-Index table.


**Returns:**

An iterator pointing to the `object\_type` with the lowest primary key value in the Multi-Index table.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
      });
      auto itr = addresses.rend();
      itr--;
      eosio_assert(itr->account_name == name("brendan"), "Lock arf, Incorrect First Record ");
      itr--;
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect Second Record");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1aa642a184ce729f91bc70200ba2a55253" href="#1aa642a184ce729f91bc70200ba2a55253">lower\_bound</a>

```cpp
const_iterator eosio::multi_index::lower_bound (
    uint64_t primary
) const
```


Searches for the `object\_type` with the lowest primary key that is greater than or equal to a given primary key.


**Parameters:**


* **primary** - Primary key that establishes the target value for the lower bound search. 



**Returns:**

An iterator pointing to the `object\_type` that has the lowest primary key that is greater than or equal to `primary`. If an object could not be found, it will return the `end` iterator. If the table does not exist\*\* it will return `-1`.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint32_t zip = 0;
     uint64_t primary_key() const { return account_name; }
     uint64_t by_zip() const { return zip; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address, indexed_by< name("zip"), const_mem_fun<address, uint64_t, &address::by_zip> > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
        address.zip = 93446;
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
        address.zip = 93445;
      });
      uint32_t zipnumb = 93445;
      auto zip_index = addresses.get_index<name("zip")>();
      auto itr = zip_index.lower_bound(zipnumb);
      eosio_assert(itr->account_name == name("brendan"), "Lock arf, Incorrect First Lower Bound Record ");
      itr++;
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect Second Lower Bound Record");
      itr++;
      eosio_assert(itr == zip_index.end(), "Lock arf, Incorrect End of Iterator");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a5a9f1b25fd64a2bdb163f97ca97eb371" href="#1a5a9f1b25fd64a2bdb163f97ca97eb371">upper\_bound</a>

```cpp
const_iterator eosio::multi_index::upper_bound (
    uint64_t primary
) const
```


Searches for the `object\_type` with the highest primary key that is less than or equal to a given primary key.


**Parameters:**


* **primary** - Primary key that establishes the target value for the upper bound search 



**Returns:**

An iterator pointing to the `object\_type` that has the highest primary key that is less than or equal to `primary`. If an object could not be found, it will return the `end` iterator. If the table does not exist\*\* it will return `-1`.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint32_t zip = 0;
     uint64_t liked = 0;
     uint64_t primary_key() const { return account_name; }
     uint64_t by_zip() const { return zip; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address, indexed_by< name("zip"), const_mem_fun<address, uint64_t, &address::by_zip> > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
        address.zip = 93446;
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
        address.zip = 93445;
      });
      uint32_t zipnumb = 93445;
      auto zip_index = addresses.get_index<name("zip")>();
      auto itr = zip_index.upper_bound(zipnumb);
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect First Upper Bound Record ");
      itr++;
      eosio_assert(itr == zip_index.end(), "Lock arf, Incorrect End of Iterator");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a1b2d26324eb34354f5db2abcf2d87853" href="#1a1b2d26324eb34354f5db2abcf2d87853">available\_primary\_key</a>

```cpp
uint64_t eosio::multi_index::available_primary_key () const
```


Returns an available primary key.


**Returns:**

An available (unused) primary key value.


Notes: Intended to be used in tables in which the primary keys of the table are strictly intended to be auto-incrementing, and thus will never be set to custom values by the contract. Violating this expectation could result in the table appearing to be full due to inability to allocate an available primary key. Ideally this method would only be used to determine the appropriate primary key to use within new objects added to a table in which the primary keys of the table are strictly intended from the beginning to be autoincrementing and thus will not ever be set to custom arbitrary values by the contract. Violating this agreement could result in the table appearing full when in reality there is plenty of space left.
Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t key;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return key; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.key = addresses.available_primary_key();
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a98fecc3166ebb3746ee810f7f7d9beac" href="#1a98fecc3166ebb3746ee810f7f7d9beac">get\_index (1/2)</a>

```cpp
auto eosio::multi_index::get_index ()
```


Returns an appropriately typed Secondary Index.


**Template parameters:**


* **IndexName** - the ID of the desired secondary index



**Returns:**

An index of the appropriate type: Primitive 64-bit unsigned integer key (idx64), Primitive 128-bit unsigned integer key (idx128), 128-bit fixed-size lexicographical key (idx128), 256-bit fixed-size lexicographical key (idx256), Floating point key, Double precision floating point key, Long Double (quadruple) precision floating point key


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint32_t zip = 0;
     uint64_t primary_key() const { return account_name; }
     uint64_t by_zip() const { return zip; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address, indexed_by< name("zip"), const_mem_fun<address, uint64_t, &address::by_zip> > address_index;
    void myaction() {
      address_index addresses(_self, _self.value);  // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
        address.zip = 93446;
      });
      uint32_t zipnumb = 93446;
      auto zip_index = addresses.get_index<name("zip")>();
      auto itr = zip_index.find(zipnumb);
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect Record ");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1ae6e01877b9862c8193df27f05c305f2c" href="#1ae6e01877b9862c8193df27f05c305f2c">get\_index (2/2)</a>

```cpp
auto eosio::multi_index::get_index () const
```


Returns an appropriately typed Secondary Index.


**Template parameters:**


* **IndexName** - the ID of the desired secondary index



**Returns:**

An index of the appropriate type: Primitive 64-bit unsigned integer key (idx64), Primitive 128-bit unsigned integer key (idx128), 128-bit fixed-size lexicographical key (idx128), 256-bit fixed-size lexicographical key (idx256), Floating point key, Double precision floating point key, Long Double (quadruple) precision floating point key


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint32_t zip = 0;
     uint64_t primary_key() const { return account_name; }
     uint64_t by_zip() const { return zip; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address, indexed_by< name("zip"), const_mem_fun<address, uint64_t, &address::by_zip> > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
        address.zip = 93446;
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
        address.zip = 93445;
      });
      uint32_t zipnumb = 93445;
      auto zip_index = addresses.get_index<name("zip")>();
      auto itr = zip_index.upper_bound(zipnumb);
      eosio_assert(itr->account_name == name("dan"), "Lock arf, Incorrect First Upper Bound Record ");
      itr++;
      eosio_assert(itr == zip_index.end(), "Lock arf, Incorrect End of Iterator");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1ad611e3ff51efdf038b934138d071cf83" href="#1ad611e3ff51efdf038b934138d071cf83">iterator\_to</a>

```cpp
const_iterator eosio::multi_index::iterator_to (
    const T & obj
) const
```


Returns an iterator to the given object in a Multi-Index table.


**Parameters:**


* **obj** - A reference to the desired object



**Returns:**

An iterator to the given object


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint32_t zip = 0;
     uint64_t primary_key() const { return account_name; }
     uint64_t by_zip() const { return zip; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address, indexed_by< name("zip"), const_mem_fun<address, uint64_t, &address::by_zip> > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
        address.zip = 93446;
      });
      addresses.emplace(payer, [&](auto& address) {
        address.account_name = name("brendan");
        address.first_name = "Brendan";
        address.last_name = "Blumer";
        address.street = "1 EOS Way";
        address.city = "Hong Kong";
        address.state = "HK";
        address.zip = 93445;
      });
      auto user = addresses.get(name("dan"));
      auto itr = address.find(name("dan"));
      eosio_assert(iterator_to(user) == itr, "Lock arf, Invalid iterator");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1ab64fbf4dd04be357ec0cbbf125a3da5e" href="#1ab64fbf4dd04be357ec0cbbf125a3da5e">emplace</a>

```cpp
const_iterator eosio::multi_index::emplace (
    name payer,
    Lambda && constructor
)
```


Adds a new object (i.e., row) to the table.


**Parameters:**


* **payer** - Account name of the payer for the Storage usage of the new object 
* **constructor** - Lambda function that does an in-place initialization of the object to be created in the table



**Precondition:**

A multi index table has been instantiated 




**Post**

A new object is created in the Multi-Index table, with a unique primary key (as specified in the object). The object is serialized and written to the table. If the table does not exist, it is created. 




**Post**

Secondary indices are updated to refer to the newly added object. If the secondary index tables do not exist, they are created. 




**Post**

The payer is charged for the storage usage of the new object and, if the table (and secondary index tables) must be created, for the overhead of the table creation.




**Returns:**

A primary key iterator to the newly created object


Exception - The account is not authorized to write to the table.
Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a4b3556ef69c7faa917f185ae33a34442" href="#1a4b3556ef69c7faa917f185ae33a34442">modify (1/2)</a>

```cpp
void eosio::multi_index::modify (
    const_iterator itr,
    name payer,
    Lambda && updater
)
```


Modifies an existing object in a table.


**Parameters:**


* **itr** - an iterator pointing to the object to be updated 
* **payer** - account name of the payer for the Storage usage of the updated row 
* **updater** - lambda function that updates the target object



**Precondition:**

itr points to an existing element 




**Precondition:**

payer is a valid account that is authorized to execute the action and be billed for storage usage.




**Post**

The modified object is serialized, then replaces the existing object in the table. 




**Post**

Secondary indices are updated; the primary key of the updated object is not changed. 




**Post**

The payer is charged for the storage usage of the updated object. 




**Post**

If payer is the same as the existing payer, payer only pays for the usage difference between existing and updated object (and is refunded if this difference is negative). 




**Post**

If payer is different from the existing payer, the existing payer is refunded for the storage usage of the existing object.


Exceptions: If called with an invalid precondition, execution is aborted.
Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr != addresses.end(), "Lock arf, Address for account not found");
      addresses.modify( itr, account payer, [&]( auto& address ) {
        address.city = "San Luis Obispo";
        address.state = "CA";
      });
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1ab44fb8b993970f1b3b7ac5f37cd2d2c9" href="#1ab44fb8b993970f1b3b7ac5f37cd2d2c9">modify (2/2)</a>

```cpp
void eosio::multi_index::modify (
    const T & obj,
    name payer,
    Lambda && updater
)
```


Modifies an existing object in a table.


**Parameters:**


* **obj** - a reference to the object to be updated 
* **payer** - account name of the payer for the Storage usage of the updated row 
* **updater** - lambda function that updates the target object



**Precondition:**

obj is an existing object in the table 




**Precondition:**

payer is a valid account that is authorized to execute the action and be billed for storage usage.




**Post**

The modified object is serialized, then replaces the existing object in the table. 




**Post**

Secondary indices are updated; the primary key of the updated object is not changed. 




**Post**

The payer is charged for the storage usage of the updated object. 




**Post**

If payer is the same as the existing payer, payer only pays for the usage difference between existing and updated object (and is refunded if this difference is negative). 




**Post**

If payer is different from the existing payer, the existing payer is refunded for the storage usage of the existing object.


Exceptions: If called with an invalid precondition, execution is aborted.
Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr != addresses.end(), "Lock arf, Address for account not found");
      addresses.modify( *itr, payer, [&]( auto& address ) {
        address.city = "San Luis Obispo";
        address.state = "CA";
      });
      eosio_assert(itr->city == "San Luis Obispo", "Lock arf, Address not modified");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1af05dfe7b56cf37179f76632cf619f0d1" href="#1af05dfe7b56cf37179f76632cf619f0d1">get</a>

```cpp
const T & eosio::multi_index::get (
    uint64_t primary,
    const char * error_msg = "unable to find key"
) const
```


Retrieves an existing object from a table using its primary key.


**Parameters:**


* **primary** - Primary key value of the object 



**Returns:**

A constant reference to the object containing the specified primary key.


Exception - No object matches the given key
Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto user = addresses.get(name("dan"));
      eosio_assert(user.first_name == "Daniel", "Lock arf, Couldn't get him.");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a40a65cdfcc298b85e0e4ddf4c3581c1c" href="#1a40a65cdfcc298b85e0e4ddf4c3581c1c">find</a>

```cpp
const_iterator eosio::multi_index::find (
    uint64_t primary
) const
```


Search for an existing object in a table using its primary key.


**Parameters:**


* **primary** - Primary key value of the object 



**Returns:**

An iterator to the found object which has a primary key equal to `primary` OR the `end` iterator of the referenced table if an object with primary key `primary` is not found.


Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr != addresses.end(), "Everting Lock arf, Couldn't get him.");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 

### function <a id="1a8a65dee974002fd6324e72a0a978fddf" href="#1a8a65dee974002fd6324e72a0a978fddf">require\_find</a>

```cpp
const_iterator eosio::multi_index::require_find (
    uint64_t primary,
    const char * error_msg = "unable to find key"
) const
```


Search for an existing object in a table using its primary key.


**Parameters:**


* **primary** - Primary key value of the object 
* **error\_msg** - error message if an object with primary key `primary` is not found. 



**Returns:**

An iterator to the found object which has a primary key equal to `primary` OR throws an exception if an object with primary key `primary` is not found. 




### function <a id="1ad28ac8d91e9af22cbbc12962a805d253" href="#1ad28ac8d91e9af22cbbc12962a805d253">erase (1/2)</a>

```cpp
const_iterator eosio::multi_index::erase (
    const_iterator itr
)
```


Remove an existing object from a table using its primary key.


**Parameters:**


* **itr** - An iterator pointing to the object to be removed



**Precondition:**

itr points to an existing element 




**Post**

The object is removed from the table and all associated storage is reclaimed. 




**Post**

Secondary indices associated with the table are updated. 




**Post**

The existing payer for storage usage of the object is refunded for the table and secondary indices usage of the removed object, and if the table and indices are removed, for the associated overhead.




**Returns:**

For the signature with ``, returns a pointer to the object following the removed object.


Exceptions: The object to be removed is not in the table. The action is not authorized to modify the table. The given iterator is invalid.
Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr != addresses.end(), "Lock arf, Address for account not found");
      addresses.erase( itr );
      eosio_assert(itr != addresses.end(), "Everting lock arf, Address not erased properly");
    }
}
EOSIO_ABI( addressbook, (myaction) )
```

 

### function <a id="1ac5f84064eacb4265ad44f15cecebb4be" href="#1ac5f84064eacb4265ad44f15cecebb4be">erase (2/2)</a>

```cpp
void eosio::multi_index::erase (
    const T & obj
)
```


Remove an existing object from a table using its primary key.


**Parameters:**


* **obj** - Object to be removed



**Precondition:**

obj is an existing object in the table 




**Post**

The object is removed from the table and all associated storage is reclaimed. 




**Post**

Secondary indices associated with the table are updated. 




**Post**

The existing payer for storage usage of the object is refunded for the table and secondary indices usage of the removed object, and if the table and indices are removed, for the associated overhead.


Exceptions: The object to be removed is not in the table. The action is not authorized to modify the table. The given iterator is invalid.
Example:

```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;
using namespace std;
class addressbook: contract {
  struct address {
     uint64_t account_name;
     string first_name;
     string last_name;
     string street;
     string city;
     string state;
     uint64_t primary_key() const { return account_name; }
  };
  public:
    addressbook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}
    typedef eosio::multi_index< name("address"), address > address_index;
    void myaction() {
      address_index addresses(_self, _self.value); // code, scope
      // add to table, first argument is account to bill for storage
      addresses.emplace(_self, [&](auto& address) {
        address.account_name = name("dan");
        address.first_name = "Daniel";
        address.last_name = "Larimer";
        address.street = "1 EOS Way";
        address.city = "Blacksburg";
        address.state = "VA";
      });
      auto itr = addresses.find(name("dan"));
      eosio_assert(itr != addresses.end(), "Lock arf, Record is not found");
      addresses.erase(*itr);
      itr = addresses.find(name("dan"));
      eosio_assert(itr == addresses.end(), "Lock arf, Record is not deleted");
    }
}
EOSIO_DISPATCH( addressbook, (myaction) )
```

 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
