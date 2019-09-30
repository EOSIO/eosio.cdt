/*
 * Regression test for https://github.com/EOSIO/eosio.cdt/issues/600
 *
 * Verifies that nested typedefs build.
 */

#include <eosio/eosio.hpp>
using namespace eosio;

namespace foo {
//using str = std::string;
typedef std::string str;
}

class [[eosio::contract]] using_nested_typedef : public contract {
public:
   using contract::contract;
   [[eosio::action]]
   void hi(foo::str s) {
      print(s);
   }
};
