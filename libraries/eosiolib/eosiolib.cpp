#include "core/eosio/datastream.hpp"
#include "core/eosio/powers.hpp"
#include "contracts/eosio/system.hpp"
#include "contracts/eosio/privileged.hpp"

#include <algorithm>

extern "C" volatile uint64_t eosio_contract_name = 0;
extern "C" volatile void eosio_set_contract_name(uint64_t n) { eosio_contract_name = n; } // LLVM creates the call to this at the beginning of apply

namespace eosio {
   extern "C" {
      __attribute__((eosio_wasm_import))
      uint64_t current_time();
     __attribute__((eosio_wasm_import))
     void set_blockchain_parameters_packed(char*, uint32_t);
     __attribute__((eosio_wasm_import))
     uint32_t get_blockchain_parameters_packed(char*, uint32_t);
     __attribute__((eosio_wasm_import))
     void set_parameters_packed(const char*, uint32_t);
     __attribute__((eosio_wasm_import))
     uint32_t get_parameters_packed(const char*, uint32_t, char*, uint32_t);
     __attribute__((eosio_wasm_import))
     int64_t set_proposed_producers( char *producer_data, uint32_t producer_data_size );
     __attribute__((eosio_wasm_import))
     uint32_t get_active_producers(uint64_t*, uint32_t);
   }

   // producer_schedule.hpp
   bool block_signing_authority_v0::is_valid()const {
      uint32_t sum_weights = 0;
      std::set<eosio::public_key> unique_keys;

      for (const auto& kw: keys ) {
         if( std::numeric_limits<uint32_t>::max() - sum_weights <= kw.weight ) {
            sum_weights = std::numeric_limits<uint32_t>::max();
         } else {
            sum_weights += kw.weight;
         }

         unique_keys.insert(kw.key);
      }

      if( keys.size() != unique_keys.size() )
         return false; // producer authority includes a duplicated key

      if( threshold == 0 )
         return false; // producer authority has a threshold of 0

      if( sum_weights < threshold )
         return false; // producer authority is unsatisfiable

      return true;
   }

   // privileged.hpp
   void set_blockchain_parameters(const eosio::blockchain_parameters& params) {
      char buf[sizeof(eosio::blockchain_parameters)];
      eosio::datastream<char *> ds( buf, sizeof(buf) );
      ds << params;
      set_blockchain_parameters_packed( buf, ds.tellp() );
   }

   void get_blockchain_parameters(eosio::blockchain_parameters& params) {
      char buf[sizeof(eosio::blockchain_parameters)];
      size_t size = get_blockchain_parameters_packed( buf, sizeof(buf) );
      eosio::check( size <= sizeof(buf), "buffer is too small" );
      eosio::datastream<const char*> ds( buf, size_t(size) );
      ds >> params;
   }

   //  considering of runtime efficiency, using a estimated size to save the exact size calculating time
   const int estimate_param_ids_buff_size = 128;
   const int estimate_params_buff_size = 256;
   void set_parameters(const id_param_pairs_type & params) {
      char buff[estimate_params_buff_size];
      unsigned_int size = params.size();
      if(params.size() == 0) return;
      eosio::datastream<char *> ds( buff, sizeof(buff) );
      ds << size;
      for(const auto & p : params ){
         unsigned_int id = p.first;
         ds << id;
         std::variant<uint16_t, uint32_t, uint64_t> var = p.second;
         if(std::holds_alternative<uint16_t>(var)){
            uint16_t data = std::get<uint16_t>(var);
            ds << data;
         } else if (std::holds_alternative<uint32_t>(var) ){
            uint32_t data = std::get<uint32_t>(var);
            ds << data;
         } else {
            uint64_t data = std::get<uint64_t>(var);
            ds << data;
         }
      }
      eosio::check( ds.tellp() <= sizeof(buff), "buffer is too small" );
      set_parameters_packed( buff, ds.tellp() );
   }

   void get_parameters(const std::vector<uint32_t> & param_ids,  id_param_pairs_type & params) {
      char buff_ids[estimate_param_ids_buff_size];
      char buff_params[estimate_params_buff_size];
      unsigned_int id_size = param_ids.size();
      params.clear();
      if(param_ids.size() == 0) return;
      eosio::datastream<char*> id_ds( buff_ids, sizeof(buff_ids) );
      id_ds << id_size;
      for(const auto & id : param_ids){
         unsigned_int _id = id;
         id_ds << _id;
      }
      eosio::check( id_ds.tellp() <= sizeof(buff_ids), "ids buffer is too small" );
      uint32_t size = get_parameters_packed( buff_ids,  id_ds.tellp(), buff_params, sizeof(buff_params));
      eosio::check( size <= sizeof(buff_params), "params buffer is too small" );
      eosio::datastream<const char*> para_ds( buff_params, size );
      unsigned_int para_size;
      para_ds >> para_size;
      eosio::check(id_size == para_size, "pairs returned doesn't match amount of id to query");
      for(uint32_t i = 0; i < para_size; ++i){
         unsigned_int _id;
         std::variant<uint16_t, uint32_t, uint64_t>  var;
         para_ds >> _id;
         uint32_t id = _id;
         if(id == max_block_net_usage_id){
            uint64_t data;
            para_ds >> data;
            var = data;
         } else if(id == max_inline_action_depth_id || id == max_authority_depth_id){
            uint16_t data;
            para_ds >> data;
            var = data;
         } else {
            uint32_t data;
            para_ds >> data;
            var = data;
         }
         params.push_back({id, var});
      }
   }

   std::optional<uint64_t> set_proposed_producers( const std::vector<producer_key>& prods ) {
      auto packed_prods = eosio::pack( prods );
      int64_t ret = set_proposed_producers((char*)packed_prods.data(), packed_prods.size());
      if (ret >= 0)
        return static_cast<uint64_t>(ret);
      return {};
   }

   // system.hpp
   time_point current_time_point() {
      static auto ct = time_point(microseconds(static_cast<int64_t>(current_time())));
      return ct;
   }

   block_timestamp current_block_time() {
      static auto bt = block_timestamp(current_time_point());
      return bt;
   }

   std::vector<name> get_active_producers() {
      const auto buffer_size = get_active_producers(nullptr, 0);
      const auto prod_cnt = buffer_size / sizeof(name);
      std::vector<name> active_prods(prod_cnt);
      get_active_producers((uint64_t*)active_prods.data(), buffer_size);
      return active_prods;
   }

   // powers.hpp
   template const std::array<uint64_t, 20>  powers_of_base<10, uint64_t>;

   /**
    *  Writes a number as a string to the provided char buffer
    *
    *  @brief Writes number x 10^(-num_decimal_places) (optionally negative) as a string to the provided char buffer
    *  @pre The range [begin, end) must be a valid range of memory to write to.
    *  @param begin - The start of the char buffer
    *  @param end - Just past the end of the char buffer
    *  @param dry_run - If true, do not actually write anything into the range.
    *  @param number - The number to print before shifting the decimal point to the left by num_decimal_places.
    *  @param num_decimal_places - The number of decimal places to shift the decimal point.
    *  @param negative - Whether to print a minus sign in the front.
    *  @return char* - Just past the end of the last character that would be written assuming dry_run == false and end was large enough to provide sufficient space. (Meaning only applies if returned pointer >= begin.)
    *  @post If the output string fits within the range [begin, end), the range [begin, returned pointer) contains the string representation of the number. Nothing is written if dry_run == true or returned pointer > end (insufficient space) or if returned pointer < begin (overflow in calculating desired end).
    */
   char* write_decimal( char* begin, char* end, bool dry_run, uint64_t number, uint8_t num_decimal_places, bool negative ) {
      const auto& powers_of_ten = powers_of_base<10, uint64_t>;

      uint8_t num_digits = (std::upper_bound( powers_of_ten.begin(), powers_of_ten.end(), number ) - powers_of_ten.begin()); // num_digits == 0 iff number == 0
      // 0 <= num_digits <= 20

      uint16_t characters_needed = std::max( num_digits, num_decimal_places );
      uint16_t decimal_point_pos = num_digits;
      if( num_decimal_places >= num_digits ) {
         ++characters_needed; // space needing for additional leading zero digit
         decimal_point_pos = 1;
      } else {
         decimal_point_pos -= num_decimal_places;
      }
      if( num_decimal_places > 0 ) ++characters_needed; // space for decimal point
      uint16_t after_minus_pos = 0;
      if( negative ) {
         ++characters_needed; // space for minus sign
         ++after_minus_pos;
         ++decimal_point_pos;
      }
      // 1 <= characters_needed <= 258
      // 1 <= decimal_point_pos <= num_digits + 1 <= 21

      char* actual_end = begin + characters_needed;
      if( dry_run || (actual_end < begin) || (actual_end > end) ) return actual_end;

      int i = characters_needed - 1;
      for( ; number > 0 && i > decimal_point_pos; --i ) {
         *(begin + i) = (number % 10) + '0';
         number /= 10;
      }
      for( ; i > decimal_point_pos; --i ) {
         *(begin + i) = '0';
      }
      if( i == decimal_point_pos ) {
         *(begin + i) = '.';
         --i;
      }
      for( ; i >= after_minus_pos; --i ) {
         *(begin + i) = (number % 10) + '0';
         number /= 10;
      }
      if( i == 0 ) *(begin + i) = '-';

      return actual_end;
   }

} // namespace eosio
