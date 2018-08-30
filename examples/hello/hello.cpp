#include <eosiolib/eosio.hpp>
using namespace eosio;
class hello : public eosio::contract {
  public:
      using contract::contract;

      [[eosio::action]]
      void hi( account_name user ) {
         print( "Hello, ", name{user} );
      }
};

EOSIO_ABI( hello, (hi) )

#include <eosiolib/eosio.hpp>
using namespace eosio;

namespace test {
   struct [[ eosio::action ]] ae {
      void printddd() { print("osss"); }
      int cc;
      float ee;
      account_name name;
   };
   struct dc {
      void printll() { print("eoo"); }
   };
   struct bb : dc {
      uint64_t c;
   };
}

class hello : public eosio::contract {
  public:
      using contract::contract;

      [[ eosio::action ]]
      void hi( account_name user ) {
         print( "Hello, ", name{user} );
      }

      [[ eosio::action ]]
      void hey( aaa::bb input ) {
         print(input.c);
      }

      struct [[eosio::table]] test {
         uint64_t owner;
         uint64_t sec;
         uint64_t third;
         uint64_t primary_key() const { return owner; }
         uint64_t by_sec() const { return sec; }
         uint64_t by_another() const { return third; }
      };
};

typedef eosio::multi_index< N(hoosh), hello::test, indexed_by<N(sec), const_mem_fun<hello::test, uint64_t, &hello::test::by_sec>>>   ttable;
//using ttable = eosio::multi_index< N(hoosh), hello::test>;

EOSIO_ABI( hello, (hi) )
