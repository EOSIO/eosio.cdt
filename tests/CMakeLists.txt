set(unit_test_dir ${CMAKE_BINARY_DIR}/EosioWasmTests-prefix/src/EosioWasmTests-build)

add_test(name_tests ${unit_test_dir}/name_tests)
add_test(system_tests ${unit_test_dir}/system_tests)
add_test(print_tests ${unit_test_dir}/print_tests)
