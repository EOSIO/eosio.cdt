add_contract(malloc_tests malloc_tests malloc_tests.cpp)
add_contract(malloc_tests old_malloc_tests malloc_tests.cpp)
add_contract(simple_tests simple_tests simple_tests.cpp)
add_contract(transfer_contract transfer_contract transfer.cpp)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/simple_wrong.abi ${CMAKE_CURRENT_BINARY_DIR}/simple_wrong.abi COPYONLY )

target_link_libraries(old_malloc_tests PUBLIC --use-freeing-malloc)
