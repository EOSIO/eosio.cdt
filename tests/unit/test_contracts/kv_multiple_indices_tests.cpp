#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;
   std::string foo;
   uint64_t bar;
   int32_t baz;
   uint128_t i128;

   auto pk() const { return eosio::make_key(primary_key); }
   auto foo_key() const { return eosio::make_key(foo); }
   auto bar_key() const { return eosio::make_key(bar); }
   auto baz_key() const { return eosio::make_key(baz); }
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
   kv_index primary_index{eosio::name{"primary"}, &my_struct::pk};
   kv_index foo_index{eosio::name{"foo"}, &my_struct::foo_key};
   kv_index bar_index{eosio::name{"bar"}, &my_struct::bar_key};
   kv_index baz_index{eosio::name{"baz"}, &my_struct::baz_key};
   kv_index ifoo_index{eosio::name{"ifoo"}, &my_struct::foo_i_key};
   kv_index i128_index{eosio::name{"ia"}, &my_struct::i128_key};

   my_table() {
      init(eosio::name{"kvtest"}, &primary_index, &foo_index, &bar_index, &baz_index, &ifoo_index, &i128_index);
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
      .i128 = (static_cast<uint128_t>(1) << 127) - 5
   };
   my_struct s2{
      .primary_key = "alice"_n,
      .foo = "C",
      .bar = 4,
      .baz = -1,
      .i128 = (static_cast<uint128_t>(1) << 127) - 4
   };
   my_struct s3{
      .primary_key = "john"_n,
      .foo = "e",
      .bar = 3,
      .baz = -2,
      .i128 = (static_cast<uint128_t>(1) << 127) - 3
   };
   my_struct s4{
      .primary_key = "joe"_n,
      .foo = "g",
      .bar = 2,
      .baz = 1,
      .i128 = (static_cast<uint128_t>(1) << 127) - 2
   };
   my_struct s5{
      .primary_key = "billy"_n,
      .foo = "I",
      .bar = 1,
      .baz = 2,
      .i128 = (static_cast<uint128_t>(1) << 127) - 1
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

      auto itr = t.primary_index.find("bob"_n);
      auto val = itr.value();
      eosio::check(val.primary_key == "bob"_n, "Got the wrong primary_key");

      itr = t.foo_index.find("C");
      val = itr.value();
      eosio::check(val.primary_key == "alice"_n, "Got the wrong primary_key");

      itr = t.bar_index.find((uint64_t)1);
      val = itr.value();
      eosio::check(val.primary_key == "billy"_n, "Got the wrong primary_key");

      itr = t.baz_index.find(0);
      val = itr.value();
      eosio::check(val.primary_key == "bob"_n, "Got the wrong primary_key");

      itr = t.baz_index.find(-1);
      val = itr.value();
      eosio::check(val.primary_key == "alice"_n, "Got the wrong primary_key");

      itr = t.baz_index.find(2);
      val = itr.value();
      eosio::check(val.primary_key == "billy"_n, "Got the wrong primary_key");

      itr = t.baz_index.find(1);
      val = itr.value();
      eosio::check(val.primary_key == "joe"_n, "Got the wrong primary_key");

      itr = t.baz_index.find(-2);
      val = itr.value();
      eosio::check(val.primary_key == "john"_n, "Got the wrong primary_key");
   }

   [[eosio::action]]
   void findi() {
      my_table t;

      auto end_itr = t.i128_index.end();

      auto itr = t.i128_index.begin();
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
   void iteration() {
      my_table t;

      auto begin_itr = t.foo_index.begin();
      auto end_itr = t.foo_index.end();

      // operator++ (case sensitive string)
      // ----------
      auto itr = t.foo_index.begin();
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
      itr = t.baz_index.begin();
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

      auto begin_itr = t.ifoo_index.begin();
      auto end_itr = t.ifoo_index.end();

      // operator++ (case insensitive string)
      // ----------
      auto itr = t.ifoo_index.begin();
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
      auto vals = t.bar_index.range(b, e);
      eosio::check(vals == expected, "range did not return expected vector");

      expected = {s3};
      vals = t.bar_index.range(e, e);
      eosio::check(vals == expected, "range did not return expected vector");
   }
};
