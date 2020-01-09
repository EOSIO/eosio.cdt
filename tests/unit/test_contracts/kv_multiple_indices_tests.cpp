#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;
   std::string foo;
   uint64_t bar;
   int32_t baz;
   uint128_t i128;
   float test_float;

   auto foo_i_key() const { return eosio::make_key(foo, true); }
   auto i128_key() const { return eosio::make_key(i128); }

   bool operator==(const my_struct b) const {
      return primary_key == b.primary_key &&
             foo == b.foo &&
             bar == b.bar &&
             baz == b.baz &&
             i128 == b.i128;
   }
};

struct my_table : eosio::kv_table<my_struct, "testtable"_n> {
   struct {
      kv_index primary{eosio::name{"primary"}, &my_struct::primary_key};
      kv_index foo{eosio::name{"foo"}, &my_struct::foo};
      kv_index bar{eosio::name{"bar"}, &my_struct::bar};
      kv_index baz{eosio::name{"baz"}, &my_struct::baz};
      kv_index i128{eosio::name{"ia"}, &my_struct::i128};
      kv_index ifoo{eosio::name{"ifoo"}, &my_struct::foo_i_key};
      kv_index flt{eosio::name{"float"}, &my_struct::test_float};
   } index;

   my_table() {
      init(eosio::name{"kvtest"}, &index);
   }
};

class [[eosio::contract]] kv_multiple_indices_tests : public eosio::contract {
public:
   using contract::contract;
   my_struct s{
      .primary_key = "bob"_n,
      .foo = "a",
      .bar = 5,
      .baz = 0,
      .i128 = (static_cast<uint128_t>(1) << 127) - 5,
      .test_float = 4.2574
   };
   my_struct s2{
      .primary_key = "alice"_n,
      .foo = "C",
      .bar = 4,
      .baz = -1,
      .i128 = (static_cast<uint128_t>(1) << 127) - 4,
      .test_float = 5.2574
   };
   my_struct s3{
      .primary_key = "john"_n,
      .foo = "e",
      .bar = 3,
      .baz = -2,
      .i128 = (static_cast<uint128_t>(1) << 127) - 3,
      .test_float = 187234
   };
   my_struct s4{
      .primary_key = "joe"_n,
      .foo = "g",
      .bar = 2,
      .baz = 1,
      .i128 = (static_cast<uint128_t>(1) << 127) - 2,
      .test_float = 0
   };
   my_struct s5{
      .primary_key = "billy"_n,
      .foo = "I",
      .bar = 1,
      .baz = 2,
      .i128 = (static_cast<uint128_t>(1) << 127) - 1,
      .test_float = -4.2574
   };

   [[eosio::action]]
   void setup() {
      my_table t;

      t.put(s);
      t.put(s2);
      t.put(s3);
      t.put(s4);
      t.put(s5);
   }

   [[eosio::action]]
   void find() {
      my_table t;

      auto itr = t.index.primary.find("bob"_n);
      auto val = itr.value();
      eosio::check(val.primary_key == "bob"_n, "Got the wrong primary_key");

      itr = t.index.foo.find("C");
      val = itr.value();
      eosio::check(val.primary_key == "alice"_n, "Got the wrong primary_key");

      itr = t.index.bar.find((uint64_t)1);
      val = itr.value();
      eosio::check(val.primary_key == "billy"_n, "Got the wrong primary_key");

      itr = t.index.baz.find(0);
      val = itr.value();
      eosio::check(val.primary_key == "bob"_n, "Got the wrong primary_key");

      itr = t.index.baz.find(-1);
      val = itr.value();
      eosio::check(val.primary_key == "alice"_n, "Got the wrong primary_key");

      itr = t.index.baz.find(2);
      val = itr.value();
      eosio::check(val.primary_key == "billy"_n, "Got the wrong primary_key");

      itr = t.index.baz.find(1);
      val = itr.value();
      eosio::check(val.primary_key == "joe"_n, "Got the wrong primary_key");

      itr = t.index.baz.find(-2);
      val = itr.value();
      eosio::check(val.primary_key == "john"_n, "Got the wrong primary_key");
   }

   [[eosio::action]]
   void findi() {
      my_table t;

      auto end_itr = t.index.i128.end();

      auto itr = t.index.i128.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().i128 == s.i128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().i128 == s2.i128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().i128 == s3.i128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().i128 == s4.i128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().i128 == s5.i128, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void findf() {
      my_table t;
      auto end_itr = t.index.flt.end();

      auto itr = t.index.flt.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().test_float == s5.test_float, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().test_float == s4.test_float, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().test_float == s.test_float, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().test_float == s2.test_float, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().test_float == s3.test_float, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void iteration() {
      my_table t;

      auto begin_itr = t.index.foo.begin();
      auto end_itr = t.index.foo.end();

      // operator++ (case sensitive string)
      // ----------
      auto itr = t.index.foo.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().foo == "C", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "I", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "a", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "e", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "g", "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");

      // operator-- (case sensitive string)
      // ----------
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr == begin_itr, "Should be the beginning");

      // operator int32_t
      // ----------------
      itr = t.index.baz.begin();
      eosio::check(itr.value().baz == -2, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().baz == -1, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().baz == 0, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().baz == 1, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().baz == 2, "Got the wrong value");
   }

   [[eosio::action]]
   void iterationi() {
      my_table t;

      auto begin_itr = t.index.ifoo.begin();
      auto end_itr = t.index.ifoo.end();

      // operator++ (case insensitive string)
      // ----------
      auto itr = t.index.ifoo.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().foo == "a", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "C", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "e", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "g", "Got the wrong value");
      ++itr;
      eosio::check(itr.value().foo == "I", "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");

      // operator-- (case sensitive string)
      // ----------
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      --itr;
      eosio::check(itr == begin_itr, "Should be the beginning");
   }

   [[eosio::action]]
   void range() {
      my_table t;

      std::vector<my_struct> expected{s5, s4, s3};
      uint64_t b = 1;
      uint64_t e = 3;
      auto vals = t.index.bar.range(b, e);
      eosio::check(vals == expected, "range did not return expected vector");

      expected = {s3};
      vals = t.index.bar.range(e, e);
      eosio::check(vals == expected, "range did not return expected vector");
   }
};
