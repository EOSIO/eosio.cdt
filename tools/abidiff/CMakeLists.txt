configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cdt-abidiff.cpp.in ${CMAKE_BINARY_DIR}/cdt-abidiff.cpp)

add_tool(cdt-abidiff)

set_target_properties(cdt-abidiff PROPERTIES LINK_FLAGS "-Wl,-rpath,\"\\$ORIGIN/../lib\"")
