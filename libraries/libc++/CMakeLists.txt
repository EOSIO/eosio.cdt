
set(SRC_FILENAMES algorithm.cpp any.cpp bind.cpp charconv.cpp chrono.cpp condition_variable.cpp functional.cpp
                  future.cpp ios.cpp ios.instantiations.cpp iostream.cpp locale.cpp memory.cpp mutex.cpp new.cpp optional.cpp
                  regex.cpp stdexcept.cpp string.cpp strstream.cpp system_error.cpp exception.cpp
                  typeinfo.cpp utility.cpp valarray.cpp variant.cpp vector.cpp hash.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything -frtti")

set(LIBCXX_SRC_DIR ${llvm_SOURCE_DIR}/libcxx/src/)
set(LIBCXXABI_SRC_DIR ${llvm_SOURCE_DIR}/libcxxabi/src/)

add_library(c++ "cxa_virtual_fallback.cpp" 
           ${LIBCXXABI_SRC_DIR}/private_typeinfo.cpp
           ${LIBCXXABI_SRC_DIR}/cxa_demangle.cpp
           ${LIBCXX_SRC_DIR}$<JOIN:${SRC_FILENAMES}, ${LIBCXX_SRC_DIR}>)
target_compile_options(c++ PUBLIC -Wno-everything -frtti --no-missing-ricardian-clause)
target_compile_definitions(c++ PRIVATE _LIBCPP_USE_CLOCK_GETTIME _LIBCPP_BUILDING_LIBRARY _LIBCXXABI_BUILDING_LIBRARY)
target_include_directories(c++ PRIVATE ${LIBCXX_SRC_DIR})

set(_LIBCPP_HAS_NO_THREADS ON)
set(_LIBCPP_HAS_MUSL_LIBC ON)
set(_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS ON)
set(_LIBCPP_HAS_NO_VENDOR_AVAILABILITY_ANNOTATIONS ON)
set(_LIBCPP_ABI_DEFINES "#define _LIBCPP_NO_EXCEPTIONS")

configure_file(${llvm_SOURCE_DIR}/libcxx/include/__config_site.in ${BASE_BINARY_DIR}/include/c++/v1/__config_site)
file(COPY ${llvm_SOURCE_DIR}/libcxx/include/ DESTINATION ${BASE_BINARY_DIR}/include/c++/v1)
file(COPY ${llvm_SOURCE_DIR}/libcxxabi/include/ DESTINATION ${BASE_BINARY_DIR}/include/c++/v1)

# target_include_directories(c++
#   PUBLIC 
#     ${BASE_BINARY_DIR}/include/c++/v1
#     ${musl_SOURCE_DIR}/include
#     ${musl_SOURCE_DIR}/src/internal
#     ${musl_SOURCE_DIR}/arch/eos)

target_link_libraries(c++ c cstdio)

add_custom_command( TARGET c++ POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c++> ${BASE_BINARY_DIR}/lib )

set(UNSUPPORTED_HEADERS
   ${BASE_BINARY_DIR}/include/libcxx/fstream
   ${BASE_BINARY_DIR}/include/libcxx/future
   ${BASE_BINARY_DIR}/include/libcxx/random
   ${BASE_BINARY_DIR}/include/libcxx/shared_mutex)
file(REMOVE ${UNSUPPORTED_HEADERS})
