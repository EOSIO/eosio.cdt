add_executable( hello hello.cpp )
target_include_directories(hello PUBLIC ${STANDARD_INCLUDES})
target_link_libraries(hello PRIVATE ${STANDARD_LIBS})
