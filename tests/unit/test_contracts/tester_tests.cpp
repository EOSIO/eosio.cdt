#include "tester_tests.hpp"

using namespace eosio;

[[eosio::action]] void tester_tests::putdb(int key, int value) {
   table t(get_self(), 0);
   auto inserter = [=](table_item& item) {
      item.key = key;
      item.value = value;
   };
   if(auto iter = t.find(key); iter == t.end()) {
      t.emplace(get_self(), inserter);
   } else {
      t.modify(iter, get_self(), inserter);
   }
}
