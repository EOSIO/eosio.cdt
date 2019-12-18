#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name n1;
   eosio::name n2;
   std::string foo;
   std::string bar;

   auto primary_key() const { return eosio::make_key(n1); }

   bool operator==(const my_struct b) const {
      return n1 == b.n1 &&
             n2 == b.n2 &&
             foo == b.foo &&
             bar == b.bar;
   }
};

struct my_table : eosio::kv_table<my_struct> {
   eosio::kv_table<my_struct>::index primary_index{eosio::name{"primary"}, &my_struct::primary_key};

   my_table() {
      init(eosio::name{"kvtest"}, eosio::name{"table"}, &primary_index);
   }
};

class [[eosio::contract]] kv_single_index_tests : public eosio::contract {
public:
   using contract::contract;

   my_struct s{
      .n1 = "bob"_n,
      .n2 = "alice"_n,
      .foo = "a",
      .bar = "b"
   };
   my_struct s2{
      .n1 = "alice"_n,
      .n2 = "bob"_n,
      .foo = "c",
      .bar = "d"
   };
   my_struct s3{
      .n1 = "john"_n,
      .n2 = "joe"_n,
      .foo = "e",
      .bar = "f"
   };
   my_struct s4{
      .n1 = "joe"_n,
      .n2 = "john"_n,
      .foo = "g",
      .bar = "h"
   };
   my_struct s5{
      .n1 = "billy"_n,
      .n2 = "vincent"_n,
      .foo = "i",
      .bar = "j"
   };

   [[eosio::action]]
   void setup() {
      my_table t;

      t.upsert(s3);
      t.upsert(s);
      t.upsert(s4);
      t.upsert(s2);
      t.upsert(s5);
   }

   [[eosio::action]]
   void find() {
      my_table t;
      auto end_itr = t.primary_index.end();

      auto itr = t.primary_index.find("bob"_n);
      auto val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "bob"_n, "Got the wrong n1");
      eosio::check(val.n2 == "alice"_n, "Got the wrong n2");

      itr = t.primary_index.find("joe"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "joe"_n, "Got the wrong n1");
      eosio::check(val.n2 == "john"_n, "Got the wrong n2");

      itr = t.primary_index.find("alice"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "alice"_n, "Got the wrong n1");
      eosio::check(val.n2 == "bob"_n, "Got the wrong n2");

      itr = t.primary_index.find("john"_n);
      val = itr.value();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(val.n1 == "john"_n, "Got the wrong n1");
      eosio::check(val.n2 == "joe"_n, "Got the wrong n2");
   }

   [[eosio::action]]
   void finderror() {
      my_table t;
      auto itr = t.primary_index.find("larry"_n);
      auto val = itr.value();
   }

   [[eosio::action]]
   void iteration() {
      my_table t;
      auto begin_itr = t.primary_index.begin();
      auto end_itr = t.primary_index.end();

      // operator++
      // ----------
      auto itr = t.primary_index.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "alice"_n, "Got the wrong beginning");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "billy"_n, "Got the wrong value");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "bob"_n, "Got the wrong value");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "joe"_n, "Got the wrong value");
      itr++;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().n1 == "john"_n, "Got the wrong value");
      itr++;
      eosio::check(itr == end_itr, "Should be the end");

      // operator--
      // ----------
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr != begin_itr, "Should not be the beginning");
      itr--;
      eosio::check(itr == begin_itr, "Should be the beginning");
   }

   [[eosio::action]]
   void range() {
      my_table t;

      std::vector<my_struct> expected{s, s4, s3};
      auto vals = t.primary_index.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");

      expected = {s};
      vals = t.primary_index.range("bob"_n, "bob"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }

   [[eosio::action]]
   void rangeerror() {
      my_table t;
      std::vector<my_struct> expected = {s4, s3, s2};
      auto vals = t.primary_index.range("joe"_n, "alice"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }

   [[eosio::action]]
   void erase() {
      my_table t;
      auto end_itr = t.primary_index.end();

      t.erase("joe"_n);
      auto itr = t.primary_index.find("joe"_n);
      eosio::check(itr == end_itr, "key was not properly deleted");

      std::vector<my_struct> expected = {s, s3};
      auto vals = t.primary_index.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }
};
