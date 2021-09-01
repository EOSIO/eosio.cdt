SET(SRC_FILENAMES algorithm.cpp any.cpp bind.cpp charconv.cpp condition_variable.cpp functional.cpp
                  future.cpp ios.cpp iostream.cpp locale.cpp memory.cpp mutex.cpp new.cpp optional.cpp
                  regex.cpp stdexcept.cpp string.cpp strstream.cpp system_error.cpp exception.cpp
                  typeinfo.cpp utility.cpp valarray.cpp variant.cpp vector.cpp hash.cpp)

SET(SRC_FILES "")
FOREACH(FN ${SRC_FILENAMES})
  LIST(APPEND SRC_FILES "libcxx/src/${FN}")
ENDFOREACH(FN)
LIST(APPEND SRC_FILES "libcxxabi/src/private_typeinfo.cpp")
LIST(APPEND SRC_FILES "libcxxabi/src/cxa_demangle.cpp")
LIST(APPEND SRC_FILES "libcxxabi/src/cxa_virtual_fallback.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything -frtti")

add_library(c++ ${SRC_FILES})
target_compile_options(c++ PUBLIC -Wno-everything -frtti --no-missing-ricardian-clause)
target_compile_definitions(c++ PUBLIC _LIBCPP_BUILDING_LIBRARY _LIBCXXABI_BUILDING_LIBRARY)

target_include_directories(c++
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libcxxabi/include
  PRIVATE
    ${CMAKE_SOURCE_DIR}/libc/musl/include
    ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
    ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos)

target_link_libraries(c++ c cstdio)

add_custom_command( TARGET c++ POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c++> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include/ DESTINATION ${BASE_BINARY_DIR}/include/c++/v1)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libcxxabi/include/ DESTINATION ${BASE_BINARY_DIR}/include/c++/v1)

set(UNSUPPORTED_HEADERS
   ${BASE_BINARY_DIR}/include/libcxx/fstream
   ${BASE_BINARY_DIR}/include/libcxx/future
   ${BASE_BINARY_DIR}/include/libcxx/random
   ${BASE_BINARY_DIR}/include/libcxx/shared_mutex)
file(REMOVE ${UNSUPPORTED_HEADERS})
