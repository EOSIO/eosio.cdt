#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;
   eosio::name n2;
   std::string foo;
   std::string bar;

   bool operator==(const my_struct b) const {
      return primary_key == b.primary_key &&
             n2 == b.n2 &&
             foo == b.foo &&
             bar == b.bar;
   }
};

DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram", primary_key)

class [[eosio::contract]] kv_single_index_tests : public eosio::contract {
public:
   using contract::contract;

   my_struct s{
      .primary_key = "bob"_n,
      .n2 = "alice"_n,
      .foo = "a",
      .bar = "b"
   };
   my_struct s2{
      .primary_key = "alice"_n,
      .n2 = "bob"_n,
      .foo = "c",
      .bar = "d"
   };
   my_struct s3{
      .primary_key = "john"_n,
      .n2 = "joe"_n,
      .foo = "e",
      .bar = "f"
   };
   my_struct s4{
      .primary_key = "joe"_n,
      .n2 = "john"_n,
      .foo = "g",
      .bar = "h"
   };
   my_struct s5{
      .primary_key = "billy"_n,
      .n2 = "vincent"_n,
      .foo = "i",
      .bar = "j"
   };

   [[eosio::action]]
   void setup() {
      my_table t{"kvtest"_n};

      t.put(s3);
      t.put(s);
      t.put(s4);
      t.put(s2);
      t.put(s5);
   }

   [[eosio::action]]
   void find() {
      my_table t{"kvtest"_n};
      auto end_itr = t.index.primary_key.end();

      auto itr = t.index.primary_key.find("bob"_n);
      auto val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.primary_key == "bob"_n, "Got the wrong primary_key");
      eosio::check(val.n2 == "alice"_n, "Got the wrong n2");

      itr = t.index.primary_key.find("joe"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.primary_key == "joe"_n, "Got the wrong primary_key");
      eosio::check(val.n2 == "john"_n, "Got the wrong n2");

      itr = t.index.primary_key.find("alice"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.primary_key == "alice"_n, "Got the wrong primary_key");
      eosio::check(val.n2 == "bob"_n, "Got the wrong n2");

      itr = t.index.primary_key.find("john"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.primary_key == "john"_n, "Got the wrong primary_key");
      eosio::check(val.n2 == "joe"_n, "Got the wrong n2");

      itr = t.index.primary_key.find("billy"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.primary_key == "billy"_n, "Got the wrong primary_key");
      eosio::check(val.n2 == "vincent"_n, "Got the wrong n2");
   }

   [[eosio::action]]
   void finderror() {
      my_table t{"kvtest"_n};
      auto itr = t.index.primary_key.find("larry"_n);
      auto val = itr.value();
   }

   [[eosio::action]]
   void iteration() {
      my_table t{"kvtest"_n};
      auto begin_itr = t.index.primary_key.begin();
      auto end_itr = t.index.primary_key.end();

      // operator++
      // ----------
      auto itr = t.index.primary_key.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "alice"_n, "Got the wrong beginning");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "billy"_n, "Got the wrong value");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong value");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong value");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "john"_n, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");

      // operator--
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
   void itrerror1() {
      my_table t{"kvtest"_n};
      auto end_itr = t.index.primary_key.end();
      ++end_itr;
   }

   [[eosio::action]]
   void itrerror2() {
      my_table t{"kvtest"_n};
      auto begin_itr = t.index.primary_key.begin();
      --begin_itr;
   }

   [[eosio::action]]
   void range() {
      my_table t{"kvtest"_n};

      std::vector<my_struct> expected{s, s4, s3};
      auto vals = t.index.primary_key.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");

      expected = {s};
      vals = t.index.primary_key.range("bob"_n, "bob"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }

   [[eosio::action]]
   void rangeerror1() {
      my_table t{"kvtest"_n};
      auto vals = t.index.primary_key.range("joe"_n, "alice"_n);
   }

   [[eosio::action]]
   void rangeerror2() {
      my_table t{"kvtest"_n};
      auto vals = t.index.primary_key.range("chris"_n, "joe"_n);
   }

   [[eosio::action]]
   void rangeerror3() {
      my_table t{"kvtest"_n};
      auto vals = t.index.primary_key.range("alice"_n, "chris"_n);
   }

   [[eosio::action]]
   void erase() {
      my_table t{"kvtest"_n};
      auto end_itr = t.index.primary_key.end();

      t.erase("joe"_n);
      auto itr = t.index.primary_key.find("joe"_n);
      eosio::check(itr == end_itr, "key was not properly deleted");

      std::vector<my_struct> expected = {s, s3};
      auto vals = t.index.primary_key.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }

   [[eosio::action]]
   void eraseerror() {
      my_table t{"kvtest"_n};
      t.erase("chris"_n);
   }
};
