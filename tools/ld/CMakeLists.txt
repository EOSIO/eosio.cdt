configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cdt-ld.cpp.in ${CMAKE_BINARY_DIR}/cdt-ld.cpp)

add_tool(cdt-ld)

set_target_properties(cdt-ld PROPERTIES LINK_FLAGS "-Wl,-rpath,\"\\$ORIGIN/../lib\"")
