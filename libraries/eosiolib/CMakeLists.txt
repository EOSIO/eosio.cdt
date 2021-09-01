set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(eosio
            eosiolib.cpp
            crypto.cpp
            tester/fpconv.c
            ${CMAKE_SOURCE_DIR}/abieos/src/crypto.cpp
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
            tester/tester.cpp
            tester/tester_intrinsics.cpp
            ${CMAKE_SOURCE_DIR}/abieos/src/crypto.cpp
            ${HEADERS})

add_library(eosio_embed
            tester/fpconv.c
            ${CMAKE_SOURCE_DIR}/abieos/src/crypto.cpp
            embed/eosio/eosio_assert.cpp
            embed/eosio/printi.cpp
            embed/eosio/printn.cpp
            embed/eosio/prints.cpp
            embed/eosio/printui.cpp
            ${HEADERS})

set(abieos_includes ${CMAKE_SOURCE_DIR}/abieos/include
                    ${CMAKE_SOURCE_DIR}/abieos/src
                    ${CMAKE_SOURCE_DIR}/abieos/external/rapidjson/include)

target_include_directories(eosio_tester PUBLIC
                                        ${CMAKE_SOURCE_DIR}/eosiolib/tester
                                        ${CMAKE_SOURCE_DIR}/eosiolib/core
                                        ${CMAKE_SOURCE_DIR}/eosiolib/contracts
                                        ${abieos_includes}
                                        ${CMAKE_SOURCE_DIR}/boost/include
                                        ${CMAKE_SOURCE_DIR}/fmt/include
                                        ${CMAKE_SOURCE_DIR}/meta_refl/include)

target_compile_options(eosio_tester PRIVATE -fno-default-paths)
target_link_libraries(eosio_tester c++)

add_custom_command(
   OUTPUT ${BASE_BINARY_DIR}/lib/crt0.o
   COMMAND ${CMAKE_CXX_COMPILER} -fno-default-paths -c -o ${BASE_BINARY_DIR}/lib/crt0.o "-I$<JOIN:$<TARGET_PROPERTY:eosio_tester,INCLUDE_DIRECTORIES>,;-I>" ${CMAKE_SOURCE_DIR}/eosiolib/tester/crt0.cpp
   DEPENDS ${CMAKE_SOURCE_DIR}/eosiolib/tester/crt0.cpp
   COMMENT "Building C++ object crt0.o"
   COMMAND_EXPAND_LISTS
)
add_custom_target(crt0.o.target DEPENDS ${BASE_BINARY_DIR}/lib/crt0.o)
add_dependencies(eosio_tester crt0.o.target)

add_native_library(native_eosio
                   eosiolib.cpp
                   crypto.cpp
                   malloc.cpp
                   ${HEADERS})

set_target_properties(eosio_malloc PROPERTIES LINKER_LANGUAGE C)

target_include_directories(eosio PUBLIC
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/meta_refl/include
                                 ${CMAKE_SOURCE_DIR}/eosiolib/core
                                 ${CMAKE_SOURCE_DIR}/eosiolib/contracts
                                 ${abieos_includes}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_include_directories(native_eosio PUBLIC
                                 ${CMAKE_SOURCE_DIR}/eosiolib/core
                                 ${CMAKE_SOURCE_DIR}/eosiolib/contracts
                                 ${abieos_includes}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_link_libraries( eosio c++ c )
add_dependencies( native_eosio eosio )

add_custom_command( TARGET eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_malloc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_malloc> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_dsm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_dsm> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_cmem POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_cmem> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_tester> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_embed POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_embed> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_eosio> ${BASE_BINARY_DIR}/lib )


file(GLOB_RECURSE eosio_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib "*.h" "*.hpp")
foreach (f ${eosio_headers})
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib/${f} ${BASE_BINARY_DIR}/include/eosiolib/${f})
endforeach()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/include/eosio DESTINATION ${BASE_BINARY_DIR}/include/abieos FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos.h DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos_numeric.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos_ripemd160.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/external/rapidjson/include/rapidjson DESTINATION ${BASE_BINARY_DIR}/include/rapidjson FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../fmt/include/fmt DESTINATION ${BASE_BINARY_DIR}/include/fmt FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Catch2/single_include/catch2 DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
