#include <eosiolib/eosio.hpp>
#include <eosiolib/string.hpp>

using namespace eosio;

CONTRACT rope_tests : public contract{
   public:
      using contract::contract;

      ACTION test1() {
         rope r("hello");
         char buff[256];
         //r.c_str(buff);
         //eosio::print(buff);
         r += ", world";
         r += " please!";
         r.c_str(buff);
         //eosio::print(buff);
         //eosio_assert(false, "something");
      }
};

EOSIO_DISPATCH(rope_tests, (test1))
