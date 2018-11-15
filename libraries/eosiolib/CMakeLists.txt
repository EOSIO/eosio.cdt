file(GLOB HEADERS "*.hpp"
                  "*.h")

add_wasm_cxx_library(eosio
            eosiolib.cpp
            crypto.cpp
            ${HEADERS})

target_include_directories(eosio PUBLIC
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)

target_link_libraries( eosio c c++ )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${CMAKE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

add_custom_command( TARGET eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio> ${CMAKE_BINARY_DIR}/lib )
