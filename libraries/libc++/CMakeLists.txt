SET(SRC_FILENAMES algorithm.cpp any.cpp bind.cpp chrono.cpp condition_variable.cpp functional.cpp
                  future.cpp hash.cpp ios.cpp iostream.cpp locale.cpp memory.cpp mutex.cpp new.cpp optional.cpp
                  regex.cpp stdexcept.cpp string.cpp strstream.cpp system_error.cpp exception.cpp
                  typeinfo.cpp utility.cpp valarray.cpp variant.cpp vector.cpp eosio.cpp)

SET(SRC_FILES "")
FOREACH(FN ${SRC_FILENAMES})
   LIST(APPEND SRC_FILES "libcxx/src/${FN}")
ENDFOREACH(FN)

add_library(c++ STATIC ${SRC_FILES})

add_native_library(native_c++ STATIC ${SRC_FILES})

target_include_directories(c++
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include)

target_include_directories(native_c++
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include)

target_link_libraries(c++ PUBLIC c)
target_link_libraries(native_c++ PUBLIC native_c)

target_compile_options(c++ PUBLIC "-Wno-everything" "-allow-sse" "-fno-default-paths")
target_compile_options(native_c++ PUBLIC "-Wno-everything" "-allow-sse" "-fno-default-paths")

add_custom_command( TARGET c++ POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c++> ${BASE_BINARY_DIR}/lib )

add_custom_command( TARGET native_c++ POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_c++> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libcxx/include/ DESTINATION ${BASE_BINARY_DIR}/include/libcxx)
