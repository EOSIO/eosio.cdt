SET(SRC_FILENAMES algorithm.cpp any.cpp bind.cpp charconv.cpp condition_variable.cpp functional.cpp
                  future.cpp ios.cpp iostream.cpp locale.cpp memory.cpp mutex.cpp new.cpp optional.cpp
                  regex.cpp stdexcept.cpp string.cpp strstream.cpp system_error.cpp exception.cpp
                  typeinfo.cpp utility.cpp valarray.cpp variant.cpp vector.cpp eosio.cpp hash.cpp)

SET(SRC_FILES "")
FOREACH(FN ${SRC_FILENAMES})
   LIST(APPEND SRC_FILES "libcxx/src/${FN}")
ENDFOREACH(FN)
LIST(APPEND SRC_FILES "libcxxabi/src/private_typeinfo.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything -frtti")

add_library(c++
            ${SRC_FILES})

add_native_library(native_c++
            ${SRC_FILES})

add_definitions(-D_LIBCPP_BUILDING_LIBRARY -D_LIBCXXABI_BUILDING_LIBRARY)

target_include_directories(c++
   PUBLIC 
      ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include
      ${CMAKE_CURRENT_SOURCE_DIR}/libcxxabi/include
   PRIVATE
      ${CMAKE_SOURCE_DIR}/libc/musl/include
      ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
      ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
)

target_include_directories(native_c++
   PUBLIC 
      ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include
      ${CMAKE_CURRENT_SOURCE_DIR}/libcxxabi/include
   PRIVATE
      ${CMAKE_SOURCE_DIR}/libc/musl/include
      ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
      ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
)

target_link_libraries(c++ c)
target_link_libraries(native_c++ native_c)

target_compile_options(native_c++ PUBLIC -allow-sse)

add_custom_command( TARGET c++ POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c++> ${BASE_BINARY_DIR}/lib )

add_custom_command( TARGET native_c++ POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_c++> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include/ DESTINATION ${BASE_BINARY_DIR}/include/libcxx)
