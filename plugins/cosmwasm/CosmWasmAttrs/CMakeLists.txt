add_llvm_library(LLVMCosmWasmAttrs MODULE CosmWasmAttrs.cpp PLUGIN_TOOL clang)

if(APPLE)
   target_link_libraries(LLVMCosmWasmAttrs PUBLIC clang-cpp)
endif()
