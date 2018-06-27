SET(SRC_FILENAMES algorithm.cpp any.cpp bind.cpp condition_variable.cpp exception.cpp functional.cpp
                  future.cpp ios.cpp iostream.cpp locale.cpp memory.cpp mutex.cpp new.cpp optional.cpp
                  regex.cpp shared_mutex.cpp stdexcept.cpp string.cpp strstream.cpp system_error.cpp
                  thread.cpp typeinfo.cpp utility.cpp valarray.cpp variant.cpp vector.cpp)

#SET(SRC_FILENAMES exception.cpp)

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
                           $<INSTALL_INTERFACE:${SDK_INSTALL_PREFIX}/include/>)

target_link_libraries(c++ c)

install(TARGETS c++ EXPORT EosioLib
   LIBRARY DESTINATION ${SDK_INSTALL_PREFIX}/lib/
   ARCHIVE DESTINATION ${SDK_INSTALL_PREFIX}/lib/)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include/ DESTINATION ${SDK_INSTALL_PREFIX}/include/libcxx)
