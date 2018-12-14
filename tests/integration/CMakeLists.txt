cmake_minimum_required( VERSION 3.5 )

set(EOSIO_VERSION_MIN "1.4")
set(EOSIO_VERSION_SOFT_MAX "1.4")
#set(EOSIO_VERSION_HARD_MAX "")

find_package(eosio)

### Check the version of eosio
set(VERSION_MATCH_ERROR_MSG "")
EOSIO_CHECK_VERSION(VERSION_OUTPUT "${EOSIO_VERSION}"
                                   "${EOSIO_VERSION_MIN}"
                                   "${EOSIO_VERSION_SOFT_MAX}"
                                   "${EOSIO_VERSION_HARD_MAX}"
                                   VERSION_MATCH_ERROR_MSG)
if(VERSION_OUTPUT STREQUAL "MATCH")
   message(STATUS "Using eosio version ${EOSIO_VERSION}")
elseif(VERSION_OUTPUT STREQUAL "WARN")
   message(WARNING "Using eosio version ${EOSIO_VERSION} even though it exceeds the maximum supported version of ${EOSIO_VERSION_SOFT_MAX}; continuing with configuration, however build may fail.\nIt is recommended to use eosio version ${EOSIO_VERSION_SOFT_MAX}.x")
else() # INVALID OR MISMATCH
   message(FATAL_ERROR "Found eosio version ${EOSIO_VERSION} but it does not satisfy version requirements: ${VERSION_MATCH_ERROR_MSG}\nPlease use eosio version ${EOSIO_VERSION_SOFT_MAX}.x")
endif(VERSION_OUTPUT STREQUAL "MATCH")


enable_testing()

configure_file(${CMAKE_SOURCE_DIR}/contracts.hpp.in ${CMAKE_BINARY_DIR}/contracts.hpp)

include_directories(${CMAKE_BINARY_DIR})

file(GLOB INT_TESTS "*.cpp" "*.hpp")

add_eosio_test( integration_tests ${INT_TESTS} )
