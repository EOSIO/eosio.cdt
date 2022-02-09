/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */

#include <stdint.h>

extern "C" {
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
}
namespace eosio {

   void coverage_inc_fun_cnt(uint64_t code, uint32_t file_num, uint32_t func_num) {
       ::coverage_inc_fun_cnt(code, file_num, func_num);
   }

   void coverage_inc_line_cnt(uint64_t code, uint32_t file_num, uint32_t line_num) {
       ::coverage_inc_line_cnt(code, file_num, line_num);
   }

   uint32_t coverage_get_fun_cnt(uint64_t code, uint32_t file_num, uint32_t func_num) {
       return ::coverage_get_fun_cnt(code, file_num, func_num);
   }

   uint32_t coverage_get_line_cnt(uint64_t code, uint32_t file_num, uint32_t line_num) {
       return ::coverage_get_line_cnt(code, file_num, line_num);
   }

   void coverage_dump(uint32_t n) {
       ::coverage_dump(n);
   }

   void coverage_reset() {
       ::coverage_reset();
   }
}