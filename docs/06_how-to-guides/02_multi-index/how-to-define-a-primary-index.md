## How to define a primary index
TO DO: review this content, make sure it is an how to (a receipe not how to cook)

EOSIO has the ability to sort tables by up to 16 indices. In the following section, we're going to add another index to the `addressbook` contract, so we can iterate through the records in a different way. 
[block:api-header]
{
  "title": "Step 1: Remove existing data from table"
}
[/block]
As mentioned earlier, **a table's struct cannot be modified when it has data in it.** The first step allows the removal of the data already added.

Remove all records of alice and bob that were added in previous tutorials. 
[block:code]
{
  "codes": [
    {
      "code": "cleos push action addressbook erase '[\"alice\"]' -p alice@active",
      "language": "shell"
    }
  ]
}
[/block]

[block:code]
{
  "codes": [
    {
      "code": "cleos push action addressbook erase '[\"bob\"]' -p bob@active",
      "language": "shell"
    }
  ]
}
[/block]

[block:api-header]
{
  "title": "Step 2: Add new index member and getter"
}
[/block]
Add a new member variable and its getter to the `addressbook.cpp` contract. Since the secondary index needs to be numeric field so a `uint64_t` age variable is added. 
[block:code]
{
  "codes": [
    {
      "code": "uint64_t age;\nuint64_t get_secondary_1() const { return age;}",
      "language": "cplusplus",
      "name": null
    }
  ]
}
[/block]

[block:api-header]
{
  "title": "Step 3: Add secondary index to `addresses` table configuration"
}
[/block]
A field has been defined as the secondary index, next the  `address_index` table needs to be reconfigured.
[block:code]
{
  "codes": [
    {
      "code": "typedef eosio::multi_index<\"people\"_n, person, \nindexed_by<\"byage\"_n, const_mem_fun<person, uint64_t, &person::get_secondary_1>>\n  > address_index;",
      "language": "cplusplus"
    }
  ]
}
[/block]
In the third parameter, we pass a `index_by` struct which is used to instantiate a index. 

In that `index_by` struct, we specify the name of index as `"byage"` and the second type parameter as a function call operator should extract a const value as an index key. In this case, we point it to the getter we created earlier so this multiple index table will index records by `age` variable.
[block:code]
{
  "codes": [
    {
      "code": "indexed_by<\"byage\"_n, const_mem_fun<person, uint64_t, &person::get_secondary_1>>",
      "language": "cplusplus"
    }
  ]
}
[/block]

[block:api-header]
{
  "title": "Step 4: Modify code"
}
[/block]
With all the changes in previous steps, we can now update the `upsert` function. Change the function parameter list to the following:
[block:code]
{
  "codes": [
    {
      "code": "  void upsert(name user, std::string first_name, std::string last_name, uint64_t age, std::string street, std::string city, std::string state)",
      "language": "cplusplus"
    }
  ]
}
[/block]
Add additional lines to update `age` field in `upsert` function as the following:
[block:code]
{
  "codes": [
    {
      "code": "  void upsert(name user, std::string first_name, std::string last_name, uint64_t age, std::string street, std::string city, std::string state) {\n    require_auth( user );\n    address_index addresses(_code, _code.value);\n    auto iterator = addresses.find(user.value);\n    if( iterator == addresses.end() )\n    {\n      addresses.emplace(user, [&]( auto& row ) {\n       row.key = user;\n       row.first_name = first_name;\n       row.last_name = last_name;\n       // -- Add code below --\n       row.age = age; \n       row.street = street;\n       row.city = city;\n       row.state = state;\n      });\n    }\n    else {\n      std::string changes;\n      addresses.modify(iterator, user, [&]( auto& row ) {\n        row.key = user;\n        row.first_name = first_name;\n        row.last_name = last_name;\n        // -- Add code below --\n        row.age = age;\n        row.street = street;\n        row.city = city;\n        row.state = state;\n      });\n    }\n  }",
      "language": "cplusplus"
    }
  ]
}
[/block]

[block:api-header]
{
  "title": "Step 5: Compile and Deploy"
}
[/block]
Compile
[block:code]
{
  "codes": [
    {
      "code": "eosio-cpp -o addressbook.wasm addressbook.cpp --abigen\n",
      "language": "shell"
    }
  ]
}
[/block]
Deploy
[block:code]
{
  "codes": [
    {
      "code": "cleos set contract addressbook CONTRACTS_DIR/addressbook",
      "language": "shell"
    }
  ]
}
[/block]

[block:api-header]
{
  "title": "Step 6: Test it"
}
[/block]
Insert records
[block:code]
{
  "codes": [
    {
      "code": "cleos push action addressbook upsert '[\"alice\", \"alice\", \"liddell\", 9, \"123 drink me way\", \"wonderland\", \"amsterdam\"]' -p alice@active",
      "language": "shell"
    }
  ]
}
[/block]

[block:code]
{
  "codes": [
    {
      "code": "cleos push action addressbook upsert '[\"bob\", \"bob\", \"is a guy\", 49, \"doesnt exist\", \"somewhere\", \"someplace\"]' -p bob@active",
      "language": "shell"
    }
  ]
}
[/block]
Look up alice's address by the age index. Here the `--index 2` parameter is used to indicate that the query applies to the secondary index (index #2)
[block:code]
{
  "codes": [
    {
      "code": "cleos get table addressbook addressbook people --upper 10 \\\n--key-type i64 \\\n--index 2",
      "language": "shell"
    }
  ]
}
[/block]
You should see something like the following
[block:code]
{
  "codes": [
    {
      "code": "{\n  \"rows\": [{\n      \"key\": \"alice\",\n      \"first_name\": \"alice\",\n      \"last_name\": \"liddell\",\n      \"age\": 9,\n      \"street\": \"123 drink me way\",\n      \"city\": \"wonderland\",\n      \"state\": \"amsterdam\"\n    }\n  ],\n  \"more\": false\n}",
      "language": "json"
    }
  ]
}
[/block]
Look it up by Bob's age
[block:code]
{
  "codes": [
    {
      "code": "cleos get table addressbook addressbook people --upper 50 --key-type i64 --index 2",
      "language": "shell"
    }
  ]
}
[/block]
It should return
[block:code]
{
  "codes": [
    {
      "code": "{\n  \"rows\": [{\n      \"key\": \"alice\",\n      \"first_name\": \"alice\",\n      \"last_name\": \"liddell\",\n      \"age\": 9,\n      \"street\": \"123 drink me way\",\n      \"city\": \"wonderland\",\n      \"state\": \"amsterdam\"\n    },{\n      \"key\": \"bob\",\n      \"first_name\": \"bob\",\n      \"last_name\": \"is a loser\",\n      \"age\": 49,\n      \"street\": \"doesnt exist\",\n      \"city\": \"somewhere\",\n      \"state\": \"someplace\"\n    }\n  ],\n  \"more\": false\n}",
      "language": "json"
    }
  ]
}
[/block]

All good!

[block:api-header]
{
  "title": "Wrapping Up"
}
[/block]
The complete `addressbook` contract up to this point:
[block:code]
{
  "codes": [
    {
      "code": "#include <eosiolib/eosio.hpp>\n#include <eosiolib/print.hpp>\n\nusing namespace eosio;\n\nclass [[eosio::contract]] addressbook : public eosio::contract {\n\npublic:\n  using contract::contract;\n  \n  addressbook(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}\n\n  [[eosio::action]]\n  void upsert(name user, std::string first_name, std::string last_name, uint64_t age, std::string street, std::string city, std::string state) {\n    require_auth( user );\n    address_index addresses(_code, _code.value);\n    auto iterator = addresses.find(user.value);\n    if( iterator == addresses.end() )\n    {\n      addresses.emplace(user, [&]( auto& row ) {\n       row.key = user;\n       row.first_name = first_name;\n       row.last_name = last_name;\n       row.age = age;\n       row.street = street;\n       row.city = city;\n       row.state = state;\n      });\n    }\n    else {\n      std::string changes;\n      addresses.modify(iterator, user, [&]( auto& row ) {\n        row.key = user;\n        row.first_name = first_name;\n        row.last_name = last_name;\n        row.age = age;\n        row.street = street;\n        row.city = city;\n        row.state = state;\n      });\n    }\n  }\n\n  [[eosio::action]]\n  void erase(name user) {\n    require_auth(user);\n\n    address_index addresses(_self, _code.value);\n\n    auto iterator = addresses.find(user.value);\n    eosio_assert(iterator != addresses.end(), \"Record does not exist\");\n    addresses.erase(iterator);\n  }\n\nprivate:\n  struct [[eosio::table]] person {\n    name key;\n    std::string first_name;\n    std::string last_name;\n    uint64_t age;\n    std::string street;\n    std::string city;\n    std::string state;\n  \n    uint64_t primary_key() const { return key.value; }\n    uint64_t get_secondary_1() const { return age;}\n  \n  };\n\n  typedef eosio::multi_index<\"people\"_n, person, indexed_by<\"byage\"_n, const_mem_fun<person, uint64_t, &person::get_secondary_1>>> address_index;\n  \n};\n\nEOSIO_DISPATCH( addressbook, (upsert)(erase))",
      "language": "cplusplus"
    }
  ]
}
[/block]
If you wanted to use the secondary indexes in code, we need to get the index using `get_index()` 
[block:code]
{
  "codes": [
    {
      "code": "[[eosio::action]]\nvoid findByAge(name user, uint64_t age) {\n require_auth(user); \n \n address_index addresses(get_self(), get_first_receiver().value);\n \n auto age_index = addresses.get_index<\"byage\"_n>();\n auto itr = age_index.find(age);\n check(itr != age_index.end(), \"Yes, we have someone with that age\");\n}",
      "language": "cplusplus"
    }
  ]
}
[/block]
