enable_testing()

set(unit_test_dir ${CMAKE_BINARY_DIR}/EosioWasmTests-prefix/src/EosioWasmTests-build)

add_test(name_tests ${unit_test_dir}/name_tests.wasm)
