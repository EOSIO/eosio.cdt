#include <eosio/eosio.hpp>

class [[eosio::contract]] key_value : public eosio::contract {
public:
   using contract::contract;

   struct my_struct {
      eosio::name primary_key;
      std::string foo;
      uint64_t bar;

      std::string fullname;
      uint32_t age;

      std::pair<int, int> a;
      std::optional<float> b;
      std::list<double> c;
      std::vector<int> d;

      eosio::non_unique<std::string, uint32_t> non_unique_name;

      bool operator==(const my_struct& b) const {
         return primary_key == b.primary_key &&
                foo == b.foo &&
                bar == b.bar &&
                fullname == b.fullname &&
                age == b.age;
      }
   };

   struct [[eosio::table]] my_table : eosio::kv_table<my_struct, "testtable"_n> {
      KV_NAMED_INDEX("primarykey"_n, primary_key)
      KV_NAMED_INDEX("foo"_n, foo)
      index<uint64_t> bar{eosio::name{"bar"_n}, &value_type::bar};
      KV_NAMED_INDEX("nonuniqnme"_n, non_unique_name)
      KV_NAMED_INDEX("age"_n, age)

      my_table(eosio::name contract_name) {
         init(contract_name, primary_key, foo, bar, non_unique_name, age);
      }
   };

   [[eosio::action]]   
   void noop() {}

   #if 0
   my_struct s1{
      .primary_key = "bob"_n,
      .foo = "a",
      .bar = 5,
      .fullname = "Bob Smith",
      .age = 25,
      .non_unique_name = {"Bob Smith", 25}
   };
   my_struct s2{
      .primary_key = "alice"_n,
      .foo = "C",
      .bar = 4,
      .fullname = "Alice Smith",
      .age = 100,
      .non_unique_name = {"Alice Smith", 100}
   };
   my_struct s3{
      .primary_key = "john"_n,
      .foo = "e",
      .bar = 3,
      .fullname = "John Smith",
      .age = 42,
      .non_unique_name = {"John Smith", 42}
   };
   my_struct s4{
      .primary_key = "joe"_n,
      .foo = "g",
      .bar = 2,
      .fullname = "Bob Smith",
      .age = 47,
      .non_unique_name = {"Bob Smith", 47}
   };
   my_struct s5{
      .primary_key = "billy"_n,
      .foo = "I",
      .bar = 1,
      .fullname = "Bob Smith",
      .age = 26,
      .non_unique_name = {"Bob Smith", 26}
   };

   [[eosio::action]]
   void setup() {
      my_table t{"kvtest"_n};

      t.put(s1);
      t.put(s2);
      t.put(s3);
      t.put(s4);
      t.put(s5);
   }
   #endif
};
