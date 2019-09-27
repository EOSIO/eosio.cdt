#include <multi_index_example.hpp>
[[eosio::action]] 
void multi_index_example::set( name user ) {
   auto itr = testtab.find(user.value);
   if ( itr == testtab.end() ) {
      testtab.emplace( _self, [&]( auto& u ) {
            u.test_primary = user;
            u.secondary = "second"_n;
            u.datum = 0;
      });
   }
}

[[eosio::action]] 
void multi_index_example::print( name user ) {
   auto itr = testtab.find(user.value);
   check( itr != testtab.end(), "user does not exist in table" );
   eosio::print_f("Test Table : {%, %, %}\n", itr->test_primary, itr->secondary, itr->datum);
}

[[eosio::action]] 
void multi_index_example::bysec( name secid ) {
   auto idx = testtab.get_index<"secid"_n>();
   for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
      print( itr->test_primary );
   }
}

[[eosio::action]] 
void multi_index_example::mod( name user, uint32_t n ) {
   auto itr = testtab.find(user.value);
   check( itr != testtab.end(), "user does not exist in table" );
   testtab.modify( itr, _self, [&]( auto& row ) {
      row.secondary = user;
      row.datum = n;
   });
}

[[eosio::action]]
void multi_index_example::del( name user ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);
  if ( itr == testtab.end() ) {
    printf("user does not exist in table, nothing to delete" );
    return;
  }

  // if we got so far it means user exists so we can delete it using 
  // the iterator found based on its primary key
  testtab.erase( itr );
}
