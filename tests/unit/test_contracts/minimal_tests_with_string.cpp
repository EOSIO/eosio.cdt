// Verifies that the dispatching code is self-contained
#include <string>

class [[eosio::contract]] minimal_tests_with_string {
 public:
   template<typename N, typename DS>
   explicit constexpr minimal_tests_with_string(const N&, const N&, const DS&) {}
   [[eosio::action]] void test1(std::string memo) {}
};
