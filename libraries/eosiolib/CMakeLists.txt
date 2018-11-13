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
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)

target_include_directories(native_eosio PUBLIC
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)

target_link_libraries( eosio c c++ )
target_link_libraries( native_eosio native_c native_c++ )

install(TARGETS eosio EXPORT EosioLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

install(TARGETS native_eosio EXPORT EosioLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${CMAKE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
add_custom_command( TARGET eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio> ${CMAKE_BINARY_DIR}/lib )
add_custom_command( TARGET native_eosio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_eosio> ${CMAKE_BINARY_DIR}/lib )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
