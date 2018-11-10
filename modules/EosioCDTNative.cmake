macro (add_native_library TARGET)
   add_library( ${TARGET} ${ARGN} )
   target_compile_options( ${TARGET} PUBLIC -fnative )
endmacro()

macro (add_native_executable TARGET)
   add_executable( ${TARGET} ${ARGN} )
   target_compile_options( ${TARGET} PUBLIC -fnative )
endmacro()
