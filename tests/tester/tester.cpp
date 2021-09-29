#include <eosio/tester.hpp>
#include <string_view>
#include "../unit/test_contracts/tester_tests.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

eosio::checksum256 make_checksum256(std::string_view src) {
   std::array<uint8_t, 32> buf;
   eosio::unhex(buf.begin(), src.begin(), src.end());
   return eosio::checksum256(buf);
}

TEST_CASE_METHOD(eosio::test_chain, "start_block", "[start_block][finish_block]")
{
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 1);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000001B225618995010555CEBBF045276CBC2CFB0CC27ED412FCC1210B5CC5"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304000));
   }
   start_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 2);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000002C7D8A38018436CFF6FF2AAC872990A3A3F3A870EF19AE90E44F533B3"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304001));
   }
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 3);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000003716F3799687FB2961344E5079D03721FE9E56F2C851310D0A61E43B1"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304002));
   }
   start_block(); // no pending block to finish
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 3);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000003716F3799687FB2961344E5079D03721FE9E56F2C851310D0A61E43B1"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304002));
   }
   start_block(499); // finish block 4
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 5);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000005B804E3F4DD6597A2153CEF0363B9005F6152E13B3EF70C88D2E3F771"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304004));
   }
   start_block(500);
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 7);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000007DB2CFE1174E083D9CBA7AEA0EBE02A9D00EEAE469B9561E289FA4F74"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304006));
   }
}

TEST_CASE_METHOD(eosio::test_chain, "start_block skipping time", "[start_block]") {
   eosio::action empty{ { { "eosio"_n, "active"_n } }, "eosio"_n, eosio::name(), std::tuple() };
   start_block(500);
   transact({empty});
   start_block(1000);
   transact({empty});
   start_block(1500);
   transact({empty});
   // Verify that we can apply a transaction immediately after a large skip
   start_block(1000000);
   transact({empty});
}

template<typename T>
eosio::checksum256 sha256(const T& t) {
   auto packed = eosio::pack(t);
   return eosio::sha256(packed.data(), packed.size());
}

TEST_CASE_METHOD(eosio::test_chain, "transaction_trace members", "[transaction_trace]") {
   eosio::action empty{ { { "eosio"_n, "active"_n } }, "eosio"_n, eosio::name(), std::tuple() };
   eosio::transaction t = make_transaction( { empty } );
   auto trace = push_transaction( t );
   CHECK(trace.id == sha256(t));
   CHECK(trace.status == eosio::transaction_status::executed);
   CHECK(trace.cpu_usage_us == 2000); // The tester always bills 2 ms per transaction.
   CHECK(trace.net_usage_words == 12); // default minimum net usage
   CHECK(trace.elapsed > 0); // Variable
   CHECK(trace.net_usage == 96); // net_usage_words*8
   CHECK(trace.scheduled == false);

   // action_trace
   CHECK(trace.action_traces.size() == 1);
   CHECK(trace.action_traces[0].action_ordinal == 1);
   CHECK(trace.action_traces[0].creator_action_ordinal == 0);
   CHECK(trace.action_traces[0].receipt->receiver == "eosio"_n);
   CHECK(trace.action_traces[0].receipt->act_digest == sha256(empty));
   CHECK(trace.action_traces[0].receipt->global_sequence == 2);
   CHECK(trace.action_traces[0].receipt->recv_sequence == 2);
   CHECK(trace.action_traces[0].receipt->auth_sequence == (std::vector<eosio::account_auth_sequence>{ { "eosio"_n, 2 } }));
   CHECK(trace.action_traces[0].receipt->code_sequence == 0);
   CHECK(trace.action_traces[0].receipt->abi_sequence == 0);
   CHECK(trace.action_traces[0].receiver == "eosio"_n);
   CHECK(trace.action_traces[0].context_free == false);
   CHECK(trace.action_traces[0].elapsed > 0);
   CHECK(trace.action_traces[0].console == "");
   CHECK(trace.action_traces[0].account_ram_deltas == std::vector<eosio::account_delta>{});
   CHECK(!trace.action_traces[0].except);
   CHECK(!trace.action_traces[0].error_code);

   CHECK(!trace.account_ram_delta);
   CHECK(!trace.except);
   CHECK(!trace.error_code);
   CHECK(trace.failed_dtrx_trace.size() == 0);
}

TEST_CASE_METHOD(eosio::test_chain, "Simple action::send", "[send]") {
   eosio::action empty{ { { "eosio"_n, "active"_n } }, "eosio"_n, eosio::name(), std::tuple() };
   empty.send();
}

TEST_CASE_METHOD(eosio::test_chain, "MultiIndex API", "[multi_index]") {
   create_account("test"_n);
   set_code("test"_n, "../unit/test_contracts/tester_tests.wasm");
   tester_tests::putdb_action("test"_n, { "test"_n, "active"_n }).send(1, 2);
   tester_tests::table t("test"_n, 0);
   for(auto& item : t) {
      CHECK(item.key == 1);
      CHECK(item.value == 2);
   }
}

TEST_CASE_METHOD(eosio::test_chain, "Creating signatures", "[sign]") {
   create_account("test"_n);
   set_code("test"_n, "../unit/test_contracts/tester_tests.wasm");
   std::string data = "Cogitanti miho saepenumero et memoria vetera repetenti perbeati fuisse";
   auto hash = eosio::sha256(data.data(), data.size());

   auto k1_pvt = eosio::private_key_from_string("5KDhbSEzvfjmrX9t8hLAzVM1AKxVRFcVutVSKULUt9vquKXNNRX");
   auto k1_pub = eosio::public_key_from_string("EOS8VebGY8Crdk3bhz4RaGNaEuhJPfLoWBogc9pQKMRe7QqJJaxHn");
   auto k1_sig = eosio::sign(k1_pvt, hash);
   as("test"_n).act<tester_tests::assertsig_action>(hash, k1_sig, k1_pub);

   auto r1_pvt = eosio::private_key_from_string("PVT_R1_SXT9sxVG9XZMmtNjbQU42XkimqV61buEbqDsxWJWNNPjQrgSL");
   auto r1_pub = eosio::public_key_from_string("PUB_R1_5BqdSPaEpcCfLSaFCHgnkBUJWiuxt3TcaNndgCRRFG9cXwiFHc");
   auto r1_sig = eosio::sign(r1_pvt, hash);
   as("test"_n).act<tester_tests::assertsig_action>(hash, r1_sig, r1_pub);
}

TEST_CASE("Runtime library 128-bit integer operations") {
   volatile int128_t a = -100, b = 2;
   CHECK((a * b) == -200);
   CHECK((a / b) == -50);
   CHECK((a % b) == 0);
   CHECK((a << b) == -400);
   CHECK((a >> b) == -25);

   volatile uint128_t au = 100, bu = 2;
   CHECK((au * bu) == 200);
   CHECK((au / bu) == 50);
   CHECK((au % bu) == 0);
   CHECK((au << bu) == 400);
   CHECK((au >> bu) == 25);
}
