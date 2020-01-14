#include <eosio/eosio.hpp>

struct testing_struct {
   uint16_t a;
   uint16_t b;

   bool operator==(testing_struct rhs){
      return a == rhs.a && b == rhs.b;
   }
};

struct my_struct {
   eosio::name tname;
   std::string tstring;
   uint64_t tui64;
   int32_t ti32;
   uint128_t ti128;
   float tfloat;
   testing_struct tstruct;
};

struct my_table : eosio::kv_table<my_struct, "testtable"_n> {
   struct {
      kv_index tname{eosio::name{"a"}, &my_struct::tname};
      kv_index tstring{eosio::name{"b"}, &my_struct::tstring};
      kv_index tui64{eosio::name{"c"}, &my_struct::tui64};
      kv_index ti32{eosio::name{"d"}, &my_struct::ti32};
      kv_index ti128{eosio::name{"e"}, &my_struct::ti128};
      kv_index tfloat{eosio::name{"f"}, &my_struct::tfloat};
      kv_index tstruct{eosio::name{"g"}, &my_struct::tstruct};
   } index;

   my_table() {
      init(eosio::name{"kvtest"}, &index);
   }
};

class [[eosio::contract]] kv_make_key_tests : public eosio::contract {
public:
   using contract::contract;
   my_struct s1{
      .tname = "bob"_n,
      .tstring = "a",
      .tui64 = 5,
      .ti32 = 0,
      .ti128 = (static_cast<uint128_t>(1) << 127) - 5,
      .tfloat = 4.2574,
      .tstruct = { 1, 2 }
   };
   my_struct s2{
      .tname = "alice"_n,
      .tstring = "C",
      .tui64 = 4,
      .ti32 = -1,
      .ti128 = (static_cast<uint128_t>(1) << 127) - 4,
      .tfloat = 5.2574,
      .tstruct = { 5, 6 }
   };
   my_struct s3{
      .tname = "john"_n,
      .tstring = "e",
      .tui64 = 3,
      .ti32 = -2,
      .ti128 = (static_cast<uint128_t>(1) << 127) - 3,
      .tfloat = 187234,
      .tstruct = { 3, 4 }
   };
   my_struct s4{
      .tname = "joe"_n,
      .tstring = "g",
      .tui64 = 2,
      .ti32 = 1,
      .ti128 = (static_cast<uint128_t>(1) << 127) - 2,
      .tfloat = 0,
      .tstruct = { 7, 8 }
   };
   my_struct s5{
      .tname = "billy"_n,
      .tstring = "I",
      .tui64 = 1,
      .ti32 = 2,
      .ti128 = (static_cast<uint128_t>(1) << 127) - 1,
      .tfloat = -4.2574,
      .tstruct = { 9, 10 }
   };

   [[eosio::action]]
   void setup() {
      my_table t;

      t.put(s1);
      t.put(s2);
      t.put(s3);
      t.put(s4);
      t.put(s5);
   }

   [[eosio::action]]
   void makekeyname() {
      my_table t;

      auto end_itr = t.index.tname.end();
      auto itr = t.index.tname.begin();

      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().tname == s2.tname, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tname == s5.tname, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tname == s1.tname, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tname == s4.tname, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tname == s3.tname, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void makekeystr() {
      my_table t;

      auto end_itr = t.index.tstring.end();
      auto itr = t.index.tstring.begin();

      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().tstring == s2.tstring, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstring == s5.tstring, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstring == s1.tstring, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstring == s3.tstring, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstring == s4.tstring, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void makekeyuill() {
      my_table t;

      auto end_itr = t.index.tui64.end();
      auto itr = t.index.tui64.begin();

      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().tui64 == s5.tui64, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tui64 == s4.tui64, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tui64 == s3.tui64, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tui64 == s2.tui64, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tui64 == s1.tui64, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void makekeyil() {
      my_table t;

      auto end_itr = t.index.ti32.end();
      auto itr = t.index.ti32.begin();

      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().ti32 == s3.ti32, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti32 == s2.ti32, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti32 == s1.ti32, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti32 == s4.ti32, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti32 == s5.ti32, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void makekeyilll() {
      my_table t;

      auto end_itr = t.index.ti128.end();
      auto itr = t.index.ti128.begin();

      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().ti128 == s1.ti128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti128 == s2.ti128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti128 == s3.ti128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti128 == s4.ti128, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().ti128 == s5.ti128, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void makekeyflt() {
      my_table t;

      auto end_itr = t.index.tfloat.end();
      auto itr = t.index.tfloat.begin();

      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().tfloat == s5.tfloat, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tfloat == s4.tfloat, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tfloat == s1.tfloat, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tfloat == s2.tfloat, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tfloat == s3.tfloat, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }

   [[eosio::action]]
   void makekeystct() {
      my_table t;

      auto end_itr = t.index.tstruct.end();
      auto itr = t.index.tstruct.begin();

      eosio::check(itr != end_itr, "Should not be the end");
      eosio::check(itr.value().tstruct == s1.tstruct, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct == s3.tstruct, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct == s2.tstruct, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct == s4.tstruct, "Got the wrong value");
      ++itr;
      eosio::check(itr.value().tstruct == s5.tstruct, "Got the wrong value");
      ++itr;
      eosio::check(itr == end_itr, "Should be the end");
   }
};
