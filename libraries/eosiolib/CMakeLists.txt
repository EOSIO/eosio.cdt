file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(eosio
            eosiolib.cpp
            crypto.cpp
            ${HEADERS})

add_native_library(native_eosio
                   eosiolib.cpp
                   crypto.cpp
                   ${HEADERS})

target_include_directories(eosio PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 {CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_include_directories(native_eosio PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 ${CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_link_libraries( eosio c c++ )
target_link_libraries( native_eosio native_c native_c++ )
add_dependencies( native_eosio eosio )


add_custom_command( TARGET eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_eosio> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
