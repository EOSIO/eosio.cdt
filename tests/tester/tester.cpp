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
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000001F9175EC5AE6DA2F9FE99481374AC391603400534E0562E9124325F75"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304000));
   }
   start_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 2);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000002273D84013E8FC942B2F11646B07E8E31774951C5CE19ABD663A1999B"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304001));
   }
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 3);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000003082FC6F82362072BA12D953FF3A9F0F4AC6FEC83072417662212E993"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304002));
   }
   start_block(); // no pending block to finish
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 3);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000003082FC6F82362072BA12D953FF3A9F0F4AC6FEC83072417662212E993"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304002));
   }
   start_block(499); // finish block 4
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 5);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("00000005D4C5FF96B4FBB319E1CB81E581B518AC8133B490C1BB78216B14A0B5"));
      CHECK(info.timestamp == eosio::block_timestamp(1262304004));
   }
   start_block(500);
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      CHECK(info.block_num == 7);
      CHECK(eosio::checksum256(info.block_id) == make_checksum256("000000075DC2520236FAF327E55690B73039EA1AFB6D522C86CBD2A053DB167E"));
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
