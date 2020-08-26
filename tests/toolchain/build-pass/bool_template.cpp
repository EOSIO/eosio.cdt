#include <eosio/eosio.hpp>

using namespace eosio;

namespace test {
   using _Bool = int32_t;
}

using My_Bool = float;

class [[eosio::contract]] bool_template : public eosio::contract {
   public:
      using contract::contract;

      [[eosio::action]] void test1(std::optional<bool> a) {}
      [[eosio::action]] void test2(std::variant<uint64_t, bool> a) {}
      [[eosio::action]] void test3(bool a) {}

      [[eosio::action]] void test4(test::_Bool a) {}
      [[eosio::action]] void test5(My_Bool a) {}
};
