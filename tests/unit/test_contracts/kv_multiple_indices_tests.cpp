#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;
   std::string foo;
   uint64_t bar;

   bool operator==(const my_struct b) const {
      return primary_key == b.primary_key &&
             foo == b.foo &&
             bar == b.bar;
   }
};

DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram",
      primary_key,
      foo,
      bar
)

DEFINE_TABLE(my_table_2, my_struct, "testtable", "eosio.kvram",
      primary_key,
      nullptr,
      bar
)

struct my_table_idx : eosio::kv_table<my_table_idx, my_struct, "testtable"_n, "eosio.kvram"_n> {
   struct {
      kv_index primary_key{"prim"_n, &my_struct::primary_key};
      kv_index foo{"f"_n, &my_struct::foo};
   } index;

   my_table_idx(eosio::name contract_name) {
      init(contract_name, &index);
   }
};

struct my_table_idx_err : eosio::kv_table<my_table_idx_err, my_struct, "testtable"_n, "eosio.kvram"_n> {
   struct {
      kv_index primary_key{"prim"_n, &my_struct::primary_key};
      kv_index foo{&my_struct::foo};
   } index;

   my_table_idx_err(eosio::name contract_name) {
      init(contract_name, &index);
   }
};

struct my_table_idx_err_2 : eosio::kv_table<my_table_idx_err_2, my_struct, "testtable"_n, "eosio.kvram"_n> {
   struct {
      kv_index primary_key{&my_struct::primary_key};
      kv_index foo{"f"_n, &my_struct::foo};
   } index;

   my_table_idx_err_2(eosio::name contract_name) {
      init(contract_name, &index);
   }
};

class [[eosio::contract]] kv_multiple_indices_tests : public eosio::contract {
public:
   using contract::contract;
   my_struct s1{
      .primary_key = "bob"_n,
      .foo = "a",
      .bar = 5
   };
   my_struct s2{
      .primary_key = "alice"_n,
      .foo = "C",
      .bar = 4
   };
   my_struct s3{
      .primary_key = "john"_n,
      .foo = "e",
      .bar = 3
   };
   my_struct s4{
      .primary_key = "joe"_n,
      .foo = "g",
      .bar = 2
   };
   my_struct s5{
      .primary_key = "billy"_n,
      .foo = "I",
      .bar = 1
   };

   [[eosio::action]]
   void setup() {
      my_table t = my_table::open("kvtest"_n);

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
   void find() {
      my_table t = my_table::open("kvtest"_n);

      auto itr = t.index.primary_key.find("bob"_n);
      auto val = itr.value();
      eosio::check(val.primary_key == "bob"_n, "Got the wrong primary_key");

      itr = t.index.foo.find("C");
      val = itr.value();
      eosio::check(val.primary_key == "alice"_n, "Got the wrong primary_key");

      itr = t.index.bar.find((uint64_t)1);
      val = itr.value();
      eosio::check(val.primary_key == "billy"_n, "Got the wrong primary_key");
   }

   [[eosio::action]]
   void finderror() {
      my_table t = my_table::open("kvtest"_n);

      auto itr = t.index.primary_key.find("C");
      auto val = itr.value();
   }

   [[eosio::action]]
   void iteration() {
      my_table t = my_table::open("kvtest"_n);

      auto foo_begin_itr = t.index.foo.begin();
      auto foo_end_itr = t.index.foo.end();

      auto bar_begin_itr = t.index.bar.begin();
      auto bar_end_itr = t.index.bar.end();

      auto foo_itr = t.index.foo.begin();
      auto bar_itr = t.index.bar.begin();

      eosio::check(foo_itr != foo_end_itr, "Should not be the end");
      eosio::check(bar_itr != bar_end_itr, "Should not be the end");
      eosio::check(foo_itr.value().foo == s2.foo, "Got the wrong value");
      eosio::check(bar_itr.value().bar == s5.bar, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value().foo == s5.foo, "Got the wrong value");
      eosio::check(bar_itr.value().bar == s4.bar, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value().foo == s1.foo, "Got the wrong value");
      eosio::check(bar_itr.value().bar == s3.bar, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value().foo == s3.foo, "Got the wrong value");
      eosio::check(bar_itr.value().bar == s2.bar, "Got the wrong value");

      ++foo_itr;
      ++bar_itr;
      eosio::check(foo_itr.value().foo == s4.foo, "Got the wrong value");
      eosio::check(bar_itr.value().bar == s1.bar, "Got the wrong value");

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
};
