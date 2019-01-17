project(hello_tests)
find_package(eosio.cdt)
add_native_executable(hello_test hello_test.cpp ${CMAKE_SOURCE_DIR}/../src/hello.cpp)
target_include_directories(hello_test PUBLIC ${CMAKE_SOURCE_DIR}/../include)
