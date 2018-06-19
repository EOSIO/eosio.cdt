configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core_symbol.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/core_symbol.hpp)

file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(eosio 
            eosiolib.cpp 
            ${HEADERS})

target_include_directories(eosio PUBLIC 
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries( eosio c c++ )

install(TARGETS eosio EXPORT EosioLib
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
