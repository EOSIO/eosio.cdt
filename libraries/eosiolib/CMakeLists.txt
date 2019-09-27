file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(eosio
            eosiolib.cpp
            crypto.cpp
            ${HEADERS})

add_library(eosio_malloc
            malloc.cpp
            ${HEADERS})

add_library(eosio_dsm
            simple_malloc.cpp
            ${HEADERS})

add_library(eosio_cmem
            memory.cpp
            ${HEADERS})

add_library(eosio_tester
            tester/tester_intrinsics.cpp
            tester/fpconv.c
            ${HEADERS})

add_native_library(native_eosio
                   eosiolib.cpp
                   crypto.cpp
                   malloc.cpp
                   ${HEADERS})

set_target_properties(eosio_malloc PROPERTIES LINKER_LANGUAGE C)

target_include_directories(eosio PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 ${CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_link_libraries( eosio c c++ )
add_dependencies( native_eosio eosio )

add_custom_command( TARGET eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_malloc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_malloc> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_dsm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_dsm> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_cmem POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_cmem> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_tester> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_eosio> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos.h DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos_numeric.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos_ripemd160.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/external/date/include/date DESTINATION ${BASE_BINARY_DIR}/include/date FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/external/rapidjson/include/rapidjson DESTINATION ${BASE_BINARY_DIR}/include/rapidjson FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../fmt/include/fmt DESTINATION ${BASE_BINARY_DIR}/include/fmt FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
