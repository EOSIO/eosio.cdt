set(LLVM_LINK_COMPONENTS support)

include_directories(include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-cc.cpp.in ${CMAKE_BINARY_DIR}/eosio-cc.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-cpp.cpp.in ${CMAKE_BINARY_DIR}/eosio-cpp.cpp)

add_executable(eosio-cc ${CMAKE_BINARY_DIR}/eosio-cc.cpp)
target_compile_options(eosio-cc PRIVATE -fexceptions -fno-rtti)
target_include_directories(eosio-cc PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_SOURCE_DIR}/eosio_llvm/tools/clang/include ${LLVM_INCLUDE_DIR})

target_link_libraries(eosio-cc
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

add_executable(eosio-cpp ${CMAKE_BINARY_DIR}/eosio-cpp.cpp)
target_compile_options(eosio-cpp PRIVATE -fexceptions -fno-rtti)
target_include_directories(eosio-cpp PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_SOURCE_DIR}/eosio_llvm/tools/clang/include ${LLVM_INCLUDE_DIR})

target_link_libraries(eosio-cpp
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
install(FILES ${CMAKE_BINARY_DIR}/bin/eosio-cpp DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES ${CMAKE_BINARY_DIR}/bin/eosio-cc DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
