/*
 * Regression test for https://github.com/EOSIO/eosio.cdt/issues/558
 *
 * Verifies that a class/function can be used from the std namespace
 */

#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <tuple>

using std::tuple;
using namespace eosio;

class[[eosio::contract("hello")]] hello : public contract
{
public:
   using contract::contract;

   [[eosio::action]] void hi(name user) {
      require_auth(user);
      print("Hello, ", user);
   }

   struct [[eosio::table]] greeting {
      uint64_t id;
      tuple<int, int> t;
      uint64_t primary_key() const { return id; }
   };
   typedef multi_index<"greeting"_n, greeting> greeting_index;
};
