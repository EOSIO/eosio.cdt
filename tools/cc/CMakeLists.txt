configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cdt-cc.cpp.in ${CMAKE_BINARY_DIR}/cdt-cc.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cdt-cpp.cpp.in ${CMAKE_BINARY_DIR}/cdt-cpp.cpp)

add_tool(cdt-cc)
add_tool(cdt-cpp)

set_target_properties(cdt-cc PROPERTIES LINK_FLAGS "-Wl,-rpath,\"\\$ORIGIN/../lib\"")
set_target_properties(cdt-cpp PROPERTIES LINK_FLAGS "-Wl,-rpath,\"\\$ORIGIN/../lib\"")
