#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;

   bool operator==(const my_struct b) const {
      return primary_key == b.primary_key;
   }
};

DEFINE_TABLE(my_table, my_struct, "testtable", "eosio.kvram", primary_key)

class [[eosio::contract]] kv_single_index_tests : public eosio::contract {
public:
   using contract::contract;

   my_struct s{
      .primary_key = "bob"_n
   };
   my_struct s2{
      .primary_key = "alice"_n
   };
   my_struct s3{
      .primary_key = "john"_n
   };
   my_struct s4{
      .primary_key = "joe"_n
   };
   my_struct s5{
      .primary_key = "billy"_n
   };

   [[eosio::action]]
   void setup() {
      my_table t = my_table::open("kvtest"_n);

      t.put(s3);
      t.put(s);
      t.put(s4);
      t.put(s2);
      t.put(s5);
   }

   [[eosio::action]]
   void find() {
      my_table t = my_table::open("kvtest"_n);
      auto end_itr = t.index.primary_key.end();

      auto itr = t.index.primary_key.find("bob"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong primary_key");

      itr = t.index.primary_key.find("joe"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong primary_key");

      itr = t.index.primary_key.find("alice"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "alice"_n, "Got the wrong primary_key");

      itr = t.index.primary_key.find("john"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "john"_n, "Got the wrong primary_key");

      itr = t.index.primary_key.find("billy"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "billy"_n, "Got the wrong primary_key");
   }

   [[eosio::action]]
   void finderror() {
      my_table t = my_table::open("kvtest"_n);
      auto itr = t.index.primary_key.find("larry"_n);
      auto val = itr.value();
   }

   [[eosio::action]]
   void get() {
      my_table t = my_table::open("kvtest"_n);
      auto end_itr = t.index.primary_key.end();

      auto val = t.index.primary_key.get("bob"_n);
      eosio::check(val->primary_key == "bob"_n, "Got the wrong value");

      val = t.index.primary_key.get("william"_n);
      eosio::check(!val, "Should not have gotten a value");
   }

   [[eosio::action]]
   void bounds() {
      my_table t = my_table::open("kvtest"_n);
      auto end_itr = t.index.primary_key.end();

      auto itr = t.index.primary_key.lower_bound("bob"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong primary_key");

      itr = t.index.primary_key.lower_bound("catherine"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong primary_key");

      itr = t.index.primary_key.lower_bound("william"_n);
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void iteration() {
      my_table t = my_table::open("kvtest"_n);
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
      my_table t = my_table::open("kvtest"_n);
      auto end_itr = t.index.primary_key.end();
      ++end_itr;
   }

   [[eosio::action]]
   void itrerror2() {
      my_table t = my_table::open("kvtest"_n);
      auto begin_itr = t.index.primary_key.begin();
      --begin_itr;
   }

   [[eosio::action]]
   void range() {
      my_table t = my_table::open("kvtest"_n);

      std::vector<my_struct> expected{s, s4, s3};
      auto vals = t.index.primary_key.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");

      expected = {s};
      vals = t.index.primary_key.range("bob"_n, "bob"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }

   [[eosio::action]]
   void rangeerror1() {
      my_table t = my_table::open("kvtest"_n);
      auto vals = t.index.primary_key.range("joe"_n, "alice"_n);
   }

   [[eosio::action]]
   void rangeerror2() {
      my_table t = my_table::open("kvtest"_n);
      auto vals = t.index.primary_key.range("chris"_n, "joe"_n);
   }

   [[eosio::action]]
   void rangeerror3() {
      my_table t = my_table::open("kvtest"_n);
      auto vals = t.index.primary_key.range("alice"_n, "chris"_n);
   }

   [[eosio::action]]
   void erase() {
      my_table t = my_table::open("kvtest"_n);
      auto end_itr = t.index.primary_key.end();

      t.erase("joe"_n);
      auto itr = t.index.primary_key.find("joe"_n);
      eosio::check(itr == end_itr, "key was not properly deleted");

      std::vector<my_struct> expected = {s, s3};
      auto vals = t.index.primary_key.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }
};
