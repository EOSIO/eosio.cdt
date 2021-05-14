add_custom_command(TARGET BlancPlugins POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin)
macro(blanc_plugin_install profile target file)
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/plugins/${profile})
   add_custom_command(TARGET BlancPlugins POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${target}/${file} ${CMAKE_BINARY_DIR}/bin/)
   install(FILES ${BINARY_DIR}/${target}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro(blanc_plugin_install)

add_custom_command(TARGET BlancTools PRE_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin)
macro(eosio_tool_pre_symlink files symlink)
  foreach(file ${files})
    if(APPLE)
      set(FILEPATH ${BREW_LLVM_PATH}/bin/${file})
    else()
      set(FILEPATH /usr/bin/${file})
    endif()
    if(NOT EXISTS ${FILEPATH})
      continue()
    endif()
    add_custom_command(TARGET BlancTools PRE_BUILD COMMAND cd ${CMAKE_BINARY_DIR}/bin && ln -sf ${FILEPATH} ${symlink})
    install(FILES ${CMAKE_BINARY_DIR}/bin/${symlink} DESTINATION ${CDT_INSTALL_PREFIX}/bin)
    install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/bin)")
    install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/bin/${file} ${CMAKE_INSTALL_PREFIX}/bin/${symlink})")
    break()
  endforeach()
endmacro(eosio_tool_pre_symlink)

macro( eosio_tool_install file )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/tools/bin)
   add_custom_command( TARGET BlancTools POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro( eosio_tool_install )

macro( eosio_tool_install_and_symlink file symlink )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/tools/bin)
   add_custom_command( TARGET BlancTools POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/bin)")
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/bin/${file} ${CMAKE_INSTALL_PREFIX}/bin/${symlink})")
endmacro( eosio_tool_install_and_symlink )

macro( eosio_cmake_install_and_symlink file symlink )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/modules)
   install(FILES ${BINARY_DIR}/${file} DESTINATION ${CDT_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME})
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME})")
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME}/${file} ${CMAKE_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME}/${symlink})")
endmacro( eosio_cmake_install_and_symlink )

macro( eosio_libraries_install )
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lib)
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION ${CDT_INSTALL_PREFIX}/lib PATTERN "${CMAKE_BINARY_DIR}/lib/cmake" EXCLUDE)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CDT_INSTALL_PREFIX}/include)
endmacro( eosio_libraries_install )

eosio_tool_pre_symlink("${BLANC_RANLIB_BACKEND_PREFERRED};${BLANC_RANLIB_BACKEND}" ${BLANC_RANLIB})
eosio_tool_pre_symlink("${BLANC_AR_BACKEND_PREFERRED};${BLANC_AR_BACKEND}" ${BLANC_AR})

eosio_tool_install_and_symlink(${BLANC_POSTPASS} ${BLANC_POSTPASS})
eosio_tool_install_and_symlink(${BLANC_C_COMPILER} ${BLANC_C_COMPILER})
eosio_tool_install_and_symlink(${BLANC_CXX_COMPILER} ${BLANC_CXX_COMPILER})
eosio_tool_install_and_symlink(${BLANC_LINKER} ${BLANC_LINKER})
#eosio_tool_install_and_symlink(eosio-abidiff eosio-abidiff)

blanc_plugin_install(eosio EosioAttrs LLVMEosioAttrs${CMAKE_SHARED_LIBRARY_SUFFIX})
blanc_plugin_install(eosio eosio_plugin eosio_plugin${CMAKE_SHARED_LIBRARY_SUFFIX})
blanc_plugin_install(cosmwasm CosmWasmAttrs LLVMCosmWasmAttrs${CMAKE_SHARED_LIBRARY_SUFFIX})

eosio_cmake_install_and_symlink(${CMAKE_PROJECT_NAME}-config.cmake ${CMAKE_PROJECT_NAME}-config.cmake)
eosio_cmake_install_and_symlink(EosioWasmToolchain.cmake EosioWasmToolchain.cmake)
eosio_cmake_install_and_symlink(EosioCDTMacros.cmake EosioCDTMacros.cmake)

eosio_libraries_install()
