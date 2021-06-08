execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_ashlti3
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_ashrti3
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_lshrti3
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_divti3
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_udivti3
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_multi3
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_modti3
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_umodti3
                COMMAND_ERROR_IS_FATAL ANY
)