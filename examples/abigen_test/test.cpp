#include <eosiolib/eosio.hpp>
using namespace eosio;

namespace test {
   struct [[ eosio::action ]] testa {
      void printddd() { print("testa"); }
      int fielda;
      float fieldb;
      account_name name;
   };
   struct testb {
      void printll() { print("testb"); }
   };
   struct testc : testb {
      uint64_t num;
   };
}

class test_contract : public eosio::contract {
  public:
      using contract::contract;

      [[ eosio::action ]]
      void testacta( account_name user ) {
         print( "Hello, ", name{user} );
      }

      [[ eosio::action ]]
      void testactb( test::testc input ) {
         print(input.num);
      }

      struct [[eosio::table]] testtable {
         uint64_t owner;
         uint64_t sec;
         uint64_t third;
         uint64_t primary_key() const { return owner; }
      };
};

typedef eosio::multi_index< N(testtab), test_contract::testtable > testtable_t;

// NOTE THIS IS A TEST CONTRACT AND WILL NOT WORK CORRECTLY, the action `testa` will not be in the dispatcher of this macro
EOSIO_ABI( test_contract, (testacta)(testactb) )
