---
content_title: WASM Unit Test Framework Tutorial
link_text: WASM Unit Test Framework Tutorial
---

## Overview

If you are a smart contract developer and want to build unit tests for your smart contracts then WASM Unit Test Framework is a viable option. It allows you to write unit tests, which can be grouped into fixtures, for each smart contract actions or even more complex scenarios, e.g. multiple action execution scenarios. By following this tutorial you will learn in detail how to use the WASM Unit Test Framework to build unit tests for your smart contracts.

## Prerequisites

For exemplification we will test the [`hello` smart contract](https://github.com/EOSIO/eosio.cdt/tree/master/examples/hello). Please make sure that the smart contract sources are copied to your local drive in the folder `EOSIO_CDT_EXAMPLE_HELLO_FOLDER` and can compile without error using `eosio.cdt`.
You can find instructions on how to build and install `eosio.cdt` in the [Installation section](../02_installation.md).

### Create the Smart Contract and the Accompanying Test File

Navigate to the `EOSIO_CDT_EXAMPLE_HELLO_FOLDER` on your local drive and create next to the already existing `tests` folder a second one `wasm_unit_tests`, then switch your current location to the newly created folder and then create two empty files `CMakeLists.txt` and `wasm_unit_tests.cpp`.

```sh
cd EOSIO_CDT_EXAMPLE_HELLO_FOLDER
mkdir wasm_unit_tests
cd wasm_unit_tests
touch CMakeLists.txt
touch wasm_unit_test_first.cpp
```

Replace the content of the `CMakeLists.txt` with the following using your favorite source editor:

```txt
project(wasm_unit_tests)
find_package(eosio.cdt)
add_native_executable(wasm_unit_test_first wasm_unit_test_first.cpp ${CMAKE_SOURCE_DIR}/../src/hello.cpp)
target_include_directories(wasm_unit_test_first PUBLIC ${CMAKE_SOURCE_DIR}/../include)
```

### Reference the WASM Unit Test Framework and Smart Contract to Test

In order to be able to make use of the WASM Unit Test Framework constructs we need to reference the `tester.hpp` header file. Also to be able to test the `hello` smart contract by accessing its actions we need to reference its header source file.
Therefore add the following lines to the top of the unit test source file:

```c++
#include <eosio/tester.hpp>
#include <string_view>
#include <hello.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
```

### Create the Test Fixture Class

The `Catch2` test framework allows one to group tests together by means of sessions within a test case however tutorial will demonstrate the more traditional way of grouping tests using a test fixture. To accomplish that create test fixture class `hello_tests_fixture` and then pass the class name to the `TEST_CASE_METHOD` macro provided by the `Catch2` framework.

```c++
class hello_tests_fixture {
  private:
    int unique_test_id;
  public:
   hello_tests_fixture() {
       this->unique_test_id = 0;
   }
  protected:
   int get_test_id() {
     return ++ this->unique_test_id;
   }
 };
```

### Create First Test

Now we are ready to create the first unit test. To make testing easier the WASM Unit Test Framework provides an utility class `test_chain` which is simulating a local blockchain for your tests to run against. Only one `test_chain` instance can exist at a time. The test chain uses simulated time starting at 2020-01-01T00:00:00.000. We will use this class in conjunction with the `hello_tests_fixture` class we created earlier by instantiating an instance of it in the constructor of the test fixture.

```c++
class hello_tests_fixture {
  private:
    int unique_test_id;
  public:
   eosio::test_chain test_chain;
   hello_tests_fixture() {
       this->unique_test_id = 0;
   }
  protected:
   int get_test_id() {
     return ++ this->unique_test_id;
   }
 };

TEST_CASE_METHOD(hello_tests_fixture, "Action hi::send", "[send]") {
   create_account("test"_n);
   create_account("wasmunittest"_n);
   set_code("test"_n, "../hello/hello.wasm");
   hello::hi_action("test"_n, { "test"_n, "active"_n }).send("wasmunittest"_n);
}
```

### Create Second Test

Let's create a second test and place it in the same fixture. We could chose to place it in a second fixture, it all depends how one wants to organize the unit tests.

```cpp
class hello_tests_fixture {
  private:
    int unique_test_id;
  public:
   eosio::test_chain test_chain;
   hello_tests_fixture() {
       this->unique_test_id = 0;
   }
  protected:
   int get_test_id() {
     return ++ this->unique_test_id;
   }
 };

TEST_CASE_METHOD(hello_tests_fixture, "Action hi::send", "[send]") {
   create_account("test"_n);
   create_account("wasmunittest"_n);
   set_code("test"_n, "../hello/hello.wasm");
   hello::hi_action("test"_n, { "test"_n, "active"_n }).send("wasmunittest"_n);
}

TEST_CASE_METHOD(hello_tests_fixture, "Action check::send", "[send]") {
   create_account("test"_n);
   create_account("wasmunittest"_n);
   set_code("test"_n, "../hello/hello.wasm");
   hello::check_action("test"_n, { "test"_n, "active"_n }).send("wasmunittest"_n);
}
```

### Build The Test

To build the test you'll have to build the `hello` sources which includes the tests.

```sh
cd EOSIO_CDT_EXAMPLE_HELLO_FOLDER
rm -fr build
mkdir build
cd build
cmake ..
make
```

### Run The Tests

To run the tests run the following set of commands

```sh
ctest -R wasm_unit_tests$
```
