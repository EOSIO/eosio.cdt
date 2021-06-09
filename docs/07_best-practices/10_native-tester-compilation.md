---
content_title: How to use native tester/compilation
---

As of v1.5.0 native compilation can be performed and a new set of libraries to facilitate native testing and native "scratch pad" compilation. [`eosio-cc`](../03_command-reference/eosio-cc.md), [`eosio-cpp`](../03_command-reference/eosio-cpp.md) and [`eosio-ld`](../03_command-reference/eosio-ld.md) now support building "smart contracts" and unit tests natively for quick tests to help facilitate faster development \(note the default implementations of eosio `intrinsics` are currently asserts that state they are unavailable, these are user definable.\)

## Getting Started
Once you have your smart contract written then a test source file can be written.

`hello.hpp`
```c++ 
#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] hello : public eosio::contract {
  public:
      using contract::contract;

      [[eosio::action]] void hi( name user );

      // accessor for external contracts to easily send inline actions to your contract
      using hi_action = action_wrapper<"hi"_n, &hello::hi>;
};
```

and then a quick test
`hello_test.cpp`
```c++
#include <eosio/eosio.hpp>
#include <eosio/tester.hpp>

#include <hello.hpp>

using namespace eosio;
using namespace eosio::native;

EOSIO_TEST_BEGIN(hello_test)
   // These can be redefined by the user to suit there needs per unit test
   // the idea is that in a future release there will be a base library that 
   // initializes these to "useable" default implementations and probably 
   // helpers to more easily define read_action_data and action_data_size intrinsics
   // like these"
   intrinsics::set_intrinsic<intrinsics::read_action_data>(
         [](void* m, uint32_t len) {
            check(len <= sizeof(eosio::name), "failed from read_action_data");
            *((eosio::name*)m) = "hello"_n;
            return len; 
         });

   intrinsics::set_intrinsic<intrinsics::action_data_size>(
         []() {
            return (uint32_t)sizeof(eosio::name);
         });
   
   intrinsics::set_intrinsic<intrinsics::require_auth>(
         [](capi_name nm) {
         });

   
   // "Name : hello" should be in the print buffer
   CHECK_PRINT("Name : hello",
         []() {
            apply("test"_n.value, "test"_n.value, "hi"_n.value);
            });
           
   // should not assert
   apply("test"_n.value, "test"_n.value, "check"_n.value);
   
   name nm = "null"_n;
   intrinsics::set_intrinsic<intrinsics::read_action_data>(
         [&](void* m, uint32_t len) {
            check(len <= sizeof(eosio::name), "failed from read_action_data");
            *((eosio::name*)m) = nm;
            return len; 
         });

   REQUIRE_ASSERT( "check name not equal to `hello`",
         []() {
            // should assert
            apply("test"_n.value, "test"_n.value, "check"_n.value);
            });

EOSIO_TEST_END

// boilerplate main, this will be generated in a future release
int main(int argc, char** argv) {
   silence_output(true);
   EOSIO_TEST(hello_test);
   return has_failed();
}
```

Every `intrinsic` that is defined for eosio (prints, require_auth, etc.) is re-definable given the `intrinsics::set_intrinsics<intrinsics::the_intrinsic_name>()` functions.  These take a lambda whose arguments and return type should match that of the intrinsic you are trying to define.  This gives the contract writer the flexibility to modify behavior to suit the unit test being written. A sister function `intrinsics::get_intrinsics<intrinsics::the_intrinsic_name>()` will return the function object that currently defines the behavior for said intrinsic.  This pattern can be used to mock functionality and allow for easier testing of smart contracts.  For more information see, either the [tests](https://github.com/EOSIO/eosio.cdt/blob/master/examples/hello/tests/) directory or [hello_test.cpp](https://github.com/EOSIO/eosio.cdt/blob/master/examples/hello/tests/hello_test.cpp) for working examples.

## Compiling Native Code
- Raw `eosio-cpp` to compile the test or program the only addition needed to the command line is to add the flag `-fnative` this will then generate native code instead of `wasm` code.
- Via CMake
    - `add_native_library` and `add_native_executable` CMake macros have been added (these are a drop in replacement for add_library and add_executable).

## Eosio.CDT Native Tester API
- CHECK_ASSERT(...) : This macro will check whether a particular assert has occured and flag the tests as failed but allow the rest of the tests to run.  
    - This is called either by 
        - `CHECK_ASSERT("<assert message>", [](<args>){ whatever_function(<args>); })`
        - `CHECK_ASSERT([](std::string msg){ user defined comparison function }, [](<args>){ whatever_function(<args>); })`
- CHECK_PRINT(...) : This macro will check whether the print buffer holds the string that is expected and flag the tests as failed but allow the rest of the test to run.
    - This is called either by 
        - `CHECK_PRINT("<print message>", [](<args>){ whatever_function(<args>); })`
        - `CHECK_PRINT([](std::string print_buffer){ user defined comparison function }, [](<args>){ whatever_function(<args>); })`
- CHECK_EQUAL(X, Y) : This macro will check whether two inputs equal eachother and fail the test but allow the rest of the test to continue.
- REQUIRE_ASSERT(...) : This macro will check whether a particular assert has occured and flag the tests as failed and halt the test on failure.  
    - This is called either by 
        - `REQUIRE_ASSERT("<assert message>", [](<args>){ whatever_function(<args>); })`
        - `REQUIRE_ASSERT([](std::string msg){ user defined comparison function }, [](<args>){ whatever_function(<args>); })`
- REQUIRE_PRINT(...) : This macro will check whether the print buffer holds the string that is expected and flag the tests as failed and halt the test on failure.
    - This is called either by 
        - `REQUIRE_PRINT("<print message>", [](<args>){ whatever_function(<args>); })`
        - `REQUIRE_PRINT([](std::string print_buffer){ user defined comparison function }, [](<args>){ whatever_function(<args>); })`
- REQUIRE_EQUAL(X, Y) : This macro will check whether two inputs `X` and `Y` equal eachother and fail the test and halt the test on failure.
- EOSIO_TEST_BEGIN(X) : This macro defines the beginning of a unit test and assigns `X` as the symbolic name of that test.
- EOSIO_TEST_END : This macro defines the end of a unit test.
- EOSIO_TEST(X) : This is used to run a particular named unit test `X` in the main function.
