#include <eosio/tester.hpp>
#include <string_view>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

eosio::checksum256 make_checksum256(std::string_view src) {
   std::array<uint8_t, 32> buf;
   eosio::unhex(buf.begin(), src.begin(), src.end());
   return eosio::checksum256(buf);
}

BOOST_FIXTURE_TEST_CASE(start_finish_block, eosio::test_chain)
{
   {
      eosio::block_info info = get_head_block_info();
      BOOST_TEST(info.block_num == 1);
      BOOST_TEST(eosio::checksum256(info.block_id) == make_checksum256("00000001F9175EC5AE6DA2F9FE99481374AC391603400534E0562E9124325F75"));
      BOOST_TEST(info.timestamp == eosio::block_timestamp(1262304000));
   }
   start_block();
   {
      eosio::block_info info = get_head_block_info();
      BOOST_TEST(info.block_num == 2);
      BOOST_TEST(eosio::checksum256(info.block_id) == make_checksum256("00000002273D84013E8FC942B2F11646B07E8E31774951C5CE19ABD663A1999B"));
      BOOST_TEST(info.timestamp == eosio::block_timestamp(1262304001));
   }
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      BOOST_TEST(info.block_num == 3);
      BOOST_TEST(eosio::checksum256(info.block_id) == make_checksum256("00000003082FC6F82362072BA12D953FF3A9F0F4AC6FEC83072417662212E993"));
      BOOST_TEST(info.timestamp == eosio::block_timestamp(1262304002));
   }
   start_block(); // no pending block to finish
   {
      eosio::block_info info = get_head_block_info();
      BOOST_TEST(info.block_num == 3);
      BOOST_TEST(eosio::checksum256(info.block_id) == make_checksum256("00000003082FC6F82362072BA12D953FF3A9F0F4AC6FEC83072417662212E993"));
      BOOST_TEST(info.timestamp == eosio::block_timestamp(1262304002));
   }
   start_block(499); // finish block 4
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      BOOST_TEST(info.block_num == 5);
      BOOST_TEST(eosio::checksum256(info.block_id) == make_checksum256("00000005D4C5FF96B4FBB319E1CB81E581B518AC8133B490C1BB78216B14A0B5"));
      BOOST_TEST(info.timestamp == eosio::block_timestamp(1262304004));
   }
   start_block(500);
   finish_block();
   {
      eosio::block_info info = get_head_block_info();
      BOOST_TEST(info.block_num == 7);
      BOOST_TEST(eosio::checksum256(info.block_id) == make_checksum256("000000075DC2520236FAF327E55690B73039EA1AFB6D522C86CBD2A053DB167E"));
      BOOST_TEST(info.timestamp == eosio::block_timestamp(1262304006));
   }
}

BOOST_FIXTURE_TEST_CASE(start_block_skip, eosio::test_chain) {
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

BOOST_FIXTURE_TEST_CASE(transaction_trace, eosio::test_chain) {
   eosio::action empty{ { { "eosio"_n, "active"_n } }, "eosio"_n, eosio::name(), std::tuple() };
   eosio::transaction t = make_transaction( { empty } );
   auto trace = push_transaction( t );
   BOOST_TEST(trace.id == sha256(t));
   BOOST_TEST(trace.status == eosio::transaction_status::executed);
   BOOST_TEST(trace.cpu_usage_us == 2000); // The tester always bills 2 ms per transaction.
   BOOST_TEST(trace.net_usage_words == 12); // default minimum net usage
   BOOST_TEST(trace.elapsed > 0); // Variable
   BOOST_TEST(trace.net_usage == 96); // net_usage_words*8
   BOOST_TEST(trace.scheduled == false);

   // action_trace
   BOOST_TEST(trace.action_traces.size() == 1);
   BOOST_TEST(trace.action_traces[0].action_ordinal == 1);
   BOOST_TEST(trace.action_traces[0].creator_action_ordinal == 0);
   BOOST_TEST(trace.action_traces[0].receipt->receiver == "eosio"_n);
   BOOST_TEST(trace.action_traces[0].receipt->act_digest == sha256(empty));
   BOOST_TEST(trace.action_traces[0].receipt->global_sequence == 2);
   BOOST_TEST(trace.action_traces[0].receipt->recv_sequence == 2);
   BOOST_TEST(trace.action_traces[0].receipt->auth_sequence == (std::vector<eosio::account_auth_sequence>{ { "eosio"_n, 2 } }), boost::test_tools::per_element());
   BOOST_TEST(trace.action_traces[0].receipt->code_sequence == 0);
   BOOST_TEST(trace.action_traces[0].receipt->abi_sequence == 0);
   BOOST_TEST(trace.action_traces[0].receiver == "eosio"_n);
   BOOST_TEST(trace.action_traces[0].context_free == false);
   BOOST_TEST(trace.action_traces[0].elapsed > 0);
   BOOST_TEST(trace.action_traces[0].console == "");
   BOOST_TEST(trace.action_traces[0].account_ram_deltas == std::vector<eosio::account_delta>{}, boost::test_tools::per_element());
   BOOST_TEST(!trace.action_traces[0].except);
   BOOST_TEST(!trace.action_traces[0].error_code);

   BOOST_TEST(!trace.account_ram_delta);
   BOOST_TEST(!trace.except);
   BOOST_TEST(!trace.error_code);
   BOOST_TEST(trace.failed_dtrx_trace.size() == 0);
}
