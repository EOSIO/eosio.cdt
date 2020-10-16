#include <multi_index_large.hpp>

[[eosio::action]] 
void multi_index_large::set( uint64_t id, uint64_t u64, uint128_t u128,
   double f64, long double f128, eosio::checksum256 chk256 ) {
   auto itr = testtab.find(id);
   if ( itr == testtab.end() ) {
      testtab.emplace( _self, [&]( auto& u ) {
         u.id = id;
         u.u64 = u64;
         u.u128 = u128;
         u.f128 = f128;
         u.chk256 = chk256;
      });
   }
}

[[eosio::action]] 
void multi_index_large::print( uint64_t id ) {
   auto itr = testtab.find(id);
   check( itr != testtab.end(), "id does not exist in table" );
   eosio::print_f("Test Table : {%, %, %, %}\n", itr->id, itr->f128, itr->u128, itr->chk256);
}

[[eosio::action]] 
void multi_index_large::byf( double f64 ) {
   auto idx = testtab.get_index<"byf"_n>();
   for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
      if( itr->f64 == f64) {
         print( itr->id );
      }
   }
}

[[eosio::action]] 
void multi_index_large::byff( long double f128 ) {
   auto idx = testtab.get_index<"byff"_n>();
   for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
      if( itr->f128 == f128) {
         print( itr->id );
      }
   }
}

[[eosio::action]] 
void multi_index_large::byuuuu( uint128_t u128 ) {
   auto idx = testtab.get_index<"byuuuu"_n>();
   for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
      if( itr->u128 == u128 ) {  
         print( itr->id );
      }
   }
}

[[eosio::action]] 
void multi_index_large::bychkb( eosio::checksum256 chk256 ) {
   auto idx = testtab.get_index<"bychkb"_n>();
   for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
      if( itr->chk256 == chk256 ) {  
         print( itr->id );
      }
   }
}

[[eosio::action]] 
void multi_index_large::mod( uint64_t id, uint64_t u64, uint128_t u128,
   double f64, long double f128, eosio::checksum256 chk256 ) {
   auto itr = testtab.find(id);
   check( itr != testtab.end(), "id does not exist in table" );
   testtab.modify( itr, _self, [&]( auto& row ) {
      row.u64 = u64;
      row.u128 = u128;
      row.f128 = f128;
      row.chk256 = chk256;
   });
}

[[eosio::action]]
void multi_index_large::del( uint64_t id ) {
   // check if the user already exists
   auto itr = testtab.find(id);
   if ( itr == testtab.end() ) {
      printf("user does not exist in table, nothing to delete" );
      return;
   }
   testtab.erase( itr );
}




