#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;
   std::string foo;
   uint64_t bar;

   std::string fullname;
   uint32_t age;

   std::tuple<std::string, uint32_t> non_unique_name() const { return {fullname, age}; }

   bool operator==(const my_struct b) const {
      return primary_key == b.primary_key &&
             foo == b.foo &&
             bar == b.bar &&
             fullname == b.fullname &&
             age == b.age;
   }
};

struct my_table : eosio::kv_table<my_struct> {
   index<eosio::name> primary_key{&my_struct::primary_key};
   index<std::string> foo{&my_struct::foo};
   index<uint64_t>    bar{&my_struct::bar};

   index<std::tuple<std::string, uint32_t>> non_unique_name{&my_struct::non_unique_name};

   my_table(eosio::name contract_name) {
      init(contract_name, "testtable"_n, "eosio.kvram"_n, &primary_key, &foo, &bar, &non_unique_name);
   }
};

struct my_table2 : eosio::kv_table<my_struct> {
   index<eosio::name> primary_key{&my_struct::primary_key};
   null_index         nullptr_2;
   index<uint64_t>    bar{&my_struct::bar};

   my_table2(eosio::name contract_name) {
      init(contract_name, "testtable"_n, "eosio.kvram"_n, &primary_key, &nullptr_2, &bar);
   }
};

struct my_table_idx : eosio::kv_table<my_struct> {
   index<eosio::name> primary_key{"prim"_n, &my_struct::primary_key};
   index<std::string> foo{"f"_n, &my_struct::foo};

   my_table_idx(eosio::name contract_name) {
      init(contract_name, "testtable"_n, "eosio.kvram"_n, &primary_key, &foo);
   }
};

struct my_table_idx_err : eosio::kv_table<my_struct> {
   index<eosio::name> primary_key{"prim"_n, &my_struct::primary_key};
   index<std::string> foo{&my_struct::foo};

   my_table_idx_err(eosio::name contract_name) {
      init(contract_name, "testtable"_n, "eosio.kvram"_n, &primary_key, &foo);
   }
};

struct my_table_idx_err_2 : eosio::kv_table<my_struct> {
   index<eosio::name> primary_key{&my_struct::primary_key};
   index<std::string> foo{"f"_n, &my_struct::foo};

   my_table_idx_err_2(eosio::name contract_name) {
      init(contract_name, "testtable"_n, "eosio.kvram"_n, &primary_key, &foo);
   }
};

class [[eosio::contract]] kv_multiple_indices_tests : public eosio::contract {
public:
   using contract::contract;
   my_struct s1{
      .primary_key = "bob"_n,
      .foo = "a",
      .bar = 5,
      .fullname = "Bob Smith",
      .age = 25
   };
   my_struct s2{
      .primary_key = "alice"_n,
      .foo = "C",
      .bar = 4,
      .fullname = "Alice Smith",
      .age = 100
   };
   my_struct s3{
      .primary_key = "john"_n,
      .foo = "e",
      .bar = 3,
      .fullname = "John Smith",
      .age = 42
   };
   my_struct s4{
      .primary_key = "joe"_n,
      .foo = "g",
      .bar = 2,
      .fullname = "Bob Smith",
      .age = 47
   };
   my_struct s5{
      .primary_key = "billy"_n,
      .foo = "I",
      .bar = 1,
      .fullname = "Bob Smith",
      .age = 26
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

   [[eosio::action]]
   void indices() {
      my_table_idx t{"kvtest"_n};
   }

   [[eosio::action]]
   void indiceserr() {
      my_table_idx_err t{"kvtest"_n};
   }

   [[eosio::action]]
   void indiceserr2() {
      my_table_idx_err_2 t{"kvtest"_n};
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

      eosio::check(foo_itr != foo_end_itr, "Should not be the end");
      eosio::check(bar_itr != bar_end_itr, "Should not be the end");
      eosio::check(foo_itr.value() == s2, "Got the wrong value");
      eosio::check(bar_itr.value() == s5, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s5, "Got the wrong value");
      eosio::check(bar_itr.value() == s4, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s1, "Got the wrong value");
      eosio::check(bar_itr.value() == s3, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s3, "Got the wrong value");
      eosio::check(bar_itr.value() == s2, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value() == s4, "Got the wrong value");
      eosio::check(bar_itr.value() == s1, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr == foo_end_itr, "Should be the end");
      eosio::check(bar_itr == bar_end_itr, "Should be the end");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "Should not be the beginning");
      eosio::check(bar_itr != bar_begin_itr, "Should not be the beginning");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "Should not be the beginning");
      eosio::check(bar_itr != bar_begin_itr, "Should not be the beginning");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "Should not be the beginning");
      eosio::check(bar_itr != bar_begin_itr, "Should not be the beginning");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr != foo_begin_itr, "Should not be the beginning");
      eosio::check(bar_itr != bar_begin_itr, "Should not be the beginning");

      --foo_itr;
      --bar_itr;
      eosio::check(foo_itr == foo_begin_itr, "Should be the beginning");
      eosio::check(bar_itr == bar_begin_itr, "Should be the beginning");
   }

   [[eosio::action]]
   void nonunique() {
      my_table t{"kvtest"_n};

      std::vector<my_struct> expected{s1, s5, s4};
      auto vals = t.non_unique_name.range({"Bob Smith", 0}, {"Bob Smith", UINT_MAX});

      eosio::check(vals == expected, "Range did not return the expected vector.");

      expected = {s1, s5};
      vals = t.non_unique_name.range({"Bob Smith", 0}, {"Bob Smith", 27});

      eosio::check(vals == expected, "Range did not return the expected vector.");
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
      });
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
      });
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
      });
   }
};
