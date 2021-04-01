/*  host functions which are not allowed to use in read only query contract, so the functions should never return true. it should compile failed (compile time)  or throw exception(run time).
set_resource_limits : yes
set_wasm_parameters_packed : yes
set_resource_limit : yes 
set_proposed_producers : yes
set_proposed_producers_ex : yes
set_blockchain_parameters_packed : yes
set_parameters_packed : yes
set_kv_parameters_packed : yes
set_privileged  : yes
kv_erase : yes
kv_set  : yes
send_deferred : yes
*/

#include <eosio/eosio.hpp>
#include <eosio/table.hpp>
#include <eosio/contract.hpp>
#include <eosio/action.hpp>
#include <eosio/crypto.hpp>
#include <eosio/fixed_bytes.hpp>
#include <eosio/privileged.hpp>
#include <eosio/producer_schedule.hpp>

#include <eosio/asset.hpp>
#include <eosio/binary_extension.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>


extern "C" __attribute__((eosio_wasm_import)) void set_resource_limit(int64_t, int64_t, int64_t);
extern "C" __attribute__((eosio_wasm_import)) uint32_t get_kv_parameters_packed(void* params, uint32_t size, uint32_t max_version);
extern "C" __attribute__((eosio_wasm_import)) void set_kv_parameters_packed(const char* params, uint32_t size);
extern "C" __attribute__((eosio_wasm_import)) void set_blockchain_parameters_packed( char* data, uint32_t datalen );
extern "C" __attribute__((eosio_wasm_import)) uint32_t get_blockchain_parameters_packed( char* data, uint32_t datalen );

typedef uint64_t capi_name;
extern "C" __attribute__((eosio_wasm_import)) int32_t db_store_i64(uint64_t scope, capi_name table, capi_name payer, uint64_t id,  const void* data, uint32_t len);
extern "C" __attribute__((eosio_wasm_import)) void db_update_i64(int32_t iterator, capi_name payer, const void* data, uint32_t len);
extern "C" __attribute__((eosio_wasm_import)) void db_remove_i64(int32_t iterator);
extern "C" __attribute__((eosio_wasm_import)) int32_t db_idx64_store(uint64_t scope, capi_name table, capi_name payer, uint64_t id, const uint64_t* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx64_update(int32_t iterator, capi_name payer, const uint64_t* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx64_remove(int32_t iterator);
extern "C" __attribute__((eosio_wasm_import)) int32_t db_idx128_store(uint64_t scope, capi_name table, capi_name payer, uint64_t id, const uint128_t* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx128_update(int32_t iterator, capi_name payer, const uint128_t* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx128_remove(int32_t iterator);
extern "C" __attribute__((eosio_wasm_import)) int32_t db_idx256_store(uint64_t scope, capi_name table, capi_name payer, uint64_t id, const uint128_t* data, uint32_t data_len );
extern "C" __attribute__((eosio_wasm_import)) void db_idx256_update(int32_t iterator, capi_name payer, const uint128_t* data, uint32_t data_len);
extern "C" __attribute__((eosio_wasm_import)) void db_idx256_remove(int32_t iterator);
extern "C" __attribute__((eosio_wasm_import)) int32_t db_idx_double_store(uint64_t scope, capi_name table, capi_name payer, uint64_t id, const double* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx_double_update(int32_t iterator, capi_name payer, const double* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx_double_remove(int32_t iterator);
extern "C" __attribute__((eosio_wasm_import)) int32_t db_idx_long_double_store(uint64_t scope, capi_name table, capi_name payer, uint64_t id, const long double* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx_long_double_update(int32_t iterator, capi_name payer, const long double* secondary);
extern "C" __attribute__((eosio_wasm_import)) void db_idx_long_double_remove(int32_t iterator);

extern "C" __attribute__((eosio_wasm_import)) int64_t kv_erase(uint64_t contract, const char* key, uint32_t key_size);
extern "C" __attribute__((eosio_wasm_import)) int64_t kv_set(uint64_t contract, const char* key, uint32_t key_size, const char* value, uint32_t value_size, uint64_t payer);
extern "C" __attribute__((eosio_wasm_import)) void send_deferred(const uint128_t&, uint64_t, const char*, size_t, uint32_t);
extern "C" __attribute__((eosio_wasm_import)) int64_t set_proposed_producers( char*, uint32_t );
extern "C" __attribute__((eosio_wasm_import)) int64_t set_proposed_producers_ex( uint64_t producer_data_format, char *producer_data, uint32_t producer_data_size );
extern "C" __attribute__((eosio_wasm_import)) void set_wasm_parameters_packed(const void*, std::size_t);
extern "C" __attribute__((eosio_wasm_import)) void set_parameters_packed( const char* params, uint32_t params_size );

extern "C" __attribute__((eosio_wasm_import)) void send_inline(char *serialized_action, size_t size);
extern "C" __attribute__((eosio_wasm_import)) void send_context_free_inline(char *serialized_action, size_t size);

#define ACTION_TYPE  [[eosio::action, eosio::read_only]]

class [[eosio::contract]] host_functions_tests : public eosio::contract {
public:
   using contract::contract;
    
   ACTION_TYPE
   bool resource() {
      int64_t ram_bytes;
      int64_t net_weight;
      int64_t cpu_weight;
      get_resource_limits( "eosio"_n, ram_bytes, net_weight,  cpu_weight ) ;
      eosio::cout << "Get resource: ram_bytes=" << ram_bytes << " net_weight=" << net_weight << " cpu_weight=" << cpu_weight << " \n";
      set_resource_limits( "eosio"_n, ram_bytes  , net_weight  ,  cpu_weight );
      get_resource_limits( "eosio"_n, ram_bytes, net_weight,  cpu_weight ) ;
      eosio::cout << "Get resource: ram_bytes=" << ram_bytes << " net_weight=" << net_weight << " cpu_weight=" << cpu_weight << " \n";
      return true;
   }
   ACTION_TYPE
   bool setrelimit () {
      int64_t ram_bytes;
      int64_t net_weight;
      int64_t cpu_weight;
      get_resource_limits( "eosio"_n, ram_bytes, net_weight,  cpu_weight ) ;
      eosio::cout << "Get resource: ram_bytes=" << ram_bytes << " net_weight=" << net_weight << " cpu_weight=" << cpu_weight << " \n";
      set_resource_limit( "eosio"_n.value, "ram"_n.value  , ram_bytes );
      get_resource_limits( "eosio"_n, ram_bytes, net_weight,  cpu_weight ) ;
      eosio::cout << "Get resource: ram_bytes=" << ram_bytes << " net_weight=" << net_weight << " cpu_weight=" << cpu_weight << " \n";
      return true;
   }
   ACTION_TYPE
   bool bcpara () {
      char buf[sizeof(eosio::blockchain_parameters)];
      size_t size = get_blockchain_parameters_packed( buf, sizeof(buf) );
      eosio::cout << "Block chain parameter size : " << size << "\n";
      set_blockchain_parameters_packed(buf, size); 
      return true;
   }
   ACTION_TYPE
   bool setkvpara(){
      uint32_t limits[4];
      limits[0] = 0;
      limits[1] = 1024;
      limits[2] = 4096;
      limits[3] = 1024;
      char limits_buf[sizeof(limits)];
      memcpy(limits_buf, limits, sizeof(limits));
      set_kv_parameters_packed(limits_buf, sizeof(limits));
      return true;
   }
   ACTION_TYPE
   bool setpriv() {
      bool ispr = is_privileged("eosio"_n);
      eosio::cout << "eosio is privileged : " << ispr << "\n";
      set_privileged("eosio"_n, ispr);      
      return true;
   }
/*  all tested
db_store_i64 
db_update_i64 
db_remove_i64
db_idx64_store
db_idx64_update
db_idx64_remove
db_idx128_store
db_idx128_update
db_idx128_remove
db_idx256_store
db_idx256_update
db_idx256_remove
db_idx_double_store
db_idx_double_update
db_idx_double_remove
db_idx_long_double_store
db_idx_long_double_update
db_idx_long_double_remove
*/
// abcde  means 67890  a4 means 64  12c means 128 so as to no conflict with naming rule
// Name should be less than 13 characters and only contains the following symbol 12345abcdefghijklmnopqrstuvwxyz
   ACTION_TYPE
   bool dbia4s(){
      db_store_i64(0, 0, 0, 0, NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool dbia4u(){
      db_update_i64(0, 0, NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool dbia4r(){
      db_remove_i64(0);
      return true;
   }
   ACTION_TYPE
   bool dbidxa4s() {
      db_idx64_store(0, 0, 0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidxa4u() {
      db_idx64_update(0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidxa4r() {
      db_idx64_remove(0);
      return true;
   }
   ACTION_TYPE
   bool dbidx12cs() {
      db_idx128_store(0, 0, 0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidx12cu() {
      db_idx128_update(0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidx12cr() {
      db_idx128_remove(0);
      return true;
   }
   ACTION_TYPE
   bool dbidx25as() {
      db_idx256_store(0, 0, 0, 0, NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool dbidx25au() {
      db_idx256_update(0, 0, NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool dbidx25ar() {
      db_idx256_remove(0);
      return true;
   }
   ACTION_TYPE
   bool dbidxdbs(){
      db_idx_double_store(0, 0, 0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidxdbu(){
      db_idx_double_update(0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidxdbr(){
      db_idx_double_remove(0);
      return true;
   }
   ACTION_TYPE
   bool dbidxldbs (){
      db_idx_long_double_store(0, 0, 0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidxldbu(){
      db_idx_long_double_update(0, 0, NULL);
      return true;
   }
   ACTION_TYPE
   bool dbidxldbr(){
      db_idx_long_double_remove(0);
      return true;
   }
   ACTION_TYPE
   bool kverase(){
      kv_erase(0, NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool kvset(){
      kv_set(0, NULL, 0, NULL, 0, 0);
      return true;
   }
   ACTION_TYPE
   bool senddefer(){
      send_deferred(0, 0, NULL, 0, 0);
      return true;
   }
   ACTION_TYPE
   bool setpp(){
      set_proposed_producers(NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool setppex(){
      set_proposed_producers_ex( 0, NULL, 0 );
      return true;
   }
   ACTION_TYPE
   bool swpp(){
      set_wasm_parameters_packed(NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool spp(){
      set_parameters_packed( NULL, 0 );
      return true;
   }
   ACTION_TYPE
   bool sendil(){
      send_inline(NULL, 0);
      return true;
   }
   ACTION_TYPE
   bool sendcfiil(){     
      send_context_free_inline(NULL, 0);
      return true;
   }
};
