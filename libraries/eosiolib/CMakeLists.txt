file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(eosio 
            eosiolib.cpp 
            ${HEADERS})

target_include_directories(eosio PUBLIC 
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/eosiosdk/include/>)

target_link_libraries( eosio c c++ )

install(TARGETS eosio EXPORT EosioLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/eosiosdk/lib/
   ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/eosiosdk/lib/)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${CMAKE_INSTALL_PREFIX}/eosiosdk/include/eosiolib)
