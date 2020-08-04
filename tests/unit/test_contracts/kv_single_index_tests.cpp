#include <eosio/eosio.hpp>

struct my_struct {
   eosio::name primary_key;

   bool operator==(const my_struct b) const {
      return primary_key == b.primary_key;
   }
};

struct my_table : eosio::kv_table<my_struct> {
   KV_NAMED_INDEX("primary"_n, primary_key);

   my_table(eosio::name contract_name) {
      init(contract_name, "testtable"_n, eosio::kv_ram, primary_key);
   }
};

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
      my_table::iterator end_itr = t.primary_key.end();

      auto itr = t.primary_key.find("bob"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong primary_key: bob");

      itr = t.primary_key.find("joe"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong primary_key: joe");

      itr = t.primary_key.find("alice"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "alice"_n, "Got the wrong primary_key: alice");

      itr = t.primary_key.find("john"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "john"_n, "Got the wrong primary_key: john");

      itr = t.primary_key.find("billy"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "billy"_n, "Got the wrong primary_key: billy");
   }

   [[eosio::action]]
   void finderror() {
      my_table t{"kvtest"_n};
      auto itr = t.primary_key.find("larry"_n);
      auto val = itr.value();
   }

   [[eosio::action]]
   void get() {
      my_table t{"kvtest"_n};
      auto end_itr = t.primary_key.end();

      auto val = t.primary_key.get("bob"_n);
      eosio::check(val->primary_key == "bob"_n, "Got the wrong value");

      val = t.primary_key.get("william"_n);
      eosio::check(!val, "Should not have gotten a value");

      eosio::check(t.primary_key.exists("bob"_n), "Exists should return true");
      eosio::check(!t.primary_key.exists("william"_n), "Exists should return false");

      auto vval = t.primary_key["bob"_n];
      eosio::check(vval.primary_key == "bob"_n, "Got the wrong value");
   }

   [[eosio::action]]
   void geterror() {
      my_table t{"kvtest"_n};
      auto val = t.primary_key["william"_n];
   }

   [[eosio::action]]
   void bounds() {
      my_table t{"kvtest"_n};
      auto end_itr = t.primary_key.end();

      auto itr = t.primary_key.lower_bound("bob"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong primary_key: lower_bound - bob");

      itr = t.primary_key.lower_bound("catherine"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong primary_key: lower_bound - joe");

      itr = t.primary_key.lower_bound("william"_n);
      eosio::check(itr == end_itr, "Should be the end");

      itr = t.primary_key.upper_bound("billy"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong primary_key: upper_bound - bob");

      itr = t.primary_key.upper_bound("ian"_n);
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong primary_key: upper_bound - joe");

      itr = t.primary_key.upper_bound("john"_n);
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void iteration() {
      my_table t{"kvtest"_n};
      auto begin_itr = t.primary_key.begin();
      auto end_itr = t.primary_key.end();

      // operator++
      // ----------
      auto itr = t.primary_key.begin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "alice"_n, "Got the wrong beginning");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "billy"_n, "Got the wrong value: billy");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong value: bob");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong value: joe");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "john"_n, "Got the wrong value: john");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");

      // operator--
      // ----------
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 1");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 2");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 3");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 4");
      --itr;
      eosio::check(itr == begin_itr, "Should be the beginning");
   }

   [[eosio::action]]
   void itrerror1() {
      my_table t{"kvtest"_n};
      auto end_itr = t.primary_key.end();
      ++end_itr;
   }

   [[eosio::action]]
   void itrerror2() {
      my_table t{"kvtest"_n};
      auto begin_itr = t.primary_key.begin();
      --begin_itr;
   }

   [[eosio::action]]
   void riteration() {
      my_table t{"kvtest"_n};
      auto begin_itr = t.primary_key.rbegin();
      auto end_itr = t.primary_key.rend();

      // operator++
      // ----------
      auto itr = t.primary_key.rbegin();
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "john"_n, "Got the wrong value: john");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "joe"_n, "Got the wrong value: joe");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "bob"_n, "Got the wrong value: bob");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "billy"_n, "Got the wrong value: billy");
      ++itr;
      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().primary_key == "alice"_n, "Got the wrong beginning");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");

      // operator--
      // ----------
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 1");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 2");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 3");
      --itr;
      eosio::check(itr != begin_itr, "Should not be the beginning: 4");
      --itr;
      eosio::check(itr == begin_itr, "Should be the beginning");

      eosio::check(end_itr > begin_itr, "end should be greater than beginning");
      eosio::check(end_itr >= begin_itr, "end should be greater than or equal to beginning");
      eosio::check(begin_itr < end_itr, "beginning should be less than end");
      eosio::check(begin_itr <= end_itr, "beginning should be less than or equal to end");

      auto b = t.primary_key.rbegin();
      auto c = t.primary_key.rbegin();
      ++c;
      eosio::check(c > b, "c should be greater than b");
      eosio::check(c >= b, "c should be greater than or equal to b");
      eosio::check(b <= c, "b should be less than or equal to c");
      eosio::check(b < c, "b should be less than c");
   }

   [[eosio::action]]
   void ritrerror1() {
      my_table t{"kvtest"_n};
      auto end_itr = t.primary_key.rend();
      ++end_itr;
   }

   [[eosio::action]]
   void ritrerror2() {
      my_table t{"kvtest"_n};
      auto begin_itr = t.primary_key.rbegin();
      --begin_itr;
   }

   [[eosio::action]]
   void range() {
      my_table t{"kvtest"_n};

      std::vector<my_struct> expected{s, s4};
      auto vals = t.primary_key.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");

      expected = {};
      vals = t.primary_key.range("bob"_n, "bob"_n);
      eosio::check(vals == expected, "range did not return expected vector: {} - 1");
      vals = t.primary_key.range("chris"_n, "joe"_n);
      eosio::check(vals == expected, "range did not return expected vector: {} - 2");
      vals = t.primary_key.range("joe"_n, "alice"_n);
      eosio::check(vals == expected, "range did not return expected vector: {} - 3");

      expected = {s2, s5, s, s4, s3};
      vals = t.primary_key.range("alice"_n, "william"_n);
      eosio::check(vals == expected, "range did not return expected vector: {s2, s5, s, s4, s3}");
   }

   [[eosio::action]]
   void erase() {
      my_table t{"kvtest"_n};
      auto end_itr = t.primary_key.end();

      auto v = *(t.primary_key.get("joe"_n));
      t.erase(v);
      auto itr = t.primary_key.find("joe"_n);
      eosio::check(itr == end_itr, "key was not properly deleted");

      std::vector<my_struct> expected = {s};
      auto vals = t.primary_key.range("bob"_n, "john"_n);
      eosio::check(vals == expected, "range did not return expected vector");
   }
};
