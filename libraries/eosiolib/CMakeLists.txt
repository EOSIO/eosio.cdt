configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core_symbol.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/core_symbol.hpp)

#add_wast_library(TARGET eosiolib
#  INCLUDE_FOLDERS "${STANDARD_INCLUDE_FOLDERS}" ${CMAKE_SOURCE_DIR}/externals/magic_get/include
#  DESTINATION_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
#)

file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(eosio 
            eosiolib.cpp 
            ${HEADERS})

target_include_directories(eosio
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/../libc++/upstream/include"
                                  "${BOOST_INSTALL_DIR}")

target_link_libraries( eosio c c++ )
