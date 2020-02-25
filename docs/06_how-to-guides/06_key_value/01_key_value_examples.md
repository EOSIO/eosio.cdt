### DEFINE_TABLE Example:
```cpp
struct myrecord {
   std::string                primary_key;
   uint64_t                   secondary_1;
   std::tuple<uint32_t, bool> secondary_2;
}

DEFINE_TABLE(mytable, myrecord, "testtable",
   primary_key,
   secondary_1,
   secondary_2
)

// The above macro results in the following class.

struct mytable : kv_table<mytable, myrecord, "testtable"_n> {
   struct {
      kv_index primary_key{&myrecord::primary_key};
      kv_index secondary_1{&myrecord::secondary_1};
      kv_index secondary_2{&myrecord::secondary_2};
   } index;

   mytable(eosio::name contract_name) {
      init(contract_name, &index);
   }
}
```

### make_insensitive Example:

```cpp
struct location {
   std::string city;
   std::string state;
   uint64_t zip_code;

   auto icity() { return eosio::make_insensitive(city); }
   auto istate() { return eosio::make_insensitive(state); }
}

DEFINE_TABLE(my_table, location, "testtable", "eosio.kvram",
   icity,
   istate,
   zip_code
)
```

### kv_table Example:
```cpp
#include <eosiolib/eosio.hpp>
using namespace eosio;

struct address {
   uint64_t account_name;
   string first_name;
   string last_name;
   string street;
   string city;
   string state;

   auto full_name() { return first_name + " " + last_name; }
}

struct address_table : kv_table<address_table, myrecord, "testtable"_n> {
   struct {
      kv_index account_name{&address::account_name};
      kv_index full_name{&address::full_name};
   } index;

   addresses(eosio::name contract_name) {
      init(contract_name, &index);
   }
}

class addressbook : contract {
   public:
      void myaction() {
         auto addresses = address_table::open("mycontract"_n);
      }
}
```

### iterator::value Example:
```cpp
// This assumes the code from the class example.
   void myaction() {
      // add dan account to table           - see put example
      auto dan = addresses.index.full_name.find("Dan Larimer");
      eosio::check(dan != addresses.index.full_name.end(), "Dan Larimer is not in the table");
      eosio::check(dan.value().city == "Blacksburg", "Got the wrong value");
   }
```

### kv_index::find Example:
```cpp
   // This assumes the code from the class example.
   void myaction() {
      // add dan account to table           - see put example
      auto itr = addresses.index.account_name.find("dan"_n);
      eosio::check(itr != addresses.index.account_name.end(), "Couldn't get him.");
    
      auto itr = addresses.index.full_name.find("Dan Larimer");
      eosio::check(itr != addresses.index.full_name.end(), "Couldn't get him.");
   }
```

### kv_index::end Example:
```cpp
   // This assumes the code from the class example.
   void myaction() {
      // add dan account to table           - see put example
      auto itr = addresses.index.account_name.find("brendan"_n);
      eosio::check(itr == addresses.index.account_name.end(), "brendan should not be in the table.");
    
      auto itr = addresses.index.full_name.find("Brendan Blumer");
      eosio::check(itr == addresses.index.full_name.end(), "Brendan Blumer should not be in the table.");
   }
```

### kv_index::begin Example:
```cpp
   // This assumes the code from the class example.
   void myaction() {
     // add dan account to table           - see put example
     auto itr = addresses.index.account_name.find("dan"_n);
     eosio::check(itr == addresses.index.account_name.begin(), "dan should be at the beginning.");
   
     auto itr = addresses.index.full_name.find("Dan Larimer");
     eosio::check(itr == addresses.index.full_name.begin(), "Dan Larimer should be at the beginning.");
   }
```

### kv_index::range Example:
```cpp
   // This assumes the code from the class example.
   void myaction() {
     // add dan account to table           - see put example
     // add brendan account to table       - see put example
     // add john account to table          - see put example
   
     address dan = {...}
     address brendan = {...}
     address john = {...}
   
     std::vector expected_values = {brendan, dan, john};
     auto values = addresses.index.account_name.range("brendan"_n, "john"_n);
     eosio::check(values == expected_values, "Did not get the expected values");
   
     std::vector expected_values = {dan};
     auto values = addresses.index.account_name.range("dan"_n, "dan"_n);
     eosio::check(values == expected_values, "Did not get the expected values");
   }
```

### null_kv_index Example:
```cpp
// Original table:
DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram",
   primary_key,
   secondary_1,
   secondary_2
)

struct my_table : kv_table<my_table, my_struct, "testtable"_n, "eosio.kvram"_n> {
  struct {
      kv_index primary_key ...;
      kv_index secondary_1 ...;
      kv_index secondary_2 ...;
  } index;

  my_table(eosio::name contract_name) {
      init(contract_name, &index);
  }
}

// Table with secondary_1 deleted:
DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram",
   primary_key,
   nullptr
   secondary_2
)

struct my_table : kv_table<my_table, my_struct, "testtable"_n, "eosio.kvram"_n> {
  struct {
      kv_index primary_key ...;
      null_kv_index nullptr1 ...;
      kv_index secondary_2 ...;
  } index;

  my_table(eosio::name contract_name) {
      init(contract_name, &index);
  }
}
```

### kv_table::put Example:
```cpp
// This assumes the code from the class example.
void myaction() {
   auto addresses = address_table::open("mycontract"_n);
   addresses.put({
      .account_name = "dan"_n,
      .first_name = "Daniel",
      .last_name = "Larimer",
      .street = "1 EOS Way",
      .city = "Blacksburg",
      .state = "VA"
   });
}
```

### kv_table::erase Example:
```cpp
// This assumes the code from the class example.
void myaction() {
   auto addresses = address_table::open("mycontract"_n);

   auto itr = addresses.index.account_name.find("dan"_n);
   eosio::check(itr != addresses.index.account_name.end());
   addresses.erase("dan"_n);
}
```

### kv_table::open Example:
```cpp
struct myrecord {
   std::string                primary_key;
}

DEFINE_TABLE(mytable, myrecord, "testtable", primary_key)

class mycontract : contract {
   public:
      void myaction() {
         auto t = mytable::open("mycontract"_n);
      }
}
```
