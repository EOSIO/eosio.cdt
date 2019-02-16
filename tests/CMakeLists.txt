add_test(unit_tests COMMAND bash ${CMAKE_BINARY_DIR}/tests/unit/test_runner.sh)
if (eosio_FOUND AND EOSIO_RUN_INTEGRATION_TESTS)
   add_test(integration_tests ${CMAKE_BINARY_DIR}/tests/integration/integration_tests)
endif()

add_test(hello_sanity_tests COMMAND bash ${CMAKE_BINARY_DIR}/tests/sanity/hello_test.sh)
