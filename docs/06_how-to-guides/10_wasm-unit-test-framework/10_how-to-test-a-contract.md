---
content_title: How to test a contract using WASM Unit Test Framework
link_text: How to test a contract
---

## Summary

If you are a smart contract developer and want to build unit tests for your smart contracts then WASM Unit Test Framework is a viable option. It allows you to write unit tests which can be grouped into fixtures for each smart contract actions or even more complex scenarios, e.g. multiple action execution scenarios. By following this how to step by step you will learn the way to employ the WASM Unit Test Framework to build unit tests for one or more of your smart contracts.

## Prerequisites

* `eosio.cdt` built and/or installed ready to build your smart contract source code.

## Procedure

### 1. Create Unit Test Source File

The result of this step is to have your C++ unit test sources created as the first step, they can be empty, meaning no tests are implemented yet instead we are merely setting up the test source layout.

### 2. Include WASM Unit Test Framework Header File

The result of this step is to link your previously created unit test source file with the WASM Unit Test Framework. You do this by adding the following to your test source file.

```
#include <eosio/tester.hpp>
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
```

### 3. Include the Smart Contract Header File

The result of this action is to allow your unit test source to access the smart contract you want to test. You do that by including your smart contract header file/s to your unit test source file. Your header section should look similar to the one below:

```
#include <eosio/tester.hpp>
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <your_smart_contract_source.hpp>
```

### 4. Create a Test Fixture Class

In order to group your tests in a test fixture you have to create a test fixture class.

```
class first_tests_fixture {
  private:
    int unique_test_id;
  public:
   eosio::test_chain test_chain;
   first_tests_fixture() {
       this->unique_test_id = 0;
   }
  protected:
   int get_test_id() {
     return ++ this->unique_test_id;
   }
 };
```

The result of this step is to have a class which will represent your test fixture for the future yet to build unit tests. You can rename the `first_tests_fixture` to your liking but be sure if you do that to rename it everywhere else you will see it from here onwards.

### 5. Create a Test Method Within the Test Fixture Class

Create your first unit test using the construct below.

```
TEST_CASE_METHOD(first_tests_fixture, "Very short description of the unit test", "Very short description of the test fixture") {
}
```

The result of this step is to have implemented the skeleton for your first unit test. It is recommended to provide short descriptions for the fixture and the unit test method, using too long strings might produce logs that are harder to read.

### 6. Implement the Test Method

Now it is time to add your implementation for the unit test method. In the below example we create one account `hello`, another one `wasmunittest`, we set the code for the contract `hello` to the `test` account and then we send an action `hi` to that the `test` account with `wasmunittest` parameter.

```
TEST_CASE_METHOD(first_tests_fixture, "Action hi::send", "[send]") {
   create_account("test"_n);
   create_account("wasmunittest"_n);
   set_code("test"_n, "../hello/hello.wasm");
   hello::hi_action("test"_n, { "test"_n, "active"_n }).send("wasmunittest"_n);
}
```

The result of this test is to have a full implementation of the unit test method.

### 7. Build the Test Source

Build your test and make sure it compiles without errors.
The result of this step is to have a compiled test that ca be run and its results available for inspection.

## Next Steps

You can now run your test, see if it is failing or passing and fix found errors.

For detailed step by step instructions on how to accomplish what is summarized in this how to please check the [`WASM Unit Test Framework`](../09_tutorials/03_wasm-unit-test-framework.md) tutorial.
