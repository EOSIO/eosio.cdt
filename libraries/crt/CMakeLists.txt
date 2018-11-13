file(GLOB SOURCES _crt.s crt.cpp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything")

add_native_library(crt 
            ${SOURCES})

add_dependencies(crt EosioTools)

install(TARGETS crt EXPORT EosioLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} 
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

add_custom_command( TARGET crt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:crt> ${CMAKE_BINARY_DIR}/lib )
