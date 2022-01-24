

add_library(eosio
            eosiolib.cpp
            crypto.cpp
            tester/fpconv.c
            ${abieos_SOURCE_DIR}/src/crypto.cpp)
target_compile_definitions(eosio PUBLIC __eosio_cdt__)
target_compile_options(eosio PRIVATE --no-missing-ricardian-clause)
target_include_directories(eosio PUBLIC $<BUILD_INTERFACE:${boost_SOURCE_DIR}>
                                        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../meta_refl/include>
                                        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
                                        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/contracts>
                                        $<BUILD_INTERFACE:${abieos_SOURCE_DIR}/include>
                                        $<BUILD_INTERFACE:${abieos_SOURCE_DIR}/external/rapidjson/include>
                                        $<INSTALL_INTERFACE:include/eosiolib/core> 
                                        $<INSTALL_INTERFACE:include/eosiolib/contracts> 
                                        $<INSTALL_INTERFACE:include/abieos>
)

add_library(eosio_malloc
            malloc.cpp)
set_target_properties(eosio_malloc PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(eosio_malloc PUBLIC __eosio_cdt__)
target_compile_options(eosio_malloc PRIVATE --no-missing-ricardian-clause)
target_include_directories(eosio_malloc PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
                                               $<BUILD_INTERFACE:${abieos_SOURCE_DIR}/include>
                                               $<BUILD_INTERFACE:${abieos_SOURCE_DIR}/external/rapidjson/include>
                                               $<INSTALL_INTERFACE:include/eosiolib/core> 
                                               $<INSTALL_INTERFACE:include/abieos>
)
            
add_library(eosio_dsm
            simple_malloc.cpp)
target_compile_definitions(eosio_dsm PUBLIC __eosio_cdt__)
target_compile_options(eosio_dsm PRIVATE --no-missing-ricardian-clause)
target_include_directories(eosio_dsm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
                                            $<BUILD_INTERFACE:${abieos_SOURCE_DIR}/include>
                                            $<BUILD_INTERFACE:${abieos_SOURCE_DIR}/external/rapidjson/include>
                                            $<INSTALL_INTERFACE:include/eosiolib/core> 
                                            $<INSTALL_INTERFACE:include/abieos> 
)
                      

add_library(eosio_cmem
            memory.cpp)
target_compile_options(eosio_cmem PRIVATE --no-missing-ricardian-clause)

add_library(eosio_tester
            tester/tester.cpp
            tester/tester_intrinsics.cpp
            ${abieos_SOURCE_DIR}/src/crypto.cpp)
target_compile_definitions(eosio_tester PUBLIC __eosio_cdt__)
target_compile_options(eosio_tester PRIVATE --no-missing-ricardian-clause)
target_include_directories(eosio_tester PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tester>
                                               $<BUILD_INTERFACE:${fmt_SOURCE_DIR}/include>
                                               $<INSTALL_INTERFACE:include/eosiolib/tester> 
)
target_link_libraries(eosio_tester PUBLIC eosio)

add_library(eosio_embed
            tester/fpconv.c
            ${abieos_SOURCE_DIR}/src/crypto.cpp
            embed/eosio/eosio_assert.cpp
            embed/eosio/printi.cpp
            embed/eosio/printn.cpp
            embed/eosio/prints.cpp
            embed/eosio/printui.cpp)
target_compile_options(eosio_embed PRIVATE --no-missing-ricardian-clause)
target_link_libraries(eosio_embed PUBLIC eosio)

add_custom_command(
   OUTPUT ${BASE_BINARY_DIR}/lib/crt0.o
   COMMAND ${CMAKE_CXX_COMPILER} --target=wasm32 -D__eosio_cdt__ -c -o ${BASE_BINARY_DIR}/lib/crt0.o 
                                 -I${boost_SOURCE_DIR}
                                 -I${CMAKE_CURRENT_SOURCE_DIR}/../meta_refl/include
                                 -I${CMAKE_CURRENT_SOURCE_DIR}/core
                                 -I${CMAKE_CURRENT_SOURCE_DIR}/contracts
                                 -I${abieos_SOURCE_DIR}/include
                                 -I${abieos_SOURCE_DIR}/external/rapidjson/include
                                 -I${CMAKE_CURRENT_SOURCE_DIR}/tester
                                 -I${fmt_SOURCE_DIR}/include
                                 --no-missing-ricardian-clause
                                 ${CMAKE_SOURCE_DIR}/eosiolib/tester/crt0.cpp
   DEPENDS ${CMAKE_SOURCE_DIR}/eosiolib/tester/crt0.cpp
   COMMENT "Building C++ object crt0.o"
   COMMAND_EXPAND_LISTS
)
add_custom_target(crt0 DEPENDS ${BASE_BINARY_DIR}/lib/crt0.o)
add_dependencies(eosio_tester crt0)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
       DESTINATION include/eosiolib 
       FILES_MATCHING FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(DIRECTORY ${abieos_SOURCE_DIR}/include 
        DESTINATION include/abieos 
        FILES_MATCHING FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(FILES ${abieos_SOURCE_DIR}/src/abieos.h 
              ${abieos_SOURCE_DIR}/src/abieos.hpp
              ${abieos_SOURCE_DIR}/src/abieos_numeric.hpp
              ${abieos_SOURCE_DIR}/src/abieos_ripemd160.hpp
              DESTINATION include/abieos)
install(FILES ${BASE_BINARY_DIR}/lib/crt0.o 
        DESTINATION lib)

install(DIRECTORY ${abieos_SOURCE_DIR}/external/rapidjson/include DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(DIRECTORY ${fmt_SOURCE_DIR}/include DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${Catch2_SOURCE_DIR}/single_include/catch2 DESTINATION ${BASE_BINARY_DIR}/include PATTERN "*.hpp")

