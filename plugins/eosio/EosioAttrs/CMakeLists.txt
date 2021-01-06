add_llvm_library(LLVMEosioAttrs MODULE EosioAttrs.cpp PLUGIN_TOOL clang)

if(APPLE)
   target_link_libraries(LLVMEosioAttrs PUBLIC clang-cpp)
endif()
