add_executable(exchange exchange.cpp )
target_include_directories( exchange PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${STANDARD_INCLUDES})
target_link_libraries( exchange ${STANDARD_LIBS} )
