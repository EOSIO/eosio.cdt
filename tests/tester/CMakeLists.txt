cmake_minimum_required(VERSION 3.5)

project(tests)

set(EOSIO_WASM_OLD_BEHAVIOR "off")
find_package(eosio.cdt)

function(add_test TARGET SRC)
   add_executable(${TARGET} ${SRC})
   target_compile_definitions(${TARGET} PUBLIC CATCH_CONFIG_NO_POSIX_SIGNALS )
   target_link_options(${TARGET} PRIVATE -Wl,-zstack-size=65536 -Wl,--entry,initialize -Wl,--export,start -Wl,--export-table -Wl,--stack-first)
   target_link_libraries(${TARGET} PUBLIC eosio_tester ${crt0_o})
   target_compile_options(${TARGET} PUBLIC -Os)
   set_target_properties(${TARGET}
     PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

add_test(tester tester.cpp)
