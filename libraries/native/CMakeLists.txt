project(native LANGUAGES CXX ASM)

if (NOT __APPLE)
   set(CRT_ASM elf_crt.s)
else()
   set(CRT_ASM macho_crt.s)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything -allow-sse")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything -allow-sse")

file(GLOB HEADERS "*.hpp"
                  "*.h")

# Softfloat
# generate compile command database for external tools
set (CMAKE_EXPORT_COMPILE_COMMANDS "ON")

message ( STATUS "Configuring SoftFloat" )
set( C_DEFINES "-DSOFTFLOAT_FAST_INT64 -DSOFTFLOAT_ROUND_EVEN -DINLINE_LEVEL=5 -DSOFTFLOAT_FAST_DIV32TO16 -DSOFTFLOAT_FAST_DIV64TO32" )
set( CMAKE_C_FLAGS " -Wall -Werror-implicit-function-declaration -Wno-conversion ${CMAKE_C_FLAGS} ${C_DEFINES}" )
set ( softfloat_sources
  softfloat/source/s_eq128.c
  softfloat/source/s_le128.c
  softfloat/source/s_lt128.c
  softfloat/source/s_shortShiftLeft128.c
  softfloat/source/s_shortShiftRight128.c
  softfloat/source/s_shortShiftRightJam64.c
  softfloat/source/s_shortShiftRightJam64Extra.c
  softfloat/source/s_shortShiftRightJam128.c
  softfloat/source/s_shortShiftRightJam128Extra.c
  softfloat/source/s_shiftRightJam32.c
  softfloat/source/s_shiftRightJam64.c
  softfloat/source/s_shiftRightJam64Extra.c
  softfloat/source/s_shiftRightJam128.c
  softfloat/source/s_shiftRightJam128Extra.c
  softfloat/source/s_shiftRightJam256M.c
  softfloat/source/s_countLeadingZeros8.c
  softfloat/source/s_countLeadingZeros16.c
  softfloat/source/s_countLeadingZeros32.c
  softfloat/source/s_countLeadingZeros64.c
  softfloat/source/s_add128.c
  softfloat/source/s_add256M.c
  softfloat/source/s_sub128.c
  softfloat/source/s_sub256M.c
  softfloat/source/s_mul64ByShifted32To128.c
  softfloat/source/s_mul64To128.c
  softfloat/source/s_mul128By32.c
  softfloat/source/s_mul128To256M.c
  softfloat/source/s_approxRecip_1Ks.c
  softfloat/source/s_approxRecip32_1.c
  softfloat/source/s_approxRecipSqrt_1Ks.c
  softfloat/source/s_approxRecipSqrt32_1.c
  softfloat/source/8086-SSE/softfloat_raiseFlags.c
  softfloat/source/8086-SSE/s_f16UIToCommonNaN.c
  softfloat/source/8086-SSE/s_commonNaNToF16UI.c
  softfloat/source/8086-SSE/s_propagateNaNF16UI.c
  softfloat/source/8086-SSE/s_f32UIToCommonNaN.c
  softfloat/source/8086-SSE/s_commonNaNToF32UI.c
  softfloat/source/8086-SSE/s_propagateNaNF32UI.c
  softfloat/source/8086-SSE/s_f64UIToCommonNaN.c
  softfloat/source/8086-SSE/s_commonNaNToF64UI.c
  softfloat/source/8086-SSE/s_propagateNaNF64UI.c
  softfloat/source/8086-SSE/extF80M_isSignalingNaN.c
  softfloat/source/8086-SSE/s_extF80UIToCommonNaN.c
  softfloat/source/8086-SSE/s_commonNaNToExtF80UI.c
  softfloat/source/8086-SSE/s_propagateNaNExtF80UI.c
  softfloat/source/8086-SSE/f128M_isSignalingNaN.c
  softfloat/source/8086-SSE/s_f128UIToCommonNaN.c
  softfloat/source/8086-SSE/s_commonNaNToF128UI.c
  softfloat/source/8086-SSE/s_propagateNaNF128UI.c
  softfloat/source/s_roundToUI32.c
  softfloat/source/s_roundToUI64.c
  softfloat/source/s_roundToI32.c
  softfloat/source/s_roundToI64.c
  softfloat/source/s_normSubnormalF16Sig.c
  softfloat/source/s_roundPackToF16.c
  softfloat/source/s_normRoundPackToF16.c
  softfloat/source/s_addMagsF16.c
  softfloat/source/s_subMagsF16.c
  softfloat/source/s_mulAddF16.c
  softfloat/source/s_normSubnormalF32Sig.c
  softfloat/source/s_roundPackToF32.c
  softfloat/source/s_normRoundPackToF32.c
  softfloat/source/s_addMagsF32.c
  softfloat/source/s_subMagsF32.c
  softfloat/source/s_mulAddF32.c
  softfloat/source/s_normSubnormalF64Sig.c
  softfloat/source/s_roundPackToF64.c
  softfloat/source/s_normRoundPackToF64.c
  softfloat/source/s_addMagsF64.c
  softfloat/source/s_subMagsF64.c
  softfloat/source/s_mulAddF64.c
  softfloat/source/s_normSubnormalExtF80Sig.c
  softfloat/source/s_roundPackToExtF80.c
  softfloat/source/s_normRoundPackToExtF80.c
  softfloat/source/s_addMagsExtF80.c
  softfloat/source/s_subMagsExtF80.c
  softfloat/source/s_normSubnormalF128Sig.c
  softfloat/source/s_roundPackToF128.c
  softfloat/source/s_normRoundPackToF128.c
  softfloat/source/s_addMagsF128.c
  softfloat/source/s_subMagsF128.c
  softfloat/source/s_mulAddF128.c
  softfloat/source/softfloat_state.c
  softfloat/source/ui32_to_f16.c
  softfloat/source/ui32_to_f32.c
  softfloat/source/ui32_to_f64.c
  softfloat/source/ui32_to_extF80.c
  softfloat/source/ui32_to_extF80M.c
  softfloat/source/ui32_to_f128.c
  softfloat/source/ui32_to_f128M.c
  softfloat/source/ui64_to_f16.c
  softfloat/source/ui64_to_f32.c
  softfloat/source/ui64_to_f64.c
  softfloat/source/ui64_to_extF80.c
  softfloat/source/ui64_to_extF80M.c
  softfloat/source/ui64_to_f128.c
  softfloat/source/ui64_to_f128M.c
  softfloat/source/i32_to_f16.c
  softfloat/source/i32_to_f32.c
  softfloat/source/i32_to_f64.c
  softfloat/source/i32_to_extF80.c
  softfloat/source/i32_to_extF80M.c
  softfloat/source/i32_to_f128.c
  softfloat/source/i32_to_f128M.c
  softfloat/source/i64_to_f16.c
  softfloat/source/i64_to_f32.c
  softfloat/source/i64_to_f64.c
  softfloat/source/i64_to_extF80.c
  softfloat/source/i64_to_extF80M.c
  softfloat/source/i64_to_f128.c
  softfloat/source/i64_to_f128M.c
  softfloat/source/f16_to_ui32.c
  softfloat/source/f16_to_ui64.c
  softfloat/source/f16_to_i32.c
  softfloat/source/f16_to_i64.c
  softfloat/source/f16_to_ui32_r_minMag.c
  softfloat/source/f16_to_ui64_r_minMag.c
  softfloat/source/f16_to_i32_r_minMag.c
  softfloat/source/f16_to_i64_r_minMag.c
  softfloat/source/f16_to_f32.c
  softfloat/source/f16_to_f64.c
  softfloat/source/f16_to_extF80.c
  softfloat/source/f16_to_extF80M.c
  softfloat/source/f16_to_f128.c
  softfloat/source/f16_to_f128M.c
  softfloat/source/f16_roundToInt.c
  softfloat/source/f16_add.c
  softfloat/source/f16_sub.c
  softfloat/source/f16_mul.c
  softfloat/source/f16_mulAdd.c
  softfloat/source/f16_div.c
  softfloat/source/f16_rem.c
  softfloat/source/f16_sqrt.c
  softfloat/source/f16_eq.c
  softfloat/source/f16_le.c
  softfloat/source/f16_lt.c
  softfloat/source/f16_eq_signaling.c
  softfloat/source/f16_le_quiet.c
  softfloat/source/f16_lt_quiet.c
  softfloat/source/f16_isSignalingNaN.c
  softfloat/source/f32_to_ui32.c
  softfloat/source/f32_to_ui64.c
  softfloat/source/f32_to_i32.c
  softfloat/source/f32_to_i64.c
  softfloat/source/f32_to_ui32_r_minMag.c
  softfloat/source/f32_to_ui64_r_minMag.c
  softfloat/source/f32_to_i32_r_minMag.c
  softfloat/source/f32_to_i64_r_minMag.c
  softfloat/source/f32_to_f16.c
  softfloat/source/f32_to_f64.c
  softfloat/source/f32_to_extF80.c
  softfloat/source/f32_to_extF80M.c
  softfloat/source/f32_to_f128.c
  softfloat/source/f32_to_f128M.c
  softfloat/source/f32_roundToInt.c
  softfloat/source/f32_add.c
  softfloat/source/f32_sub.c
  softfloat/source/f32_mul.c
  softfloat/source/f32_mulAdd.c
  softfloat/source/f32_div.c
  softfloat/source/f32_rem.c
  softfloat/source/f32_sqrt.c
  softfloat/source/f32_eq.c
  softfloat/source/f32_le.c
  softfloat/source/f32_lt.c
  softfloat/source/f32_eq_signaling.c
  softfloat/source/f32_le_quiet.c
  softfloat/source/f32_lt_quiet.c
  softfloat/source/f32_isSignalingNaN.c
  softfloat/source/f64_to_ui32.c
  softfloat/source/f64_to_ui64.c
  softfloat/source/f64_to_i32.c
  softfloat/source/f64_to_i64.c
  softfloat/source/f64_to_ui32_r_minMag.c
  softfloat/source/f64_to_ui64_r_minMag.c
  softfloat/source/f64_to_i32_r_minMag.c
  softfloat/source/f64_to_i64_r_minMag.c
  softfloat/source/f64_to_f16.c
  softfloat/source/f64_to_f32.c
  softfloat/source/f64_to_extF80.c
  softfloat/source/f64_to_extF80M.c
  softfloat/source/f64_to_f128.c
  softfloat/source/f64_to_f128M.c
  softfloat/source/f64_roundToInt.c
  softfloat/source/f64_add.c
  softfloat/source/f64_sub.c
  softfloat/source/f64_mul.c
  softfloat/source/f64_mulAdd.c
  softfloat/source/f64_div.c
  softfloat/source/f64_rem.c
  softfloat/source/f64_sqrt.c
  softfloat/source/f64_eq.c
  softfloat/source/f64_le.c
  softfloat/source/f64_lt.c
  softfloat/source/f64_eq_signaling.c
  softfloat/source/f64_le_quiet.c
  softfloat/source/f64_lt_quiet.c
  softfloat/source/f64_isSignalingNaN.c
  softfloat/source/extF80_to_ui32.c
  softfloat/source/extF80_to_ui64.c
  softfloat/source/extF80_to_i32.c
  softfloat/source/extF80_to_i64.c
  softfloat/source/extF80_to_ui32_r_minMag.c
  softfloat/source/extF80_to_ui64_r_minMag.c
  softfloat/source/extF80_to_i32_r_minMag.c
  softfloat/source/extF80_to_i64_r_minMag.c
  softfloat/source/extF80_to_f16.c
  softfloat/source/extF80_to_f32.c
  softfloat/source/extF80_to_f64.c
  softfloat/source/extF80_to_f128.c
  softfloat/source/extF80_roundToInt.c
  softfloat/source/extF80_add.c
  softfloat/source/extF80_sub.c
  softfloat/source/extF80_mul.c
  softfloat/source/extF80_div.c
  softfloat/source/extF80_rem.c
  softfloat/source/extF80_sqrt.c
  softfloat/source/extF80_eq.c
  softfloat/source/extF80_le.c
  softfloat/source/extF80_lt.c
  softfloat/source/extF80_eq_signaling.c
  softfloat/source/extF80_le_quiet.c
  softfloat/source/extF80_lt_quiet.c
  softfloat/source/extF80_isSignalingNaN.c
  softfloat/source/extF80M_to_ui32.c
  softfloat/source/extF80M_to_ui64.c
  softfloat/source/extF80M_to_i32.c
  softfloat/source/extF80M_to_i64.c
  softfloat/source/extF80M_to_ui32_r_minMag.c
  softfloat/source/extF80M_to_ui64_r_minMag.c
  softfloat/source/extF80M_to_i32_r_minMag.c
  softfloat/source/extF80M_to_i64_r_minMag.c
  softfloat/source/extF80M_to_f16.c
  softfloat/source/extF80M_to_f32.c
  softfloat/source/extF80M_to_f64.c
  softfloat/source/extF80M_to_f128M.c
  softfloat/source/extF80M_roundToInt.c
  softfloat/source/extF80M_add.c
  softfloat/source/extF80M_sub.c
  softfloat/source/extF80M_mul.c
  softfloat/source/extF80M_div.c
  softfloat/source/extF80M_rem.c
  softfloat/source/extF80M_sqrt.c
  softfloat/source/extF80M_eq.c
  softfloat/source/extF80M_le.c
  softfloat/source/extF80M_lt.c
  softfloat/source/extF80M_eq_signaling.c
  softfloat/source/extF80M_le_quiet.c
  softfloat/source/extF80M_lt_quiet.c
  softfloat/source/f128_to_ui32.c
  softfloat/source/f128_to_ui64.c
  softfloat/source/f128_to_i32.c
  softfloat/source/f128_to_i64.c
  softfloat/source/f128_to_ui32_r_minMag.c
  softfloat/source/f128_to_ui64_r_minMag.c
  softfloat/source/f128_to_i32_r_minMag.c
  softfloat/source/f128_to_i64_r_minMag.c
  softfloat/source/f128_to_f16.c
  softfloat/source/f128_to_f32.c
  softfloat/source/f128_to_extF80.c
  softfloat/source/f128_to_f64.c
  softfloat/source/f128_roundToInt.c
  softfloat/source/f128_add.c
  softfloat/source/f128_sub.c
  softfloat/source/f128_mul.c
  softfloat/source/f128_mulAdd.c
  softfloat/source/f128_div.c
  softfloat/source/f128_rem.c
  softfloat/source/f128_sqrt.c
  softfloat/source/f128_eq.c
  softfloat/source/f128_le.c
  softfloat/source/f128_lt.c
  softfloat/source/f128_eq_signaling.c
  softfloat/source/f128_le_quiet.c
  softfloat/source/f128_lt_quiet.c
  softfloat/source/f128_isSignalingNaN.c
  softfloat/source/f128M_to_ui32.c
  softfloat/source/f128M_to_ui64.c
  softfloat/source/f128M_to_i32.c
  softfloat/source/f128M_to_i64.c
  softfloat/source/f128M_to_ui32_r_minMag.c
  softfloat/source/f128M_to_ui64_r_minMag.c
  softfloat/source/f128M_to_i32_r_minMag.c
  softfloat/source/f128M_to_i64_r_minMag.c
  softfloat/source/f128M_to_f16.c
  softfloat/source/f128M_to_f32.c
  softfloat/source/f128M_to_extF80M.c
  softfloat/source/f128M_to_f64.c
  softfloat/source/f128M_roundToInt.c
  softfloat/source/f128M_add.c
  softfloat/source/f128M_sub.c
  softfloat/source/f128M_mul.c
  softfloat/source/f128M_mulAdd.c
  softfloat/source/f128M_div.c
  softfloat/source/f128M_rem.c
  softfloat/source/f128M_sqrt.c
  softfloat/source/f128M_eq.c
  softfloat/source/f128M_le.c
  softfloat/source/f128M_lt.c
  softfloat/source/f128M_eq_signaling.c
  softfloat/source/f128M_le_quiet.c
  softfloat/source/f128M_lt_quiet.c
)

file ( GLOB softfloat_headers "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/source/include/*.h"
                              "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/build/Linux-x86_64-GCC/platform.h"
                              "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/source/8086-SSE/specialize.h" )
list( APPEND softfloat_sources ${softfloat_headers} )

add_library ( sf STATIC ${softfloat_sources} )
target_include_directories( sf PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/source/include" "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/source/8086-SSE" "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/build/Linux-x86_64-GCC" ${CMAKE_SOURCE_DIR})

add_native_library ( native STATIC ${softfloat_sources} intrinsics.cpp crt.cpp ${CRT_ASM} )
target_include_directories( native PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/source/include" "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/source/8086-SSE" "${CMAKE_CURRENT_SOURCE_DIR}/softfloat/build/Linux-x86_64-GCC" ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/eosiolib/capi ${CMAKE_SOURCE_DIR}/eosiolib/contracts ${CMAKE_SOURCE_DIR}/eosiolib/core)

add_dependencies(native native_eosio)

add_custom_command( TARGET native POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native> ${BASE_BINARY_DIR}/lib )

add_custom_command( TARGET sf POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sf> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${BASE_BINARY_DIR}/include/eosio FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "softfloat" EXCLUDE)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/native DESTINATION ${BASE_BINARY_DIR}/include/eosiolib FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "softfloat" EXCLUDE)
