#include <bluegrass/meta/function_traits.hpp>

#include <string>
#include <catch2/catch.hpp>

using namespace bluegrass;
using namespace bluegrass::meta;

void test_func() {}

struct test_struct {
   void test_method() {}
   void test_methodc()const {}
   void test_methodr()& {}
   void test_methodrr()&& {}
   void test_methodcr()const & {}
   void test_methodcrr()const && {}

   int test_method2();
   float test_method3();
   std::string test_method4();
};

TEST_CASE("Testing is_function", "[is_function_tests]") {
   REQUIRE( is_function(&test_func) );
   REQUIRE( !is_function(&test_struct::test_method) );
   REQUIRE( !is_function([](){}) );
   REQUIRE( !is_function([&](){}) );

   REQUIRE( is_function_v<&test_func> );
   REQUIRE( !is_function_v<&test_struct::test_method> );

   REQUIRE( !is_member_function(&test_func) );
   REQUIRE( !is_member_function([](){}) );
   REQUIRE( !is_member_function([&](){}) );
   REQUIRE( is_member_function(&test_struct::test_method) );
   REQUIRE( is_member_function(&test_struct::test_methodc) );
   REQUIRE( is_member_function(&test_struct::test_methodr) );
   REQUIRE( is_member_function(&test_struct::test_methodrr) );
   REQUIRE( is_member_function(&test_struct::test_methodcr) );
   REQUIRE( is_member_function(&test_struct::test_methodcrr) );

   REQUIRE( !is_member_function_v<&test_func> );
   REQUIRE( is_member_function_v<&test_struct::test_method> );
   REQUIRE( is_member_function_v<&test_struct::test_methodc> );
   REQUIRE( is_member_function_v<&test_struct::test_methodr> );
   REQUIRE( is_member_function_v<&test_struct::test_methodrr> );
   REQUIRE( is_member_function_v<&test_struct::test_methodcr> );
   REQUIRE( is_member_function_v<&test_struct::test_methodcrr> );
}

int test_func2();
float test_func3();
std::string test_func4();

struct test_struct2 {
   void test();
   void test2(int, int, float, std::string);
};

TEST_CASE("Testing return_type", "[return_type_tests]") {
   REQUIRE( std::is_same_v<int, return_type_t<&test_func2>> );
   REQUIRE( std::is_same_v<float, return_type_t<&test_func3>> );
   REQUIRE( std::is_same_v<std::string, return_type_t<&test_func4>> );

   REQUIRE( std::is_same_v<int, return_type_t<&test_struct::test_method2>> );
   REQUIRE( std::is_same_v<float, return_type_t<&test_struct::test_method3>> );
   REQUIRE( std::is_same_v<std::string, return_type_t<&test_struct::test_method4>> );

}

TEST_CASE("Testing class_from_member", "[class_from_member_tests]") {
   REQUIRE( std::is_same_v<test_struct, class_from_member_t<&test_struct::test_method>> );
   REQUIRE( std::is_same_v<test_struct, class_from_member_t<&test_struct::test_method2>> );
   REQUIRE( std::is_same_v<test_struct2, class_from_member_t<&test_struct2::test>> );
}

void test_func5(int, int, float, std::string);

TEST_CASE("Testing flattening", "[flatten_tests]") {
   using flatten1 = flatten_parameters_t<&test_func>;
   REQUIRE( std::tuple_size_v<flatten1> == 0 );
   REQUIRE( arity_v<&test_func> == 0 );

   using flatten2 = flatten_parameters_t<&test_func5>;
   REQUIRE( std::tuple_size_v<flatten2> == 4 );
   REQUIRE( arity_v<&test_func5> == 4 );
   REQUIRE( (std::is_same_v<int, std::tuple_element_t<0, flatten2>> &&
            std::is_same_v<int, std::tuple_element_t<1, flatten2>> &&
            std::is_same_v<float, std::tuple_element_t<2, flatten2>> &&
            std::is_same_v<std::string, std::tuple_element_t<3, flatten2>>)
         );
   REQUIRE( (std::is_same_v<int, parameter_at_t<0, &test_func5>> &&
            std::is_same_v<int, parameter_at_t<1, &test_func5>> &&
            std::is_same_v<float, parameter_at_t<2, &test_func5>> &&
            std::is_same_v<std::string, parameter_at_t<3, &test_func5>>)
         );

   using flatten3 = flatten_parameters_t<&test_struct2::test>;
   REQUIRE( std::tuple_size_v<flatten3> == 0 );
   REQUIRE( arity_v<&test_struct2::test> == 0 );

   using flatten4 = flatten_parameters_t<&test_struct2::test2>;
   REQUIRE( std::tuple_size_v<flatten4> == 4 );
   REQUIRE( arity_v<&test_struct2::test2> == 4 );
   REQUIRE( (std::is_same_v<int, std::tuple_element_t<0, flatten4>> &&
            std::is_same_v<int, std::tuple_element_t<1, flatten4>> &&
            std::is_same_v<float, std::tuple_element_t<2, flatten4>> &&
            std::is_same_v<std::string, std::tuple_element_t<3, flatten4>>)
         );
    REQUIRE( (std::is_same_v<int, parameter_at_t<0, &test_struct2::test2>> &&
            std::is_same_v<int, parameter_at_t<1, &test_struct2::test2>> &&
            std::is_same_v<float, parameter_at_t<2, &test_struct2::test2>> &&
            std::is_same_v<std::string, parameter_at_t<3, &test_struct2::test2>>)
         );
}
