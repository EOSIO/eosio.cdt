#include <multi_index_example.hpp>
ACTION multi_index_example::set( name user ) {
   auto itr = testtab.find(user.value);
   if ( itr == testtab.end() ) {
      testtab.emplace( _self, [&]( auto& u ) {
            u.test_primary = user;
            u.secondary = "second"_n;
            u.datum = 0;
      });
   }
}

ACTION multi_index_example::print( name user ) {
   auto itr = testtab.find(user.value);
   check( itr != testtab.end(), "test table not set" );
   eosio::print_f("Test Table : {%, %, %}\n", itr->test_primary, itr->secondary, itr->datum);
}

ACTION multi_index_example::bysec( name secid ) {
   auto idx = testtab.get_index<"secid"_n>();
   for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
      print( itr->test_primary );
   }
}


ACTION multi_index_example::mod( name user, uint32_t n ) {
   auto itr = testtab.find(user.value);
   check( itr != testtab.end(), "test table not set" );
   testtab.modify( itr, _self, [&]( auto& row ) {
      row.secondary = user;
      row.datum = n;
   });
}
