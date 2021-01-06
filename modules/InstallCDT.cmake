#macro( eosio_clang_install file )
#   set(BINARY_DIR ${CMAKE_BINARY_DIR}/eosio_llvm/bin)
#   add_custom_command( TARGET EosioClang POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
#   install(FILES ${BINARY_DIR}/${file}
#      DESTINATION ${CDT_INSTALL_PREFIX}/bin
#      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
#endmacro( eosio_clang_install )
#
#macro( eosio_clang_install_and_symlink file symlink )
#   set(BINARY_DIR ${CMAKE_BINARY_DIR}/eosio_llvm/bin)
#   add_custom_command( TARGET EosioClang POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
#   add_custom_command( TARGET EosioClang POST_BUILD COMMAND cd ${CMAKE_BINARY_DIR}/bin && ln -sf ${file} ${symlink} )
#   install(FILES ${BINARY_DIR}/${file}
#      DESTINATION ${CDT_INSTALL_PREFIX}/bin
#      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
#   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/bin)")
#   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/bin/${file} ${CMAKE_INSTALL_PREFIX}/bin/${symlink})")
#endmacro( eosio_clang_install_and_symlink )

add_custom_command( TARGET EosioPlugins POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin )
macro( eosio_plugin_install target file )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/plugins/eosio)
   add_custom_command( TARGET EosioPlugins POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${target}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/target/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro( eosio_plugin_install )

add_custom_command( TARGET EosioTools PRE_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin )
macro( eosio_tool_pre_symlink file symlink )
   if(APPLE)
      add_custom_command( TARGET EosioTools PRE_BUILD COMMAND cd ${CMAKE_BINARY_DIR}/bin && ln -sf ${BREW_LLVM_PATH}/bin/${file} ${symlink} )
   else()
      add_custom_command( TARGET EosioTools PRE_BUILD COMMAND cd ${CMAKE_BINARY_DIR}/bin && ln -sf /usr/bin/${file} ${symlink} )
   endif()
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/bin)")
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/bin/${file} ${CMAKE_INSTALL_PREFIX}/bin/${symlink})")
endmacro( eosio_tool_pre_symlink )

macro( eosio_tool_install file )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/tools/bin)
   add_custom_command( TARGET EosioTools POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro( eosio_tool_install )

macro( eosio_tool_install_and_symlink file symlink )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/tools/bin)
   add_custom_command( TARGET EosioTools POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/bin)")
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/bin/${file} ${CMAKE_INSTALL_PREFIX}/bin/${symlink})")
endmacro( eosio_tool_install_and_symlink )

macro( eosio_cmake_install_and_symlink file symlink )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/modules)
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME})")
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME}/${file} ${CMAKE_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME}/${symlink})")
endmacro( eosio_cmake_install_and_symlink )

macro( eosio_libraries_install)
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lib)
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION ${CDT_INSTALL_PREFIX}/lib)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CDT_INSTALL_PREFIX}/include)
endmacro( eosio_libraries_install )

#eosio_clang_install_and_symlink(llvm-ranlib ${EOSIO_RANLIB})
#eosio_clang_install_and_symlink(llvm-ar ${EOSIO_AR})
#eosio_clang_install_and_symlink(llvm-nm eosio-nm)
#eosio_clang_install_and_symlink(llvm-objcopy eosio-objcopy)
#eosio_clang_install_and_symlink(llvm-objdump eosio-objdump)
#eosio_clang_install_and_symlink(llvm-readobj eosio-readobj)
#eosio_clang_install_and_symlink(llvm-readelf eosio-readelf)
#eosio_clang_install_and_symlink(llvm-strip eosio-strip)

#eosio_clang_install(opt)
#eosio_clang_install(llc)
#eosio_clang_install(lld)
#eosio_clang_install(ld.lld)
#eosio_clang_install(ld64.lld)
#eosio_clang_install(clang-7)
#eosio_clang_install(wasm-ld)

eosio_tool_pre_symlink(${EOSIO_RANLIB_BACKEND} ${EOSIO_RANLIB})
eosio_tool_pre_symlink(${EOSIO_AR_BACKEND} ${EOSIO_AR})

eosio_tool_install_and_symlink(${EOSIO_POSTPASS} ${EOSIO_POSTPASS})
#eosio_tool_install_and_symlink(wat2wasm eosio-wast2wasm)
#eosio_tool_install_and_symlink(wasm2wat eosio-wasm2wast)
eosio_tool_install_and_symlink(${EOSIO_C_COMPILER} ${EOSIO_C_COMPILER})
eosio_tool_install_and_symlink(${EOSIO_CXX_COMPILER} ${EOSIO_CXX_COMPILER})
eosio_tool_install_and_symlink(${EOSIO_LINKER} ${EOSIO_LINKER})
#eosio_tool_install_and_symlink(eosio-abigen eosio-abigen)
eosio_tool_install_and_symlink(eosio-abidiff eosio-abidiff)
#eosio_tool_install_and_symlink(eosio-init eosio-init)

#eosio_clang_install(../lib/LLVMEosioApply${CMAKE_SHARED_LIBRARY_SUFFIX})
#eosio_clang_install(../lib/LLVMEosioSoftfloat${CMAKE_SHARED_LIBRARY_SUFFIX})
#eosio_clang_install(../lib/eosio_plugin${CMAKE_SHARED_LIBRARY_SUFFIX})
eosio_plugin_install(EosioApply LLVMEosioApply${CMAKE_SHARED_LIBRARY_SUFFIX})
eosio_plugin_install(EosioAttrs LLVMEosioAttrs${CMAKE_SHARED_LIBRARY_SUFFIX})
eosio_plugin_install(eosio_plugin eosio_plugin${CMAKE_SHARED_LIBRARY_SUFFIX})

eosio_cmake_install_and_symlink(${CMAKE_PROJECT_NAME}-config.cmake ${CMAKE_PROJECT_NAME}-config.cmake)
eosio_cmake_install_and_symlink(EosioWasmToolchain.cmake EosioWasmToolchain.cmake)
eosio_cmake_install_and_symlink(EosioCDTMacros.cmake EosioCDTMacros.cmake)

eosio_libraries_install()
