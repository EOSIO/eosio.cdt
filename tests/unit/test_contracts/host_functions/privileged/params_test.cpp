#include <eosio/eosio.hpp>                   /* contract, datastream, check */
#include <eosio/privileged.hpp>              /* blockchain_parameters */
#include <vector>                            /* vector */
using namespace eosio;
using namespace std;

unsigned_int operator "" _ui(unsigned long long v){ return unsigned_int(v); }

uint16_t operator "" _16(unsigned long long v){ return static_cast<uint16_t>(v); }
uint32_t operator "" _32(unsigned long long v){ return static_cast<uint32_t>(v); }
uint64_t operator "" _64(unsigned long long v){ return static_cast<uint64_t>(v); }

class [[eosio::contract("params_test")]] params_test : public eosio::contract {
public:
   using eosio::contract::contract;

   [[eosio::action]] void maintest(){

      //make sure no throw for zero parameters provided
      params_object p0(0_ui);
      set_parameters(p0);
      ASSERT_EQ(get_parameters(p0), params_object(0_ui));

      //1 for size, 1 for id, 8 for data
      params_object p1({1_ui, 0_ui});
      ASSERT_EQ(get_parameters(p1).packed.size(), 10);

      //1 for size, 1 for id, 4 for data
      params_object p2({1_ui, 1_ui});
      ASSERT_EQ(get_parameters(p2).packed.size(), 6);

      //1 for size, 1 for id, 2 for data
      params_object p3({1_ui, 16_ui});
      ASSERT_EQ(get_parameters(p3).packed.size(), 4);

      //1 for size, 3 for ids, 8+4+2 for data
      params_object p4({3_ui, 0_ui, 1_ui, 16_ui});
      ASSERT_EQ(get_parameters(p4).packed.size(), 18);

      blockchain_parameters old_way_params;
      params_object p5 = params_object(1_ui)(0_ui)(1024 * 1024 * 2_64);
      set_parameters(p5);
      get_blockchain_parameters(old_way_params);

      params_object p6({1_ui, 0_ui});
      params_object p7 = get_parameters(p6);
      params_object p8 = params_object(1_ui)(0_ui)(1024 * 1024 * 2_64);

      ASSERT_EQ(p7, p8);
      ASSERT_EQ(old_way_params.max_block_net_usage, 1024*1024*2);

      params_object p9 = params_object(1_ui)(1_ui)(20 * 100_32);
      set_parameters(p9);
      get_blockchain_parameters(old_way_params);

      params_object p10({1_ui, 1_ui});
      params_object p11 = get_parameters(p10);
      params_object p12 = params_object(1_ui)(1_ui)(20 * 100_32);
      ASSERT_EQ(p11, p12);

      ASSERT_EQ(old_way_params.target_block_net_usage_pct, 20*100);

      params_object p13 = params_object(1_ui)(16_ui)(8_16);
      set_parameters(p13);
      get_blockchain_parameters(old_way_params);

      params_object p14({1_ui, 16_ui});
      params_object p15 = get_parameters(p14);

      params_object p16 = params_object(1_ui)(16_ui)(8_16);
      ASSERT_EQ(p15, p16);
      ASSERT_EQ(old_way_params.max_authority_depth, 8);

      params_object p17 = params_object(3_ui)(0_ui)(1024 * 1024 * 3_64)
                                       (1_ui)(30 * 100_32)
                                       (16_ui)(10_16);
      set_parameters(p17);
      get_blockchain_parameters(old_way_params);

      params_object p18({3_ui, 0_ui, 1_ui, 16_ui});
      params_object p19 = get_parameters(p18);

      params_object p20 = params_object(3_ui)(0_ui)(1024*1024*3_64)
                                       (1_ui)(30*100_32)
                                       (16_ui)(10_16);
      ASSERT_EQ(p19, p20);

      ASSERT_EQ(old_way_params.max_block_net_usage, 1024*1024*3);
      ASSERT_EQ(old_way_params.target_block_net_usage_pct, 30*100);
      ASSERT_EQ(old_way_params.max_authority_depth, 10);

      params_object p21 = params_object(17_ui) (0_ui)(1024*1024*4_64)
                                               (1_ui)(35*100_32)
                                               (2_ui)(1024*1024*2_32)
                                               (3_ui)(1024_32)
                                               (4_ui)(700_32)
                                               (5_ui)(50_32)
                                               (6_ui)(120_32)
                                               (7_ui)(250'000_32)
                                               (8_ui)(15*100_32)
                                               (9_ui)(230'000_32)
                                               (10_ui)(1000_32)
                                               (11_ui)(30*60_32)
                                               (12_ui)(15*60_32)
                                               (13_ui)(30*24*3600_32)
                                               (14_ui)(1024*1024_32)
                                               (15_ui)(6_16)
                                               (16_ui)(9_16);
      set_parameters(p21);
      get_blockchain_parameters(old_way_params);

      params_object p22({17_ui,0_ui,1_ui,2_ui,3_ui,4_ui,
                         5_ui,6_ui,7_ui,8_ui,9_ui,
                         10_ui,11_ui,12_ui,13_ui,
                         14_ui,15_ui,16_ui});
      params_object p23 = get_parameters(p22);

      ASSERT_EQ(p23,
                params_object(17_ui)(0_ui)(1024*1024*4_64)
                      (1_ui)(35*100_32)
                      (2_ui)(1024*1024*2_32)
                      (3_ui)(1024_32)
                      (4_ui)(700_32)
                      (5_ui)(50_32)
                      (6_ui)(120_32)
                      (7_ui)(250'000_32)
                      (8_ui)(15*100_32)
                      (9_ui)(230'000_32)
                      (10_ui)(1000_32)
                      (11_ui)(30*60_32)
                      (12_ui)(15*60_32)
                      (13_ui)(30*24*3600_32)
                      (14_ui)(1024*1024_32)
                      (15_ui)(6_16)
                      (16_ui)(9_16));
      ASSERT_EQ(old_way_params.max_block_net_usage, 1024*1024*4);
      ASSERT_EQ(old_way_params.target_block_net_usage_pct, 35*100);
      ASSERT_EQ(old_way_params.max_transaction_net_usage, 1024*1024*2);
      ASSERT_EQ(old_way_params.base_per_transaction_net_usage, 1024);
      ASSERT_EQ(old_way_params.net_usage_leeway, 700);
      ASSERT_EQ(old_way_params.context_free_discount_net_usage_num, 50);
      ASSERT_EQ(old_way_params.context_free_discount_net_usage_den, 120);
      ASSERT_EQ(old_way_params.max_block_cpu_usage, 250'000);
      ASSERT_EQ(old_way_params.target_block_cpu_usage_pct, 15*100);
      ASSERT_EQ(old_way_params.max_transaction_cpu_usage, 230'000);
      ASSERT_EQ(old_way_params.min_transaction_cpu_usage, 1000);
      ASSERT_EQ(old_way_params.max_transaction_lifetime, 30*60);
      ASSERT_EQ(old_way_params.deferred_trx_expiration_window, 15*60);
      ASSERT_EQ(old_way_params.max_transaction_delay, 30*24*3600);
      ASSERT_EQ(old_way_params.max_inline_action_size, 1024*1024);
      ASSERT_EQ(old_way_params.max_inline_action_depth, 6);
      ASSERT_EQ(old_way_params.max_authority_depth, 9);

      //v1 config, max_action_return_value_size
      params_object p24 = params_object(1_ui)(17_ui);
      params_object p25 = get_parameters(p24);
      ASSERT_EQ(p25, params_object(1_ui)(17_ui)(256_32));

      params_object p26 = params_object(1_ui)(17_ui)(512_32);
      set_parameters(p26);

      params_object p27 = params_object(1_ui)(17_ui);
      params_object p28 = get_parameters(p27);
      ASSERT_EQ(p28, params_object(1_ui)(17_ui)(512_32));
   }

   [[eosio::action]] void setthrow1(){
      //unknown configuration index
      params_object p0({1_ui, 100_ui});
      set_parameters(p0);
   }

   [[eosio::action]] void setthrow2(){
      //length=2, only 1 argument provided
      params_object p0({2_ui, 1_ui});
      set_parameters(p0);
   }

   [[eosio::action]] void setthrow3(){
      //passing argument that will fail validation
      params_object p0 = params_object(1_ui)(1_ui)(200*100_32);
      set_parameters(p0);
   }

   [[eosio::action]] void getthrow1(){
      //unknown configuration index
      params_object p0({1_ui, 100_ui});
      get_parameters(p0);
   }

   [[eosio::action]] void getthrow2(){
      //length=2, only 1 argument provided
      params_object p0({2_ui, 1_ui});
      get_parameters(p0);
   }

   [[eosio::action]] void throwrvia1(){
      //throws when setting parameter that is not allowed because of protocol feature for
      //this parameter is not active

      //v1 config, max_action_return_value_size
      params_object p0 = params_object(1_ui)(17_ui)(1024_32);
      set_parameters(p0);

      params_object p1({1_ui, 17_ui});
      params_object p2 = get_parameters(p1);
      ASSERT_EQ(p0, p2);
   }

   [[eosio::action]] void throwrvia2(){
      //this test tries to get parameter with corresponding inactive protocol feature

      //v1 config, max_action_return_value_size
      params_object p0({1_ui, 17_ui});
      params_object p1 = get_parameters(p0);

      params_object p2 = params_object(1_ui)(17_ui)(1024_32);
      ASSERT_EQ(p1, p2);
   }
};