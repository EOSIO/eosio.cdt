/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/eosio.hpp>
#include <eosio/tester.hpp>
#include <eosio/vector.hpp>

using eosio::vector;

// Definitions in `eosio.cdt/libraries/eosio/vector.hpp`
EOSIO_TEST_BEGIN(vector_type_test)

   // Constructors
   {
      // default
      vector<int> v;
      CHECK_EQUAL( v.size(),     1 )
      CHECK_EQUAL( v.capacity(), 1 )

      // size
      vector<int> v2(5);
      CHECK_EQUAL( v2.size(),     5 )
      CHECK_EQUAL( v2.capacity(), 5 )
      int foo[5] = { 0, 0, 0, 0, 0 };
      CHECK_EQUAL( memcmp(foo, v2.data(), 5), 0 )

      // size, value
      vector<int> v3(5, 5);
      CHECK_EQUAL( v3.size(), 5 )
      CHECK_EQUAL( v3.capacity(), 5 )
      int bar[5] = { 5, 5, 5, 5, 5 };
      CHECK_EQUAL( memcmp(bar, v3.data(), 5), 0 )

      // Copy
      vector<int> v4(v3);
      CHECK_EQUAL( v3.size(),     5 )
      CHECK_EQUAL( v3.capacity(), 5 )
      CHECK_EQUAL( memcmp(bar, v3.data(), 5), 0 )
      CHECK_EQUAL( v4.size(),     5 )
      CHECK_EQUAL( v4.capacity(), 5 )
      CHECK_EQUAL( memcmp(bar, v4.data(), 5), 0 )

      // Move
      vector<int> v5(std::move(v3));
      CHECK_EQUAL( v5.size(),     5 )
      CHECK_EQUAL( v5.capacity(), 5 )
      CHECK_EQUAL( memcmp(bar, v4.data(), 5), 0 )

      // initializer_list
      vector<int> v6{1, 2, 3};
      CHECK_EQUAL( v6.size(),     3 )
      CHECK_EQUAL( v6.capacity(), 3 )
      int baz[3] = { 1, 2, 3 };
      CHECK_EQUAL( memcmp(baz, v6.data(), 3), 0 )

      // assign
      vector<int> v7{1, 2, 3};
      CHECK_EQUAL( v7.size(),     3 )
      CHECK_EQUAL( v7.capacity(), 3 )
      CHECK_EQUAL( memcmp(baz, v7.data(), 3), 0 )
      v7.clear();
      v7.assign(3, 4);
      CHECK_EQUAL( v7.size(),     3 )
      CHECK_EQUAL( v7.capacity(), 3 )
      int fizz[3] = { 4, 4, 4 };
      CHECK_EQUAL( memcmp(fizz, v7.data(), 3), 0 )
   }


   // Element access
   {
      vector<int> v{1,2,3,4};
      int foo[4] = { 1, 2, 3, 4 };
      CHECK_ASSERT( "out_of_range", ([&]() { v.at(4);}) )
      CHECK_EQUAL(  v.at(0),   1 )
      CHECK_EQUAL(  v[1],      2 )
      CHECK_EQUAL(  v.front(), 1 )
      CHECK_EQUAL(  v.back(),  4 )
      CHECK_EQUAL(  memcmp(foo, v.data(), 4),  0 )
   }

   // Iterators
   {
      vector<int> v{1, 2, 3, 4};
      CHECK_EQUAL( *v.begin(),        1 )
      CHECK_EQUAL( *v.cbegin(),       1 )
      CHECK_EQUAL( *(v.end()-1),      4 )
      CHECK_EQUAL( *(v.cend()-1),     4 )
      CHECK_EQUAL( *v.rbegin(),       4 )
      CHECK_EQUAL( *v.crbegin(),      4 )
      CHECK_EQUAL( *(v.rend()+1),     1 )
      CHECK_EQUAL( *(v.crend()+1),    1 )
   }

   // Capacity
   {
      vector<int> v(10, 100);
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )

      v.reserve(100);
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  100 )

      v.shrink_to_fit();
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
   }

   // Modifiers
   {
      // Clear
      vector<int> v(10, 100);
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.clear();
      CHECK_EQUAL( v.empty(),     true )
      CHECK_EQUAL( v.size(),      0 )
      CHECK_EQUAL( v.capacity(),  10 )

      // Insert
      v.assign(10, 100);
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.insert(v.begin()+5, 200);
      CHECK_EQUAL( v.size(),      11 )
      CHECK_EQUAL( v.capacity(),  20 )
      CHECK_EQUAL( v.at(5),       200 )

      // Emplace
      v.clear();
      v.assign(10, 100);
      v.shrink_to_fit();
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.insert(v.begin()+5, 200);
      CHECK_EQUAL( v.size(),      11 )
      CHECK_EQUAL( v.capacity(),  20 )
      CHECK_EQUAL( v.at(5),       200 )

      // Erase
      v.clear();
      v.assign(10, 100);
      v.shrink_to_fit();
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.erase(v.begin()+5);
      CHECK_EQUAL( v.size(),      9 )
      CHECK_EQUAL( v.capacity(),  10 )
      int foo[9] = { 100, 100, 100, 100, 100, 100, 100, 100, 100 };
      CHECK_EQUAL( memcmp(foo, v.data(), 9), 0 )

      // Push Back
      v.clear();
      v.assign(10, 100);
      v.shrink_to_fit();
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.push_back(200);
      CHECK_EQUAL( v.size(),      11 )
      CHECK_EQUAL( v.capacity(),  20 )
      CHECK_EQUAL( v.at(10),      200 )

      // Emplace Back
      v.clear();
      v.assign(10, 100);
      v.shrink_to_fit();
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.push_back(200);
      CHECK_EQUAL( v.size(),      11 )
      CHECK_EQUAL( v.capacity(),  20 )
      CHECK_EQUAL( v.at(10),      200 )

      // Pop Back
      v.clear();
      v.assign(10, 100);
      v.shrink_to_fit();
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.pop_back();
      CHECK_EQUAL( v.size(),      9 )
      CHECK_EQUAL( v.capacity(),  10 )
      CHECK_EQUAL( memcmp(foo, v.data(), 9), 0 )

      // Resize
      v.clear();
      v.assign(10, 100);
      v.shrink_to_fit();
      CHECK_EQUAL( v.empty(),     false )
      CHECK_EQUAL( v.size(),      10 )
      CHECK_EQUAL( v.capacity(),  10 )
      v.resize(100);
      CHECK_EQUAL( v.size(),      100 )
      CHECK_EQUAL( v.capacity(),  100 )
      int bar[10] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
      CHECK_EQUAL( memcmp(foo, v.data(), 10), 0 )

      // Swap
      v.clear();
      v.assign(5, 100);
      v.shrink_to_fit();
      vector<int> v2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      v.swap(v2);
      int baz[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      CHECK_EQUAL( memcmp(baz, v.data(),  10), 0 )
      CHECK_EQUAL( memcmp(bar, v2.data(), 10), 0 )
      
   }

EOSIO_TEST_END

int main(int argc, char* argv[]) {
   /*bool verbose = false;
   if( argc >= 2 && std::strcmp( argv[1], "-v" ) == 0 ) {
      verbose = true;
   }
   silence_output(!verbose);*/
   silence_output(false);

   EOSIO_TEST(vector_type_test);
   return has_failed();
}
