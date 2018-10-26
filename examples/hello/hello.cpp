#include <eosiolib/eosio.hpp>
#include <vector>

using namespace eosio;

CONTRACT hello : public eosio::contract {
  public:
      using contract::contract;
      
      ACTION hi( const name& user ) {
         print( "Hello, ", user );
      }
};

EOSIO_DISPATCH( hello, (hi) )
