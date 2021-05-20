# ##################################################################################################
# Define the main test executable.
# ##################################################################################################
include(CTest)
enable_testing()
add_executable( meta_refl_unit_tests main.cpp
                                    meta_tests.cpp
                                    traits_tests.cpp
              )

target_link_libraries( meta_refl_unit_tests PRIVATE bluegrass::meta_refl Catch2::Catch2 )
catch_discover_tests( meta_refl_unit_tests )
