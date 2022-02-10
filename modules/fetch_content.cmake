
include(FetchContent)

macro(fetch_content name)
   FetchContent_Declare(
      ${name}
      ${ARGN}
   )
   FetchContent_GetProperties(${name})
   if(NOT ${name}_POPULATED)
      FetchContent_Populate(${name})
   endif()
endmacro()

macro(add_content name)
   FetchContent_Declare(
      ${name}
      ${ARGN}
   )
   FetchContent_MakeAvailable(${name})
endmacro()