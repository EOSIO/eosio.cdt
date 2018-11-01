SET(SRC_FILENAMES algorithm.cpp any.cpp bind.cpp condition_variable.cpp functional.cpp
                  future.cpp ios.cpp iostream.cpp locale.cpp memory.cpp mutex.cpp new.cpp optional.cpp
                  regex.cpp stdexcept.cpp string.cpp strstream.cpp system_error.cpp exception.cpp
                  typeinfo.cpp utility.cpp valarray.cpp variant.cpp vector.cpp eosio.cpp)

SET(SRC_FILES "")
FOREACH(FN ${SRC_FILENAMES})
   LIST(APPEND SRC_FILES "libcxx/src/${FN}")
ENDFOREACH(FN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything")

add_library(c++
            ${SRC_FILES})

target_include_directories(c++
                           PUBLIC 
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)

target_link_libraries(c++ c)

install(TARGETS c++ EXPORT EosioLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} 
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

add_custom_command( TARGET c++ POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c++> ${CMAKE_BINARY_DIR}/lib )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/libcxx)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include/ DESTINATION ${CMAKE_BINARY_DIR}/include/libcxx)

