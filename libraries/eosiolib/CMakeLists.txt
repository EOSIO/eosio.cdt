file(GLOB HEADERS "*.hpp"
                  "*.h")
add_library(fpconv tester/fpconv.c)
add_library(abieos_crypto ${CMAKE_SOURCE_DIR}/abieos/src/crypto.cpp)

add_library(eosio STATIC
            eosiolib.cpp
            crypto.cpp
            ${HEADERS})

add_library(eosio_malloc STATIC
            malloc.cpp
            ${HEADERS})

add_library(eosio_dsm STATIC
            simple_malloc.cpp
            ${HEADERS})

add_library(eosio_cmem STATIC
            memory.cpp
            ${HEADERS})

add_library(eosio_tester STATIC
            tester/tester.cpp
            tester/tester_intrinsics.cpp
            ${HEADERS})

add_library(eosio_embed STATIC
            embed/eosio/eosio_assert.cpp
            embed/eosio/printi.cpp
            embed/eosio/printn.cpp
            embed/eosio/prints.cpp
            embed/eosio/printui.cpp
            ${HEADERS})

target_include_directories(eosio_tester PUBLIC
                                        ${CMAKE_SOURCE_DIR}/eosiolib/tester
                                        ${CMAKE_SOURCE_DIR}/eosiolib/core
                                        ${CMAKE_SOURCE_DIR}/eosiolib/contract
                                        ${CMAKE_SOURCE_DIR}/abieos/include
                                        ${CMAKE_SOURCE_DIR}/external/rapidjson/include
                                        ${CMAKE_SOURCE_DIR}/boost/include)

set(tester_includes -I${CMAKE_SOURCE_DIR}/libc++/libcxx/include
                    -I${CMAKE_SOURCE_DIR}/libc/musl/include
                    -I${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                    -I${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                    -I${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                    -I${CMAKE_SOURCE_DIR}/eosiolib/tester
                    -I${CMAKE_SOURCE_DIR}/eosiolib/core
                    -I${CMAKE_SOURCE_DIR}/eosiolib/contracts
                    -I${CMAKE_SOURCE_DIR}/abieos/include
                    -I${CMAKE_SOURCE_DIR}/abieos/src
                    -I${CMAKE_SOURCE_DIR}/abieos/external/rapidjson/include
                    -I${CMAKE_SOURCE_DIR}/Catch2/single_include
                    -I${CMAKE_SOURCE_DIR}/fmt/include)

add_custom_command(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/crt0.o
   COMMAND ${CMAKE_CXX_COMPILER} -c -o ${CMAKE_CURRENT_BINARY_DIR}/crt0.o ${tester_includes} ${CMAKE_SOURCE_DIR}/eosiolib/tester/crt0.cpp
   DEPENDS ${CMAKE_SOURCE_DIR}/eosiolib/tester/crt0.cpp
   COMMENT "Building C++ object crt0.o"
)
add_custom_target(crt0.o.target DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/crt0.o)
add_dependencies(eosio_tester crt0.o.target)

# TODO re-enable after more CMake fixes
#add_native_library(native_eosio
#                   eosiolib.cpp
#                   crypto.cpp
#                   malloc.cpp
#                   ${HEADERS})

set_target_properties(eosio_malloc PROPERTIES LINKER_LANGUAGE C)

target_include_directories(abieos_crypto PRIVATE
                           ${CMAKE_SOURCE_DIR}/abieos/include
                           ${CMAKE_SOURCE_DIR}/abieos/external/rapidjson/include)

target_include_directories(eosio PUBLIC
                                 ${CMAKE_SOURCE_DIR}/eosiolib/core
                                 ${CMAKE_SOURCE_DIR}/eosiolib/contract
                                 ${CMAKE_SOURCE_DIR}/abieos/include
                                 ${CMAKE_SOURCE_DIR}/abieos/external/rapidjson/include
                                 ${CMAKE_SOURCE_DIR}/boost/include)

                              #target_include_directories(native_eosio PUBLIC
                              #                                 ${CMAKE_SOURCE_DIR}/eosiolib/core
                              #                                 ${CMAKE_SOURCE_DIR}/eosiolib/contract
                              #                                 ${CMAKE_SOURCE_DIR}/abieos/include
                              #                                 ${CMAKE_SOURCE_DIR}/abieos/external/rapidjson/include
                              #                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_include_directories(eosio_embed PUBLIC
                                 ${CMAKE_SOURCE_DIR}/eosiolib/core
                                 ${CMAKE_SOURCE_DIR}/abieos/external/rapidjson/include
                                 ${CMAKE_SOURCE_DIR}/abieos/include)

target_link_libraries( fpconv PUBLIC c )
target_link_libraries( eosio PUBLIC c++ fpconv abieos_crypto )
#target_link_libraries( native_eosio PUBLIC c++ fpconv abieos_crypto )
target_link_libraries( eosio_embed PUBLIC c++ fpconv abieos_crypto)
target_link_libraries( eosio_tester PUBLIC abieos_crypto eosio )
target_link_libraries( eosio_malloc PUBLIC eosio )
target_link_libraries( eosio_dsm PUBLIC eosio abieos_crypto )
target_link_libraries( eosio_cmem PUBLIC c++ )

target_compile_options(eosio PRIVATE "-fno-default-paths")
#target_compile_options(native_eosio PRIVATE "-fno-default-paths")
target_compile_options(eosio_malloc PRIVATE "-fno-default-paths")
target_compile_options(eosio_dsm PRIVATE "-fno-default-paths")
target_compile_options(eosio_cmem PRIVATE "-fno-default-paths")
target_compile_options(eosio_tester PRIVATE "-fno-default-paths")
target_compile_options(eosio_embed PRIVATE "-fno-default-paths")

#add_dependencies( native_eosio eosio )

add_custom_command( TARGET eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_malloc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_malloc> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_dsm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_dsm> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_cmem POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_cmem> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_tester> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_embed POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_embed> ${BASE_BINARY_DIR}/lib )
#add_custom_command( TARGET native_eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_eosio> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET eosio_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/crt0.o ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/include/eosio DESTINATION ${BASE_BINARY_DIR}/include/abieos FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos.h DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos_numeric.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/src/abieos_ripemd160.hpp DESTINATION ${BASE_BINARY_DIR}/include/abieos)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/external/date/include/date DESTINATION ${BASE_BINARY_DIR}/include/date FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../abieos/external/rapidjson/include/rapidjson DESTINATION ${BASE_BINARY_DIR}/include/rapidjson FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../fmt/include/fmt DESTINATION ${BASE_BINARY_DIR}/include/fmt FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Catch2/single_include/catch2 DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
