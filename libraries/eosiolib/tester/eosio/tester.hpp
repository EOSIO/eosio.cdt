#pragma once
#define FMT_HEADER_ONLY

#include <eosio/asset.hpp>
#include <eosio/chain_types.hpp>
#include <eosio/crypto.hpp>
#include <eosio/database.hpp>
#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>
#include <cwchar>

#include <fmt/format.h>

#define TESTER_INTRINSIC extern "C" __attribute__((eosio_wasm_import))

namespace eosio {
namespace internal_use_do_not_use {

   TESTER_INTRINSIC void get_args(void* cb_alloc_data, void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC bool reenter(const char* args_begin, const char* args_end, void (*cb)(), void* cb_alloc_data,
                                 void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC int32_t open_file(const char* filename_begin, const char* filename_end, const char* mode_begin,
                                      const char* mode_end);
   TESTER_INTRINSIC void    close_file(int32_t file_index);
   TESTER_INTRINSIC bool    write_file(int32_t file_index, const char* content_begin, const char* content_end);
   TESTER_INTRINSIC bool    read_whole_file(const char* filename_begin, const char* filename_end, void* cb_alloc_data,
                                            void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC int32_t execute(const char* command_begin, const char* command_end);

   TESTER_INTRINSIC uint32_t create_chain();
   TESTER_INTRINSIC void     destroy_chain(uint32_t chain);
   TESTER_INTRINSIC void     start_block(uint32_t chain, int64_t skip_miliseconds);
   TESTER_INTRINSIC void     finish_block(uint32_t chain);
   TESTER_INTRINSIC void     get_head_block_info(uint32_t chain, void* cb_alloc_data,
                                                 void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC void     push_transaction(uint32_t chain, const char* args_begin, const char* args_end,
                                              void* cb_alloc_data, void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC bool     exec_deferred(uint32_t chain, void* cb_alloc_data,
                                           void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC void     query_database_chain(uint32_t chain, void* req_begin, void* req_end, void* cb_alloc_data,
                                                  void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC void     select_chain_for_db(uint32_t chain_index);

   template <typename Alloc_fn>
   inline void get_args(Alloc_fn alloc_fn) {
      return get_args(&alloc_fn, [](void* cb_alloc_data, size_t size) -> void* { //
         return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
      });
   }

   inline const std::vector<char>& get_args() {
      static std::optional<std::vector<char>> bytes;
      if (!bytes) {
         get_args([&](size_t size) {
            bytes.emplace();
            bytes->resize(size);
            return bytes->data();
         });
      }
      return *bytes;
   }

   template <typename Alloc_fn>
   inline bool reenter(const char* args_begin, const char* args_end, void (*fn)(), Alloc_fn alloc_fn) {
      return reenter(args_begin, args_end, fn, //
                     &alloc_fn, [](void* cb_alloc_data, size_t size) -> void* {
                        return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
                     });
   }

   template <typename Alloc_fn>
   inline bool read_whole_file(const char* filename_begin, const char* filename_end, Alloc_fn alloc_fn) {
      return read_whole_file(filename_begin, filename_end, &alloc_fn, [](void* cb_alloc_data, size_t size) -> void* { //
         return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
      });
   }

   template <typename Alloc_fn>
   inline void get_head_block_info(uint32_t chain, Alloc_fn alloc_fn) {
      get_head_block_info(chain, &alloc_fn, [](void* cb_alloc_data, size_t size) -> void* { //
         return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
      });
   }

   template <typename Alloc_fn>
   inline void push_transaction(uint32_t chain, const char* args_begin, const char* args_end, Alloc_fn alloc_fn) {
      push_transaction(chain, args_begin, args_end, &alloc_fn, [](void* cb_alloc_data, size_t size) -> void* { //
         return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
      });
   }

   template <typename Alloc_fn>
   inline bool exec_deferred(uint32_t chain, Alloc_fn alloc_fn) {
      return exec_deferred(chain, &alloc_fn, [](void* cb_alloc_data, size_t size) -> void* { //
         return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
      });
   }

   template <typename T, typename Alloc_fn>
   inline void query_database_chain(uint32_t chain, const T& req, Alloc_fn alloc_fn) {
      auto req_data = pack(req);
      query_database_chain(chain, req_data.data(), req_data.data() + req_data.size(), &alloc_fn,
                           [](void* cb_alloc_data, size_t size) -> void* {
                              return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
                           });
   }

} // namespace internal_use_do_not_use

inline const std::vector<std::string>& get_args() {
   static std::optional<std::vector<std::string>> args;
   if (!args) {
      auto&                bytes = internal_use_do_not_use::get_args();
      abieos::input_buffer bin{ bytes.data(), bytes.data() + bytes.size() };
      std::string          error;
      args.emplace();
      check(abieos::bin_to_native<std::vector<std::string>>(*args, error, bin), error);
   }
   return *args;
}

template <typename E>
bool reenter(void (*f)(), E e) {
   auto&       args = internal_use_do_not_use::get_args();
   std::string error;
   bool        ok = internal_use_do_not_use::reenter(args.data(), args.data() + args.size(), f, [&](size_t size) {
      error.resize(size);
      return error.data();
   });
   if (ok)
      return true;
   e(error);
   return false;
}

inline std::vector<char> read_whole_file(std::string_view filename) {
   std::vector<char> result;
   if (!internal_use_do_not_use::read_whole_file(filename.data(), filename.data() + filename.size(), [&](size_t size) {
          result.resize(size);
          return result.data();
       }))
      check(false, "read " + std::string(filename) + " failed");
   return result;
}

inline int32_t execute(std::string_view command) {
   return internal_use_do_not_use::execute(command.data(), command.data() + command.size());
}

inline abieos::public_key string_to_public_key(std::string_view s) {
   std::string        error;
   abieos::public_key result;
   check(abieos::string_to_public_key(result, error, s), error);
   return result;
}

inline abieos::private_key string_to_private_key(std::string_view s) {
   std::string         error;
   abieos::private_key result;
   check(abieos::string_to_private_key(result, error, s), error);
   return result;
}

inline public_key convert(const abieos::public_key& k) { return unpack<public_key>(abieos::native_to_bin(k)); }

inline abieos::asset string_to_asset(const char* s) {
   std::string   error;
   abieos::asset result;
   check(abieos::string_to_asset(result, error, s), error);
   return result;
}

inline symbol convert(abieos::symbol s) { return symbol(s.value); }
inline asset  convert(const abieos::asset& a) { return { a.amount, convert(a.sym) }; }
inline asset  s2a(const char* s) { return convert(string_to_asset(s)); }

/**
 * Validates the status of a transaction.  If expected_except is nullptr, then the
 * transaction should succeed.  Otherwise it represents a string which should be
 * part of the error message.
 */
inline void expect(const chain_types::transaction_trace& ttrace, const char* expected_except = nullptr) {
   auto& tt = std::get<0>(ttrace);
   if (expected_except) {
      if (tt.status == chain_types::transaction_status::executed)
         eosio::check(false, "transaction succeeded, but was expected to fail with: " + std::string(expected_except));
      if (!tt.except)
         eosio::check(false, "transaction has no failure message. expected: " + std::string(expected_except));
      if (tt.except->find(expected_except) == std::string::npos)
         eosio::check(false, "transaction failed with <<<" + *tt.except + ">>>, but was expected to fail with: <<<" +
                                   expected_except + ">>>");
   } else {
      if (tt.status == chain_types::transaction_status::executed)
         return;
      if (tt.except)
         eosio::print(*tt.except, "\n");
      eosio::check(false, "transaction has status " + to_string(tt.status));
   }
}

// TODO: move
struct tester_key_weight {
   eosio::public_key key    = {};
   uint16_t          weight = {};

   EOSLIB_SERIALIZE(tester_key_weight, (key)(weight))
};

// TODO: move
struct tester_permission_level_weight {
   permission_level permission = {};
   uint16_t         weight     = {};

   EOSLIB_SERIALIZE(tester_permission_level_weight, (permission)(weight))
};

// TODO: move
struct tester_wait_weight {
   uint32_t wait_sec = {};
   uint16_t weight   = {};

   EOSLIB_SERIALIZE(tester_wait_weight, (wait_sec)(weight))
};

// TODO: move
struct tester_authority {
   uint32_t                                    threshold = {};
   std::vector<tester_key_weight>              keys      = {};
   std::vector<tester_permission_level_weight> accounts  = {};
   std::vector<tester_wait_weight>             waits     = {};

   EOSLIB_SERIALIZE(tester_authority, (threshold)(keys)(accounts)(waits))
};

/**
 * Manages a chain.
 * Only one test_chain can exist at a time.
 * The test chain uses simulated time starting at 2020-01-01T00:00:00.000.
 */
class test_chain {
 private:
   uint32_t                               id;
   std::optional<chain_types::block_info> head_block_info;

 public:
   abieos::public_key  default_pub_key  = string_to_public_key("EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV");
   abieos::private_key default_priv_key = string_to_private_key("5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3");

   test_chain() : id{ internal_use_do_not_use::create_chain() } {}
   test_chain(const test_chain&) = delete;
   test_chain(test_chain&&)      = delete;
   ~test_chain() { internal_use_do_not_use::destroy_chain(id); }

   test_chain& operator=(const test_chain&) = delete;
   test_chain& operator=(test_chain&&) = default;

   /**
    * Start a new pending block.  If a block is currently pending, finishes it first.
    *
    * @param skip_milliseconds The amount of time to skip in addition to the 500 ms block time.
    */
   void start_block(int64_t skip_miliseconds = 0) {
      head_block_info.reset();
      if (skip_miliseconds > 500) {
         internal_use_do_not_use::start_block(id, skip_miliseconds - 500);
         internal_use_do_not_use::start_block(id, 0);
      } else {
         internal_use_do_not_use::start_block(id, skip_miliseconds);
      }
   }

  /**
   * Finish the current pending block.  If no block is pending, creates an empty block.
   */
   void finish_block() {
      head_block_info.reset();
      internal_use_do_not_use::finish_block(id);
   }

   const chain_types::block_info& get_head_block_info() {
      if (!head_block_info) {
         std::vector<char> bin;
         internal_use_do_not_use::get_head_block_info(id, [&](size_t size) {
            bin.resize(size);
            return bin.data();
         });
         head_block_info = chain_types::assert_bin_to_native<chain_types::block_info>(bin);
      }
      return *head_block_info;
   }

   void fill_tapos(transaction& t, uint32_t expire_sec = 1) {
      auto& info      = get_head_block_info();
      t.expiration    = time_point_sec(((abieos::time_point)info.timestamp).microseconds / 1'000'000 + expire_sec);
      t.ref_block_num = info.block_num;
      memcpy(&t.ref_block_prefix, info.block_id.value.data() + 8, sizeof(t.ref_block_prefix));
   }

   transaction make_transaction() {
      transaction t{ time_point_sec{} };
      fill_tapos(t);
      return t;
   }

   transaction make_transaction(std::vector<action>&& actions) {
      transaction t{ time_point_sec{} };
      fill_tapos(t);
      t.actions = std::move(actions);
      return t;
   }

   /**
    * Pushes a transaction onto the chain.  If no block is currently pending, starts one.
    */
   [[nodiscard]]
   chain_types::transaction_trace push_transaction(const transaction& trx, const std::vector<abieos::private_key>& keys,
                                                   const std::vector<std::vector<char>>& context_free_data = {},
                                                   const std::vector<abieos::signature>& signatures        = {}) {

      std::vector<char> packed_trx = pack(trx);
      std::vector<char> args;
      abieos::native_to_bin(packed_trx, args);
      abieos::native_to_bin(context_free_data, args);
      abieos::native_to_bin(signatures, args);
      abieos::native_to_bin(keys, args);
      std::vector<char> bin;
      internal_use_do_not_use::push_transaction(id, args.data(), args.data() + args.size(), [&](size_t size) {
         bin.resize(size);
         return bin.data();
      });
      return chain_types::assert_bin_to_native<chain_types::transaction_trace>(bin);
   }

   [[nodiscard]]
   chain_types::transaction_trace push_transaction(const transaction& trx) {
      return push_transaction(trx, { default_priv_key });
   }

   /**
    * Pushes a transaction onto the chain.  If no block is currently pending, starts one.
    *
    * Validates the transaction status according to @ref eosio::expect.
    */
   chain_types::transaction_trace transact(std::vector<action>&& actions, const std::vector<abieos::private_key>& keys,
                                           const char* expected_except = nullptr) {
      auto trace = push_transaction(make_transaction(std::move(actions)), keys);
      expect(trace, expected_except);
      return trace;
   }

   chain_types::transaction_trace transact(std::vector<action>&& actions, const char* expected_except = nullptr) {
      return transact(std::move(actions), { default_priv_key }, expected_except);
   }

   /**
    * Executes a single deferred transaction and returns the action trace.
    * If there are no available deferred transactions that are ready to execute
    * in the current pending block, returns an empty optional.
    *
    * If no block is currently pending, starts one.
    */
   [[nodiscard]]
   std::optional<chain_types::transaction_trace> exec_deferred() {
      std::vector<char> bin;
      if (!internal_use_do_not_use::exec_deferred(id, [&](size_t size) {
             bin.resize(size);
             return bin.data();
          }))
         return {};
      return chain_types::assert_bin_to_native<chain_types::transaction_trace>(bin);
   }

   chain_types::transaction_trace create_account(name ac, const public_key& pub_key,
                                                 const char* expected_except = nullptr) {
      tester_authority simple_auth{
         .threshold = 1,
         .keys      = { { pub_key, 1 } },
      };
      return transact({ action{ { { "eosio"_n, "active"_n } },
                                "eosio"_n,
                                "newaccount"_n,
                                std::make_tuple("eosio"_n, ac, simple_auth, simple_auth) } },
                      expected_except);
   }

   chain_types::transaction_trace create_account(name ac, const char* expected_except = nullptr) {
      return create_account(ac, convert(default_pub_key), expected_except);
   }

   chain_types::transaction_trace create_code_account(name ac, const public_key& pub_key, bool is_priv = false,
                                                      const char* expected_except = nullptr) {
      tester_authority simple_auth{
         .threshold = 1,
         .keys      = { { pub_key, 1 } },
      };
      tester_authority code_auth{
         .threshold = 1,
         .keys      = { { pub_key, 1 } },
         .accounts  = { { { ac, "eosio.code"_n }, 1 } },
      };
      return transact(
            {
                  action{ { { "eosio"_n, "active"_n } },
                          "eosio"_n,
                          "newaccount"_n,
                          std::make_tuple("eosio"_n, ac, simple_auth, code_auth) },
                  action{ { { "eosio"_n, "active"_n } }, "eosio"_n, "setpriv"_n, std::make_tuple(ac, is_priv) },
            },
            expected_except);
   }

   chain_types::transaction_trace create_code_account(name ac, const public_key& pub_key, const char* expected_except) {
      return create_code_account(ac, pub_key, false, expected_except);
   }

   chain_types::transaction_trace create_code_account(name ac, bool is_priv = false,
                                                      const char* expected_except = nullptr) {
      return create_code_account(ac, convert(default_pub_key), is_priv, expected_except);
   }

   chain_types::transaction_trace create_code_account(name ac, const char* expected_except) {
      return create_code_account(ac, convert(default_pub_key), false, expected_except);
   }

   /*
    * Set the code for an account.
    * Validates the transaction status as with @ref eosio::expect.
    */
   chain_types::transaction_trace set_code(name ac, const char* filename, const char* expected_except = nullptr) {
      return transact({ action{ { { ac, "active"_n } },
                                "eosio"_n,
                                "setcode"_n,
                                std::make_tuple(ac, uint8_t{ 0 }, uint8_t{ 0 }, read_whole_file(filename)) } },
                      expected_except);
   }

   chain_types::transaction_trace create_token(name contract, name signer, name issuer, asset maxsupply,
                                               const char* expected_except = nullptr) {
      return transact(
            { action{ { { signer, "active"_n } }, contract, "create"_n, std::make_tuple(issuer, maxsupply) } },
            expected_except);
   }

   chain_types::transaction_trace issue(const name& contract, const name& issuer, const asset& amount,
                                        const char* expected_except = nullptr) {
      return transact({ action{ { { issuer, "active"_n } },
                                contract,
                                "issue"_n,
                                std::make_tuple(issuer, amount, std::string{ "issuing" }) } },
                      expected_except);
   }

   chain_types::transaction_trace transfer(const name& contract, const name& from, const name& to, const asset& amount,
                                           const std::string& memo = "", const char* expected_except = nullptr) {

      return transact(
            { action{ { { from, "active"_n } }, contract, "transfer"_n, std::make_tuple(from, to, amount, memo) } },
            expected_except);
   }

   chain_types::transaction_trace issue_and_transfer(const name& contract, const name& issuer, const name& to,
                                                     const asset& amount, const std::string& memo = "",
                                                     const char* expected_except = nullptr) {
      return transact(
            {
                  action{ { { issuer, "active"_n } },
                          contract,
                          "issue"_n,
                          std::make_tuple(issuer, amount, std::string{ "issuing" }) },
                  action{
                        { { issuer, "active"_n } }, contract, "transfer"_n, std::make_tuple(issuer, to, amount, memo) },
            },
            expected_except);
   }

   template <typename T>
   inline std::vector<char> query_database(const T& request) {
      std::vector<char> result;
      internal_use_do_not_use::query_database_chain(id, request, [&result](size_t size) {
         result.resize(size);
         return result.data();
      });
      return result;
   }
};

} // namespace eosio
