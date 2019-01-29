add_contract(malloc_tests malloc_tests malloc_tests.cpp)
add_contract(malloc_tests old_malloc_tests malloc_tests.cpp)
add_contract(simple_tests simple_tests simple_tests.cpp)

target_link_libraries(old_malloc_tests PUBLIC --use-freeing-malloc)
