file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(eosio 
            eosiolib.cpp 
            ${HEADERS})

target_include_directories(eosio PUBLIC 
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${SDK_INSTALL_PREFIX}/include/>)

target_link_libraries( eosio c c++ )

install(TARGETS eosio EXPORT EosioLib
   LIBRARY DESTINATION ${SDK_INSTALL_PREFIX}/lib/
   ARCHIVE DESTINATION ${SDK_INSTALL_PREFIX}/lib/)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${SDK_INSTALL_PREFIX}/include)
