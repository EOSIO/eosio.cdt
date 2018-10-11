#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT hello : public eosio::contract {
  public:
      using contract::contract;

      ACTION hi( name user ) {
         print( "Hello, ", name{user} );
      }
};

EOSIO_DISPATCH( hello, (hi) )
