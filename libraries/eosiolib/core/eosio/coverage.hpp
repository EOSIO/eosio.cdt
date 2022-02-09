/**
 *  @file coverage.hpp
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <stdint.h>

namespace eosio {
    void coverage_inc_fun_cnt(uint64_t code, uint32_t file_num, uint32_t func_num);
    void coverage_inc_line_cnt(uint64_t code, uint32_t file_num, uint32_t line_num);
    uint32_t coverage_get_fun_cnt(uint64_t code, uint32_t file_num, uint32_t func_num);
    uint32_t coverage_get_line_cnt(uint64_t code, uint32_t file_num, uint32_t line_num);
    void coverage_dump(uint32_t n);
    void coverage_reset();
}