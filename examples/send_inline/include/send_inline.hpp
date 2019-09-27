#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]] send_inline : public contract {
   public:
      using contract::contract;

      [[eosio::action]]
      void test( name user, name inline_code );

      using test_action = action_wrapper<"test"_n, &send_inline::test>;
};
