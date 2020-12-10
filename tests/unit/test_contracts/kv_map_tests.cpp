#include <eosio/eosio.hpp>
#include <eosio/table.hpp>

struct test_record {
   int pk;
   float s;
   std::string n;
};

class [[eosio::contract]] kv_map_tests : public eosio::contract {
public:
   using contract::contract;

   using testmap_t = eosio::kv::map<"testmap"_n, int, float>;
   using testmap2_t = eosio::kv::map<"testmap2"_n, std::string, std::string>;
   using testmap3_t = eosio::kv::map<"testmap3"_n, int, float>;
   using testmap4_t = eosio::kv::map<"testmap4"_n, int, float>;

   [[eosio::action]]
   void test() {
      testmap_t t = { {33, 23.4f}, {10, 13.44f}, {103, 334.3f} };

      auto p = t[33];

      p = 102.23; // note here this will update the held value and do a db set

      testmap_t t2;

      eosio::check(p == 102.23f, "should be the same value");
      eosio::check(p == t2.at(33), "should be the same value");

      auto it = t.begin();

      auto& el = *it;

      eosio::check(el.second() == 13.44f, "should still be the same from before");

      testmap2_t t3 = { {"eosio", "fast"}, {"bit...", "hmm"} };

      auto it2 = t3.begin();
      auto& el2 = *it2;

      eosio::check(el2.value == std::string("hmm"), "should point to the lowest lexicographic key");

      ++it2;
      auto& el3 = *it2;

      eosio::check(el3.second() == std::string("fast"), "should now be pointing to the next");

      ++it2;

      auto it3 = std::move(it2);

      it2 = t3.end();

      eosio::check(it2 == it3, "they should be at the end and pointing to the same thing");
      eosio::check(!it2.is_valid(), "iterator should be at end");

      eosio::check(!it3.is_valid(), "iterator should be at end");
   }

   [[eosio::action]]
   void iter() {
      testmap_t t = { {34, 23.4f}, {11, 13.44f}, {104, 334.3f}, {5, 33.42f} };

      float test_vals[7] = {33.42f, 13.44f, 13.44f, 102.23f, 23.4f, 334.3f, 334.3f};

      int i = 0;

      // test that this will work with auto ranged for loop
      for ( const auto& e : t ) {
         eosio::check(e.second() == test_vals[i++], "invalid value in iter test");
      }
   }

   [[eosio::action]]
   void erase() {
      testmap_t t;

      t.contains(34);
      t.erase(34);

      eosio::check(!t.contains(34), "should have erased a value");
   }

   [[eosio::action]]
   void eraseexcp() {
      testmap_t t;
      t.at(34); // this should cause an assertion
   }

   [[eosio::action]]
   void bounds() {
      testmap3_t t = {{33, 10}, {10, 41.2f}, {11, 100.100f}, {2, 17.42f}};

      auto it = t.lower_bound(11);

      eosio::check(it->first() == testmap3_t::full_key((int)11), "should be pointing to 11");

      it = t.lower_bound(31);

      eosio::check(it->first() == testmap3_t::full_key((int)33), "should be pointing to 33");

      it = t.lower_bound(36);

      eosio::check(!it.is_valid(), "should be pointing to end");

      auto it2 = t.lower_bound(1);

      eosio::check(it2->first() == testmap3_t::full_key((int)2), "should be pointing to 2");

      it = t.upper_bound(10);

      eosio::check(it->first() == testmap3_t::full_key((int)11), "should be pointing to 11");

      it = t.upper_bound(33);

      eosio::check(!it.is_valid(), "should be pointing to end");
   }

   [[eosio::action]]
   void ranges() {
      testmap3_t t = {{17, 9.9f}};

      auto range = t.equal_range(16);

      eosio::check(range.first->first() == testmap3_t::full_key((int)17), "should be pointing to 17");
      eosio::check(range.second->first() == testmap3_t::full_key((int)17), "should be pointing to 17");

      range = t.equal_range(1);

      eosio::check(range.first->first() == testmap3_t::full_key((int)2), "should be pointing to 2");
      eosio::check(range.second->first() == testmap3_t::full_key((int)2), "should be pointing to 2");

      auto vec = t.ranged_slice(10, 30);

      eosio::check(vec.size() == 3, "should contain 3 elements");
      eosio::check((float)vec[0].second() == 41.2f, "should contain 41.2");
      eosio::check((float)vec[1].second() == 100.100f, "should contain 100.100");
      eosio::check((float)vec[2].second() == 9.9f, "should contain 9.9");
   }

   [[eosio::action]]
   void empty() {
      testmap3_t t = {{33, 10}, {10, 41.2f}, {11, 100.100f}, {2, 17.42f}};

      eosio::check(!t.empty(), "t shouldn't be empty");

      testmap4_t t2 = {{10, 10}, {13, 13}};

      eosio::check(!t2.empty(), "t2 shouln't be empty");

      t2.erase(10);
      t2.erase(13);

      eosio::check(t2.empty(), "t2 should now be empty");
   }

   [[eosio::action]]
   void gettmpbuf() {
      testmap3_t t = {{33, 10}, {10, 41.2f}, {11, 100.100f}, {2, 17.42f}};

      auto buff = t.get_tmp_buffer(34); // ensure this interface doesn't regress

      auto iter = t.lower_bound(11);

      eosio::check(iter->value == 100.100f, "should be equal and not fail to compile");
   }

   [[eosio::action]]
   void constrct() {
      using map_t = eosio::kv::map<"map"_n, float, eosio::time_point>;
      map_t m = {{13.3f, eosio::time_point{}}};

      m[13.3f] = eosio::time_point{eosio::microseconds{(int64_t)13}};

      auto iter = m.find(13.3f);
      auto val = iter->second();

      auto expected = eosio::time_point{eosio::microseconds{(int64_t)13}};
      eosio::check(val == expected, "should be equal and not fail to compile");

      auto iter2 = m.find(10);
      eosio::check(iter2 == m.end(), "shouldn't be found");

      const auto citer = m.find(13.3f);
      eosio::check(citer->second() == val, "should still be the same with const and shouldn't fail to compile");
   }

   struct __attribute__((packed)) key_struct_fragments {
      uint8_t  magic;
      uint64_t table;
      uint64_t index;
   };

   [[eosio::action]]
   void keys() {
      using map_t = eosio::kv::map<"map"_n, float, eosio::time_point>;
      map_t m = {{13.3f, eosio::time_point{}}};

      auto iter = m.find(13.3f);
      auto k = iter->first();

      key_struct_fragments kfs;
      memcpy(&kfs, iter->first().data(), sizeof(kfs));

      kfs.table = __builtin_bswap64(kfs.table);
      kfs.index = __builtin_bswap64(kfs.index);

      eosio::check(kfs.magic == 1, "should still be hardcoded to 1 for now");
      eosio::check(kfs.table == eosio::name("map").value, "table should be named 'map'");
      eosio::check(kfs.index == eosio::name("map.index").value, "index should be named 'map.index'");
   }
};
