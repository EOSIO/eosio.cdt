set(LLVM_LINK_COMPONENTS support)

include_directories(include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-abigen.cpp.in ${CMAKE_BINARY_DIR}/eosio-abigen.cpp)
if(APPLE)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdollar-in-identifier-extension")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

add_executable(eosio-abigen ${CMAKE_BINARY_DIR}/eosio-abigen.cpp)
target_compile_options(eosio-abigen PRIVATE -fexceptions -fno-rtti)
target_include_directories(eosio-abigen PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/../jsoncons/include ${CMAKE_SOURCE_DIR}/eosio_llvm/tools/clang/include ${LLVM_INCLUDE_DIR})
add_dependencies(eosio-abigen EosioClang)
target_link_libraries(eosio-abigen
  clangTooling
  clangBasic
  clangASTMatchers
  clangRewrite
  clangToolingCore
  clangFrontend
  clangDriver
  clangSerialization
  clangParse
  clangSema
  clangAnalysis
  clangAST
  clangBasic
  clangEdit
  clangLex

  LLVMipo
  LLVMScalarOpts
  LLVMInstCombine
  LLVMTransformUtils
  LLVMAnalysis
  LLVMTarget
  LLVMOption
  LLVMMCParser
  LLVMMC
  LLVMObject
  LLVMBitReader
  LLVMCore
  LLVMSupport
  LLVMDemangle
)

add_custom_command( TARGET eosio-abigen POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin )
add_custom_command( TARGET eosio-abigen POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio-abigen> ${CMAKE_BINARY_DIR}/bin/ )

install(FILES ${CMAKE_BINARY_DIR}/bin/eosio-abigen DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
