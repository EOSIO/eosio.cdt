#include <eosiolib/eosio.hpp>
using namespace eosio;

namespace test {
   // mark this struct as an action
   struct [[ eosio::action ]] testa {
      void printddd() { print("testa"); }
      int fielda;
      float fieldb;
      account_name name;
   };

   struct testb {
      void printll() { print("testb"); }
   };
   
   // mark this struct as an action and specify the name explicitly
   struct [[ eosio::action("testc") ]] test_c : testb {
      uint64_t num;
   };
}

class test_contract : public eosio::contract {
  public:
      using contract::contract;
      
      // mark this method as an action and specify the name explicity
      [[ eosio::action("testacta") ]]
      void testact_a( account_name user ) {
         print( "Hello, ", name{user} );
      }
      
      // mark this method as an action
      [[ eosio::action ]]
      void testactb( test::test_c input ) {
         print(input.num);
      }
      
      // mark this struct as a table and allow multi_index typedefs to define the tables
      struct [[eosio::table]] testtable {
         uint64_t owner;
         uint64_t third;
         uint64_t primary_key() const { return owner; }
      };

      // mark this struct as a table and allow multi_index typedefs to define the tables, and specify a primitive table (non multi_index) with an explicit name
      struct [[eosio::table("testtabb")]] test_table2 {
         uint64_t owner;
         uint64_t sec;
         uint64_t third;
         uint64_t primary_key() const { return owner; }
      };
};

typedef eosio::multi_index< N(testtab), test_contract::testtable > testtable_t;
typedef eosio::multi_index< N(testtaba), test_contract::testtable > testtable_a_t;
typedef eosio::multi_index< N(testtab2), test_contract::test_table2 > testtable2_t;

// NOTE THIS IS A TEST CONTRACT AND WILL NOT WORK CORRECTLY, the action `testa` will not be in the dispatcher of this macro
EOSIO_ABI( test_contract, (testact_a)(testactb) )
