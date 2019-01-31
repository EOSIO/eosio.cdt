set(unit_test_dir ${CMAKE_BINARY_DIR}/EosioWasmTests-prefix/src/EosioWasmTests-build)

add_test(asset_tests ${unit_test_dir}/asset_tests)
add_test(crypto_tests ${unit_test_dir}/crypto_tests)
add_test(name_tests ${unit_test_dir}/name_tests)
add_test(rope_tests ${unit_test_dir}/rope_tests)
add_test(symbol_tests ${unit_test_dir}/symbol_tests)
add_test(system_tests ${unit_test_dir}/system_tests)

if (eosio_FOUND)
   add_test(integration_tests ${CMAKE_BINARY_DIR}/tests/integration/integration_tests)
endif()
