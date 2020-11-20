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

   using hello_t = eosio::kv::map<"hello"_n, int, float>;
   using hello2_t = eosio::kv::map<"hello2"_n, std::string, std::string>;

   [[eosio::action]]
   void test() {
      hello_t t = { {33, 23.4f}, {10, 13.44f}, {103, 334.3f} };

      auto p = t[33];

      p = 102.23; // note here this will update the held value and do a db set

      hello_t t2;

      eosio::print_f("Before");
      eosio::check(p == 102.23f, "should be the same value");
      eosio::check(p == t2.at(33), "should be the same value");
      eosio::print_f("after");

      auto it = t.begin();

      auto& el = *it;

      eosio::check(el.second() == 13.44f, "should still be the same from before");

      hello2_t t3 = { {"eosio", "fast"}, {"bit...", "hmm"} };

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
      hello_t t = { {34, 23.4f}, {11, 13.44f}, {104, 334.3f}, {5, 33.42f} };

      float test_vals[7] = {33.42f, 13.44f, 13.44f, 102.23f, 23.4f, 334.3f, 334.3f};

      int i = 0;

      // test that this will work with auto ranged for loop
      for ( const auto& e : t ) {
         eosio::check(e.second() == test_vals[i++], "invalid value in iter test");
      }
   }

   [[eosio::action]]
   void erase() {
      hello_t t;

      t.contains(34);
      t.erase(34);

      eosio::check(!t.contains(34), "should have erased a value");

      auto r = t.equal_range(100);
      for (auto it = std::move(r.first); it != r.second; ++it)
         eosio::print_f("I %", it->second());

      using namespace eosio;

      using mi = kv::table<"mi"_n, kv::index<"foo"_n, &test_record::pk>,
                               kv::index<"bar"_n, &test_record::s>,
                               kv::index<"baz"_n, &test_record::n>>;

      mi m;
      test_record rec = {34, 42.42f, "hello"};
      test_record rec2 = {10, 33.33f, "good bye"};

      m.put(rec);

      m.put(rec2);

      eosio::print_f("Before\n");
      auto v = m.get(34);
      eosio::print_f("After\n");
   }
};
