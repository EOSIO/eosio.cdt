/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once
#include <cstdlib>

#include <eosio/name.hpp>

namespace eosio {

   namespace internal_use_do_not_use {
       
      extern "C" {
         __attribute__((eosio_wasm_import))
        void coverage_inc_fun_cnt( capi_name code, uint32_t file_num, uint32_t func_num );

        __attribute__((eosio_wasm_import))
        void coverage_inc_line_cnt( capi_name code, uint32_t file_num, uint32_t line_num );

        __attribute__((eosio_wasm_import))
        uint32_t coverage_get_fun_cnt( capi_name code, uint32_t file_num, uint32_t func_num );

        __attribute__((eosio_wasm_import))
        uint32_t coverage_get_line_cnt( capi_name code, uint32_t file_num, uint32_t line_num );

        __attribute__((eosio_wasm_import))
        void coverage_dump();

        __attribute__((eosio_wasm_import))
        void coverage_reset();
      }
   };

   inline void coverage_inc_fun_cnt( name code, uint32_t file_num, uint32_t func_num ) {
       internal_use_do_not_use::coverage_inc_fun_cnt(static_cast<capi_name>(code), file_num, func_num);
   }

   inline void coverage_inc_line_cnt( name code, uint32_t file_num, uint32_t line_num ) {
       internal_use_do_not_use::coverage_inc_line_cnt(static_cast<capi_name>(code), file_num, line_num);
   }

   inline uint32_t coverage_get_fun_cnt( name code, uint32_t file_num, uint32_t func_num ) {
       return internal_use_do_not_use::coverage_get_fun_cnt(static_cast<capi_name>(code), file_num, func_num);
   }

   inline uint32_t coverage_get_line_cnt( name code, uint32_t file_num, uint32_t line_num ) {
       return internal_use_do_not_use::coverage_get_line_cnt(static_cast<capi_name>(code), file_num, line_num);
   }    

   inline void coverage_dump() {
       internal_use_do_not_use::coverage_dump();
   }

   inline void coverage_reset() {
       internal_use_do_not_use::coverage_reset();
   }

} // namespace eosio