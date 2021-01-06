add_llvm_component_library(LLVMEosioApply EosioApply.cpp MODULE DEPENDS intrinsics_gen PLUGIN_TOOL opt)

if(APPLE)
   target_link_libraries(LLVMEosioApply PUBLIC clang-cpp LLVM)
endif()
