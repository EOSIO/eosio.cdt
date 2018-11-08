#include <eosiolib/eosio.hpp>
#include <hello.hpp>

using namespace eosio;

CONTRACT send_inline : public eosio::contract {
  public:
      using contract::contract;

      ACTION test( name user, name inline_code ) {
         print_f( "Hello % from send_inline", user );
         // constructor takes two arguments (the code the contract is deployed on and the set of permissions)
         hello::hi_action hi(inline_code, {_self, "active"_n});
         hi.send(user);
      }

      // accessor for external contracts to easily send inline actions to your contract
      using test_action = action_wrapper<"test"_n, &send_inline::test>;
};

EOSIO_DISPATCH( send_inline, (test) )
