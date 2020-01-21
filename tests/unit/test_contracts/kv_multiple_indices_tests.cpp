#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;
   std::string foo;
   uint64_t bar;
   int32_t baz;
   uint128_t i128;
   float test_float;

   struct {
      uint16_t a;
      uint16_t b;
   } tstruct;

   bool operator==(const my_struct b) const {
      return primary_key == b.primary_key &&
             foo == b.foo &&
             bar == b.bar &&
             baz == b.baz &&
             i128 == b.i128;
   }
};

DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram",
      primary_key,
      foo,
      bar,
      baz,
      i128,
      test_float,
      tstruct
)

class [[eosio::contract]] kv_multiple_indices_tests : public eosio::contract {
public:
   using contract::contract;
   my_struct s{
      .primary_key = "bob"_n,
      .foo = "a",
      .bar = 5,
      .baz = 0,
      .i128 = (static_cast<uint128_t>(1) << 127) - 5,
      .test_float = 4.2574,
      .tstruct = { 1, 2 }
   };
   my_struct s2{
      .primary_key = "alice"_n,
      .foo = "C",
      .bar = 4,
      .baz = -1,
      .i128 = (static_cast<uint128_t>(1) << 127) - 4,
      .test_float = 5.2574,
      .tstruct = { 5, 6 }
   };
   my_struct s3{
      .primary_key = "john"_n,
      .foo = "e",
      .bar = 3,
      .baz = -2,
      .i128 = (static_cast<uint128_t>(1) << 127) - 3,
      .test_float = 187234,
      .tstruct = { 3, 4 }
   };
   my_struct s4{
      .primary_key = "joe"_n,
      .foo = "g",
      .bar = 2,
      .baz = 1,
      .i128 = (static_cast<uint128_t>(1) << 127) - 2,
      .test_float = 0,
      .tstruct = { 7, 8 }
   };
   my_struct s5{
      .primary_key = "billy"_n,
      .foo = "I",
      .bar = 1,
      .baz = 2,
      .i128 = (static_cast<uint128_t>(1) << 127) - 1,
      .test_float = -4.2574,
      .tstruct = { 9, 10 }
   };

   [[eosio::action]]
   void setup() {
      my_table t{"kvtest"_n};

      t.put(s);
      t.put(s2);
      t.put(s3);
      t.put(s4);
      t.put(s5);
   }

   [[eosio::action]]
   void find() {
      my_table t{"kvtest"_n};

      auto itr = t.index.primary_key.find("bob"_n);
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
      my_table t{"kvtest"_n};

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
      my_table t{"kvtest"_n};
      auto end_itr = t.index.test_float.end();

      auto itr = t.index.test_float.begin();
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
   void finds() {
      my_table t{"kvtest"_n};
      auto end_itr = t.index.tstruct.end();

      auto itr = t.index.tstruct.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().tstruct.a == s.tstruct.a, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct.a == s3.tstruct.a, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct.a == s2.tstruct.a, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct.a == s4.tstruct.a, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct.a == s5.tstruct.a, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void iteration() {
      my_table t{"kvtest"_n};

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
   void range() {
      my_table t{"kvtest"_n};

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
