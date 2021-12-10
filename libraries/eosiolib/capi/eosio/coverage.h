/**
 *  @file coverage.h
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
__attribute__((eosio_wasm_import))
void coverage_inc_fun_cnt(uint64_t code, uint32_t file_num, uint32_t func_num);

__attribute__((eosio_wasm_import))
void coverage_inc_line_cnt(uint64_t code, uint32_t file_num, uint32_t line_num);

__attribute__((eosio_wasm_import))
uint32_t coverage_get_fun_cnt(uint64_t code, uint32_t file_num, uint32_t func_num);

__attribute__((eosio_wasm_import))
uint32_t coverage_get_line_cnt(uint64_t code, uint32_t file_num, uint32_t line_num);

__attribute__((eosio_wasm_import))
void coverage_dump(uint32_t n);

__attribute__((eosio_wasm_import))
void coverage_reset();

#ifdef __cplusplus
}
#endif