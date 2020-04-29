---
content_title: WASM Unit Test Framework Tutorial
link_text: WASM Unit Test Framework Tutorial
---

# Overview

In this tutorial you will learn how to use the WASM Unit Test Framework to test a simple smart contract.

## Create the Smart Contract and the Accompanying Test File

For exemplification we will test the [`hello` smart contract](https://github.com/EOSIO/eosio.cdt/tree/master/examples/hello).

Please make sure that the smart contract sources are copied to your local drive in folder `EOSIO_CDT_EXAMPLE_HELLO_FOLDER` and can compile without error using `eosio.cdt`.

Navigate to that folder on your local drive and create next to the already existing `tests` folder a second one `wasm_unit_tests` and then switch your current location to the newly created folder and then create two empty files `CMakeLists.txt` and `wasm_unit_tests.cpp`.

```
git clone git@github.com:EOSIO/eosio.cdt.git EOSIO_CDT_EXAMPLE_HELLO_FOLDER
cd EOSIO_CDT_EXAMPLE_HELLO_FOLDER
mkdir wasm_unit_tests
cd wasm_unit_tests
touch CMakeLists.txx
touch wasm_unit_test_first.cpp
```

Replace the content of the `CMakeLists.txt` with the following using your favorite source editor:

```
project(wasm_unit_tests)
find_package(eosio.cdt)
add_native_executable(wasm_unit_test_first wasm_unit_test_first.cpp ${CMAKE_SOURCE_DIR}/../src/hello.cpp)
target_include_directories(wasm_unit_test_first PUBLIC ${CMAKE_SOURCE_DIR}/../include)
```

## Reference the WASM Unit Test Framework

In order to be able to make use of the WASM Unit Test Framework constructs we need to reference the `tester.hpp` header file which can be done by adding below line to the top of our test source file:

```
#include <eosio/tester.hpp>
```

## Create the test fixture class

Catch2 test framework allows one to group tests together by means of sessions within a test case however we will demonstrate the more traditional way of grouping tests under test fixture. For that we will use have to create test fixture class `HelloTestsFixture` and then pass the class name to the `TEST_CASE_METHOD` macro provided by the `catch2` framework.

```
class HelloTestsFixture {
  private:
    int uniqueTestID;
  public:
   HelloTestsFixture() {
       this->uniqueTestID = 0;
   }
  protected:
   int getTestID() {
     return ++this->uniqueTestID;
   }
 };
```


## Create First Test

Now we are ready to create the first unit test. To make testing easier the WASM Unit Test Framework provides an utility class `test_chain` which is simulating a local blockchain for your tests to run against. Only one `test_chain` instance can exist at a time. The test chain uses simulated time starting at 2020-01-01T00:00:00.000. We will use this class in conjunction with the `HelloTestsFixture` class we created earlier by instantiating an instance of it in the constructor of the test fixture.

TO DO update code below
```
class HelloTestsFixture {
  private:
    int uniqueTestID;
    eosio::test_chain blockchain;
  public:
   HelloTestsFixture() {
       this->uniqueTestID = 0;
   }
  protected:
   int getTestID() {
     return ++uniqueTestID;
   }
 };

TEST_CASE_METHOD(HelloTestsFixture, "Hello first test", "[multi_index]") {
   create_account("test"_n);
   set_code("test"_n, "../unit/test_contracts/tester_tests.wasm");
   tester_tests::putdb_action("test"_n, { "test"_n, "active"_n }).send(1, 2);
   tester_tests::table t("test"_n, 0);
   for(auto& item : t) {
      CHECK(item.key == 1);
      CHECK(item.value == 2);
   }
}
```

## Create Second Test

Let's create a second test and place it in the same fixture. We could chose to place it in a second fixture, it all depends how one wants to organize the unit tests.

TO DO: update code below
```
TEST_CASE_METHOD(HelloTestsFixture, "Hello second test", "[multi_index]") {
   create_account("test"_n);
   set_code("test"_n, "../unit/test_contracts/tester_tests.wasm");
   tester_tests::putdb_action("test"_n, { "test"_n, "active"_n }).send(1, 2);
   tester_tests::table t("test"_n, 0);
   for(auto& item : t) {
      CHECK(item.key == 1);
      CHECK(item.value == 2);
   }
}
```

## Build The Test

To build the test you'll have to build the `hello` sources which includes the tests.

```
cd EOSIO_CDT_EXAMPLE_HELLO_FOLDER
rm -fr build
mkdir build
cd build
cmake ..
make
```

## Run The Tests

To run the tests run the following set of commands

```
ctest -R wasm_unit_tests$
```