cmake_minimum_required (VERSION 3.10)
project(eosio-tester VERSION 0.1 LANGUAGES CXX)
cmake_policy(SET CMP0077 NEW)

set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
      message(FATAL_ERROR "GCC version must be at least 8.0.")
   endif()
   if("${CMAKE_GENERATOR}" STREQUAL "Ninja")
      add_compile_options(-fdiagnostics-color=always)
   endif()
endif()

if (UNIX AND APPLE)
   list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/llvm@4" "/usr/local/opt/gettext")
   add_compile_options(-fvisibility=hidden)
endif()

if ("${OPENSSL_ROOT_DIR}" STREQUAL "")
   if (NOT "$ENV{OPENSSL_ROOT_DIR}" STREQUAL "")
      set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR})
      set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
   elseif (APPLE)
      set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
      set(OPENSSL_INCLUDE_DIR "/usr/local/opt/openssl/include")
   elseif(UNIX AND NOT APPLE)
      set(OPENSSL_ROOT_DIR "/usr/include/openssl")
      set(OPENSSL_INCLUDE_DIR "/usr/include/openssl/include")
   else()
      message(FATAL_ERROR "openssl not found and don't know where to look, please specify OPENSSL_ROOT_DIR")
   endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)

set(Boost_USE_STATIC_LIBS ON)
add_definitions(-DBOOST_ASIO_DISABLE_STD_EXPERIMENTAL_STRING_VIEW)
find_package(Boost 1.70 REQUIRED COMPONENTS date_time filesystem chrono system iostreams program_options)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/eos/libraries/fc/CMakeModules")
include(SetupTargetMacros)
include(GNUInstallDirs)
include(VersionMacros)

add_subdirectory(../../libraries/eos-vm eos-vm EXCLUDE_FROM_ALL)

set(EOSIO_ROOT_KEY "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV")
set(EOSIO_WASM_OLD_BEHAVIOR "Off")
add_subdirectory(../../libraries/eos/libraries/builtins builtins EXCLUDE_FROM_ALL)
add_subdirectory(../../libraries/eos/libraries/chain chain EXCLUDE_FROM_ALL)
add_subdirectory(../../libraries/eos/libraries/chainbase chainbase EXCLUDE_FROM_ALL)
add_subdirectory(../../libraries/eos/libraries/fc fc EXCLUDE_FROM_ALL)
add_subdirectory(../../libraries/eos/libraries/wabt wabt EXCLUDE_FROM_ALL)
add_subdirectory(../../libraries/eos/libraries/wasm-jit wasm-jit EXCLUDE_FROM_ALL)

target_include_directories(eosio_chain PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/eos/libraries/wabt"
    "${CMAKE_CURRENT_SOURCE_DIR}/builtins"
    "${CMAKE_CURRENT_BINARY_DIR}/wabt"
)

add_executable(eosio-tester src/eosio-tester.cpp)
target_include_directories(eosio-tester
    PRIVATE
        ../../libraries/abieos/src
        ../../libraries/abieos/external/date/include
        ../../libraries/abieos/external/rapidjson/include
        ../../libraries/eos/libraries/chain/include
        ../../libraries/eos/libraries/fc/include
        ../../libraries/eos/libraries/wasm-jit/Include
        ../../libraries/eos/libraries/wabt/src
        ${Boost_INCLUDE_DIR}
        ${JS_INCLUDE_DIRS}
        ${ROCKSDB_INCLUDE_DIR}
)
target_link_libraries(eosio-tester
    eosio_chain wabt Runtime Platform Logging IR fc eos-vm
    Boost::date_time Boost::filesystem Boost::chrono Boost::system Boost::iostreams Boost::program_options
    -lpthread)
