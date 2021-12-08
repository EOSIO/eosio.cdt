/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Increments the call count of a function in specified code/file
 *
 *  @brief 
 *  @param name - 
 */
__attribute__((eosio_wasm_import))
void coverage_inc_fun_cnt( capi_name code, uint32_t file_num, uint32_t func_num );

__attribute__((eosio_wasm_import))
void coverage_inc_line_cnt( capi_name code, uint32_t file_num, uint32_t line_num );

__attribute__((eosio_wasm_import))
void coverage_dump();

#ifdef __cplusplus
}
#endif