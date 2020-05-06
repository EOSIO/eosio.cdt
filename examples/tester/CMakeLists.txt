cmake_minimum_required(VERSION 3.5)

project(tests)

set(EOSIO_WASM_OLD_BEHAVIOR "off")
find_package(eosio.cdt)

function(add_test TARGET SRC)
   add_executable(${TARGET} ${SRC})
   target_link_libraries(${TARGET} -ftester -stack-size=65536)
   target_compile_options(${TARGET} PUBLIC -ftester -Os)
   set_target_properties(${TARGET}
     PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

add_test(tester tester.cpp)
