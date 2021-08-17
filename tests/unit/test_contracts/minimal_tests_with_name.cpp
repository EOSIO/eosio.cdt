// Verifies that the dispatching code is self-contained
#include <eosio/eosio.hpp>

class [[eosio::contract]] minimal_tests_with_name {
 public:
   template<typename N, typename DS>
   explicit constexpr minimal_tests_with_name(const N&, const N&, const DS&) {}
   [[eosio::action]] void test1(eosio::name memo) {}
};
