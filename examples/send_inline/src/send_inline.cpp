#include <send_inline.hpp>
#include <hello.hpp>
ACTION send_inline::test( name user, name inline_code ) {
   print_f( "Hello % from send_inline", user );
   // constructor takes two arguments (the code the contract is deployed on and the set of permissions)
   hello::hi_action hi(inline_code, {_self, "active"_n});
   hi.send(user);
}

EOSIO_DISPATCH( send_inline, (test) )
