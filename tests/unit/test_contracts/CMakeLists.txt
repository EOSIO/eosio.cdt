add_contract(malloc_tests malloc_tests malloc_tests.cpp)
add_contract(rope_tests rope_tests rope_tests.cpp)

target_compile_options(rope_tests PUBLIC -Os)
