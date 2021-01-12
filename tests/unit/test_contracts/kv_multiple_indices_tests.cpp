#include <eosio/eosio.hpp>
#include <eosio/table.hpp>

class [[eosio::contract]] kv_multiple_indices_tests : public eosio::contract {
public:
   struct my_struct {
      eosio::name primary_key;
      std::string foo;
      uint64_t bar;

      std::string fullname;
      uint32_t age;

      std::tuple<std::string, uint32_t> non_unique_name;

      bool operator==(const my_struct& b) const {
         return primary_key == b.primary_key &&
                foo == b.foo &&
                bar == b.bar &&
                fullname == b.fullname &&
                age == b.age;
      }
   };

   struct [[eosio::table]] my_table : eosio::kv::table<my_struct, "testtable"_n> {
      KV_NAMED_INDEX("primarykey"_n, primary_key)
      KV_NAMED_INDEX("foo"_n, foo)
      index<uint64_t> bar{eosio::name{"bar"_n}, &value_type::bar};
      KV_NAMED_INDEX("nonuniqnme"_n, non_unique_name)
      KV_NAMED_INDEX("age"_n, age)

      my_table(eosio::name contract_name) {
         init(contract_name, primary_key, foo, bar, non_unique_name, age);
      }
   };

   using contract::contract;
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

      t.put(s1, get_self());
      t.put(s2, get_self());
      t.put(s3, get_self());
      t.put(s4, get_self());
      t.put(s5, get_self());
   }

   [[eosio::action]]
   void get() {
      my_table t{"kvtest"_n};
      auto end_itr = t.foo.end();

      auto val = t.foo.get("g");
      eosio::check(val->primary_key == "joe"_n, "Got the wrong value");
   }

   [[eosio::action]]
   void iteration() {
      my_table t{"kvtest"_n};

      auto foo_begin_itr = t.foo.begin();
      auto foo_end_itr = t.foo.end();

      auto bar_begin_itr = t.bar.begin();
      auto bar_end_itr = t.bar.end();

      auto foo_itr = t.foo.begin();
      auto bar_itr = t.bar.begin();

      eosio::check(foo_itr != foo_end_itr, "foo should not be the end");
      eosio::check(bar_itr != bar_end_itr, "bar should not be the end");

      eosio::check(foo_itr.value() == s2, "Got the wrong value: foo != s2");
      eosio::check(bar_itr.value() == s5, "Got the wrong value: bar != s5");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s5, "Got the wrong value: foo != s5");
      eosio::check(bar_itr.value() == s4, "Got the wrong value: bar != s4");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s1, "Got the wrong value: foo != s1");
      eosio::check(bar_itr.value() == s3, "Got the wrong value: bar != s3");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s3, "Got the wrong value: foo != s3");
      eosio::check(bar_itr.value() == s2, "Got the wrong value: bar != s2");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s4, "Got the wrong value: foo != s4");
      eosio::check(bar_itr.value() == s1, "Got the wrong value: bar != s1");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr == foo_end_itr, "foo should be the end");
      eosio::check(bar_itr == bar_end_itr, "bar should be the end");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "foo should not be the beginning: 1");
      eosio::check(bar_itr != bar_begin_itr, "bar should not be the beginning: 1");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "foo should not be the beginning: 2");
      eosio::check(bar_itr != bar_begin_itr, "bar should not be the beginning: 2");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "foo should not be the beginning: 3");
      eosio::check(bar_itr != bar_begin_itr, "bar should not be the beginning: 3");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "foo should not be the beginning: 4");
      eosio::check(bar_itr != bar_begin_itr, "bar should not be the beginning: 4");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr == foo_begin_itr, "foo should be the beginning");
      eosio::check(bar_itr == bar_begin_itr, "bar should be the beginning");
   }

   [[eosio::action]]
   void nonunique() {
      my_table t{"kvtest"_n};

      std::vector<my_struct> expected{s1, s5, s4};
      auto vals = t.non_unique_name.range({"Bob Smith", 0}, {"Bob Smith", UINT_MAX});

      eosio::check(vals == expected, "Range did not return the expected vector: {s1, s5, s4}");

      expected = {s1, s5};
      vals = t.non_unique_name.range({"Bob Smith", 0}, {"Bob Smith", 27});

      eosio::check(vals == expected, "Range did not return the expected vector: {s1, s5}");
   }

   [[eosio::action]]
   void update() {
      my_table t{"kvtest"_n};

      t.put({
         .primary_key = "bob"_n,
         .foo = "a",
         .bar = 1000,
         .fullname = "Bob Smith",
         .age = 25
      }, get_self());
   }

   [[eosio::action]]
   void updateerr1() {
      my_table t{"kvtest"_n};

      t.put({
         .primary_key = "alice"_n,
         .foo = "a",
         .bar = 1000,
         .fullname = "Bob Smith",
         .age = 25
      }, get_self());
   }

   [[eosio::action]]
   void updateerr2() {
      my_table t{"kvtest"_n};

      t.put({
         .primary_key = "will"_n,
         .foo = "a",
         .bar = 1000,
         .fullname = "Bob Smith",
         .age = 25
      }, get_self());
   }
};
