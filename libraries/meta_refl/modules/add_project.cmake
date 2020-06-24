# ##################################################################################################
# Function for adding a project to this master project. The first parameter is the name of the repo
#    and the rest are other projects that it depends on.
# ##################################################################################################
function(add_project)
   set(options OPTIONAL)
   set(oneValueArgs TARGET DIR)
   set(multiValueArgs ARGS DEPENDS)
   cmake_parse_arguments(ADD_PROJECT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

   get_cmake_property(vars CACHE_VARIABLES)
   foreach( var ${vars} )
      get_property(hstr CACHE "${var}" PROPERTY HELPSTRING)
      if ( "${hstr}" MATCHES "No help, variable specified on the command line." OR "${hstr}" STREQUAL "" )
         list(APPEND CL_ARGS "-D${var}=${${var}}")
      endif()
   endforeach()

   if (ADD_PROJECT_DIR)
      set(SOURCE_DIR "${ADD_PROJECT_DIR}")
   else()
      set(SOURCE_DIR "${CMAKE_SOURCE_DIR}")
   endif()

   ExternalProject_Add(
      ${ADD_PROJECT_TARGET}
      SOURCE_DIR "${SOURCE_DIR}/${ADD_PROJECT_TARGET}"
      BINARY_DIR "${CMAKE_BINARY_DIR}/${ADD_PROJECT_TARGET}"
      CMAKE_ARGS ${CL_ARGS} ${ADD_PROJECT_ARGS}
      UPDATE_COMMAND  ""
      PATCH_COMMAND   ""
      TEST_COMMAND    ""
      INSTALL_COMMAND ""
      BUILD_ALWAYS 1
      DEPENDS ${ADD_PROJECT_DEPENDS}
   )
endfunction()
