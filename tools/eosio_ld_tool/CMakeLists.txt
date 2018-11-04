set(LLVM_LINK_COMPONENTS support)

include_directories(include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-ld.cpp.in ${CMAKE_BINARY_DIR}/eosio-ld.cpp)

add_clang_executable(eosio-ld ${CMAKE_BINARY_DIR}/eosio-ld.cpp)
target_compile_options(eosio-ld PRIVATE -fexceptions)
target_include_directories(eosio-ld PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(eosio-ld PRIVATE
  clangTooling
  clangBasic
  clangASTMatchers)

install(FILES ${CMAKE_BINARY_DIR}/bin/eosio-ld DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

