cmake_minimum_required( VERSION 3.5 )

macro (find_depman Var)
   foreach(elem ${CMAKE_MODULE_PATH})
      list(APPEND new_modules_path "${elem}/../")
   endforeach()
   find_file(${Var} "eosio.depman" PATHS ${new_modules_path})
endmacro()

macro (check_dependencies dep_file)
   find_depman(DepMan)
   execute_process(COMMAND "${DepMan}" "${dep_file}" "--check"
      RESULT_VARIABLE    cmd_res
      ERROR_VARIABLE     cmd_error
      OUTPUT_VARIABLE    cmd_output)
   if (${cmd_res} STREQUAL "0")
      message(STATUS "Dependency checking passed : ${cmd_output}")
   else()
      message(WARNING "Dependency checking failed : ${cmd_output}")
   endif()
endmacro()

macro (get_dependency_prefix dep_file dep_name output)
   find_depman(DepMan)
   execute_process(COMMAND "${DepMan}" "${dep_file}" "--install-dir ${dep_name}" "--no-color"
      RESULT_VARIABLE cmd_res
      ERROR_VARIABLE  cmd_error
      OUTPUT_VARIABLE cmd_output)

   string(REGEX REPLACE "\n$" "" cmd_output "${cmd_output}")
   if (${cmd_res} STREQUAL "0")
      message(STATUS "Found dependency ${dep_name} at ${cmd_output}")
      set(${output} ${cmd_output})
   endif()
   set(${output} ${cmd_output}) 
endmacro()

macro (add_external_dependency dep_file dep_name)
   get_dependency_prefix(${dep_file} ${dep_name} DEP_DIR)
   list(INSERT CMAKE_PREFIX_PATH 0 "${DEP_DIR}")
   set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${DEP_DIR}/lib/pkgconfig")
   include_directories(BEFORE "${DEP_DIR}/include")
   link_directories("${DEP_DIR}/lib")
endmacro()
