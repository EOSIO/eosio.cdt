#include <eosiolib/eosio.hpp>
#include <optional>

using namespace eosio;

typedef int type_def;

namespace test {
   // mark this struct as an action
   struct [[ eosio::action ]] testa {
      void printddd() { print("testa"); }
      int fielda;
      float fieldb;
      capi_name name;
   };

   struct testb {
      float field;
      void printll() { print("testb"); }
   };
   
   // mark this struct as an action and specify the name explicitly
   struct [[ using eosio: action("testc"), contract("abitest") ]] test_c : testb {
      uint64_t num;
   };
}

CONTRACT abitest : public eosio::contract {
  public:
      using contract::contract;
      
      // mark this method as an action and specify the name explicity
      [[ eosio::action("testacta") ]]
      void testact_a( name user, const std::string& s, std::vector<int>& c, std::vector<std::string> sv ) {
         print( "Hello, ", name{user} );
         symbol sym("TEST", 4);
      }
      
      // mark this method as an action
      ACTION testactb( test::test_c input, type_def td, std::optional<int> cc, bool d ) {
         print(input.num);
      }
      
      // mark this struct as a table and allow multi_index typedefs to define the tables
      TABLE testtable {
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

typedef eosio::multi_index< "testtab"_n,  abitest::testtable > testtable_t;
typedef eosio::multi_index< "testtaba"_n, abitest::testtable > testtable_a_t;
typedef eosio::multi_index< "testtab2"_n, abitest::test_table2 > testtable2_t;

extern "C" 
void apply(uint64_t, uint64_t, uint64_t){
}
