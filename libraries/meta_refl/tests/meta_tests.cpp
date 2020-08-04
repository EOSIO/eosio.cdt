#include <bluegrass/meta/refl.hpp>

#include <iostream>
#include <string>

#include <catch2/catch.hpp>

using namespace bluegrass;
using namespace bluegrass::meta;

struct test_struct {
   int a;
   float b;
   std::string c;
   BLUEGRASS_META_REFL(a, b, c);
};

void update(int& i) { i += 20; }
void update(float& f) { f += 20; }
void update(std::string& s) { s += " Hello"; }

// homogeneous struct
struct hom_struct {
   int a = 3;
   int b = 4;
   int c = 5;
};

// templated homogeneous struct
template <typename T, std::size_t N>
struct tn_hom_struct {
   T a;
   T b;
   T c;
};

BLUEGRASS_HOM_META(hom_struct, int)

TEST_CASE("Testing basic meta object", "[basic_meta_tests]") {
   using ts_meta = meta_object<test_struct>;

   // !!! the require macro throws off the evaluation of
   // this and it will produce the wrong result because
   // of preprocessor voodoo
   constexpr auto name = ts_meta::name;
   REQUIRE( name == "test_struct" );
   REQUIRE( ts_meta::field_count == 3 );
   constexpr auto names = ts_meta::field_names;
   REQUIRE( names.size() == 3 );
   REQUIRE( names[0] == "a" );
   REQUIRE( names[1] == "b" );
   REQUIRE( names[2] == "c" );

   test_struct ts = {42, 13.13, "Hello"};

   REQUIRE( std::is_same_v<ts_meta::field_type<0>, int> );
   REQUIRE( ts_meta::get_field<0>(ts) == 42 );
   ts_meta::get_field<0>(ts) += 3;
   REQUIRE( ts_meta::get_field<0>(ts) == 45 );

   REQUIRE( std::is_same_v<ts_meta::field_type<1>, float> );
   REQUIRE( ts_meta::get_field<1>(ts) == 13.13f );
   ts_meta::get_field<1>(ts) += 3.3;
   REQUIRE( ts_meta::get_field<1>(ts) == 16.43f );

   REQUIRE( std::is_same_v<ts_meta::field_type<2>, std::string> );
   REQUIRE( ts_meta::get_field<2>(ts) == "Hello" );
   ts_meta::get_field<2>(ts)[0] = 'f';
   REQUIRE( ts_meta::get_field<2>(ts) == "fello" );

   const auto& test_lam = [&](auto& f) { update(f); };

   ts_meta::for_each_field(ts, test_lam);

   REQUIRE( ts.a == 65 );
   REQUIRE( ts.b == 36.43f );
   REQUIRE( ts.c == "fello Hello" );

   using hs_meta = meta_object<hom_struct>;

   hom_struct hs;
   constexpr auto name2 = hs_meta::name;
   REQUIRE( name2 == "hom_struct" );
   REQUIRE( hs_meta::field_count == 3 );
   REQUIRE( hs_meta::field_names.size() == 0 );
   REQUIRE( std::is_same_v<hs_meta::field_type<0>, int> );
   REQUIRE( std::is_same_v<hs_meta::field_type<1>, int> );
   REQUIRE( std::is_same_v<hs_meta::field_type<2>, int> );
   REQUIRE( hs_meta::get_field<0>(hs) == 3 );
   REQUIRE( hs_meta::get_field<1>(hs) == 4 );
   REQUIRE( hs_meta::get_field<2>(hs) == 5 );

   hs_meta::get_field<0>(hs) = 13;
   hs_meta::get_field<1>(hs) = 14;
   hs_meta::get_field<2>(hs) = 15;

   REQUIRE( hs_meta::get_field<0>(hs) == 13 );
   REQUIRE( hs_meta::get_field<1>(hs) == 14 );
   REQUIRE( hs_meta::get_field<2>(hs) == 15 );

   int r = 30;
   const auto& test_lam2 = [&](auto i) { r += i; };

   hs_meta::for_each_field(hs, test_lam2);

   REQUIRE( r == 30 + 13 + 14 + 15 );
}
