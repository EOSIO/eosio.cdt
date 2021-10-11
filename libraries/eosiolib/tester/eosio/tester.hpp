#pragma once
#define FMT_HEADER_ONLY

#include <eosio/asset.hpp>
#include <eosio/chain_types.hpp>
#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <eosio/from_string.hpp>
#include <eosio/producer_schedule.hpp>
#include <eosio/transaction.hpp>

#include <cwchar>
#include <fmt/format.h>

namespace eosio {
namespace internal_use_do_not_use {

   std::vector<char> query_database_chain(uint32_t chain, const std::vector<char>& packed_req);
   void hex(const uint8_t* begin, const uint8_t* end, std::ostream& os);

   template <typename R, typename C, typename... Args>
   R get_return_type(R (C::*f)(Args...));
   template <typename R, typename C, typename... Args>
   R get_return_type(R (C::*f)(Args...)const);

} // namespace internal_use_do_not_use

const std::vector<std::string>& get_args();

std::vector<char> read_whole_file(std::string_view filename);

int32_t execute(std::string_view command);

asset string_to_asset(const char* s);

inline asset  s2a(const char* s) { return string_to_asset(s); }

using chain_types::transaction_status;

auto conversion_kind(chain_types::permission_level, permission_level) -> strict_conversion;
auto conversion_kind(chain_types::action, action) -> strict_conversion;

using chain_types::account_delta;
using chain_types::account_auth_sequence;

struct action_receipt {
   name                               receiver        = {};
   checksum256                        act_digest      = {};
   uint64_t                           global_sequence = {};
   uint64_t                           recv_sequence   = {};
   std::vector<account_auth_sequence> auth_sequence   = {};
   uint32_t                           code_sequence   = {};
   uint32_t                           abi_sequence    = {};
};

auto conversion_kind(chain_types::action_receipt_v0, action_receipt) -> strict_conversion;

struct action_trace {
   uint32_t                         action_ordinal         = {};
   uint32_t                         creator_action_ordinal = {};
   std::optional<action_receipt>    receipt                = {};
   name                             receiver               = {};
   action                           act                    = {};
   bool                             context_free           = {};
   int64_t                          elapsed                = {};
   std::string                      console                = {};
   std::vector<account_delta>       account_ram_deltas     = {};
   std::vector<account_delta>       account_disk_deltas    = {};
   std::optional<std::string>       except                 = {};
   std::optional<uint64_t>          error_code             = {};
   std::vector<char>                return_value           = {};
};

auto conversion_kind(chain_types::action_trace_v0, action_trace) -> widening_conversion;
auto conversion_kind(chain_types::action_trace_v1, action_trace) -> strict_conversion;

struct transaction_trace {
   checksum256                            id                  = {};
   transaction_status                     status              = {};
   uint32_t                               cpu_usage_us        = {};
   uint32_t                               net_usage_words     = {};
   int64_t                                elapsed             = {};
   uint64_t                               net_usage           = {};
   bool                                   scheduled           = {};
   std::vector<action_trace>              action_traces       = {};
   std::optional<account_delta>           account_ram_delta   = {};
   std::optional<std::string>             except              = {};
   std::optional<uint64_t>                error_code          = {};
   std::vector<transaction_trace>         failed_dtrx_trace   = {};
};

auto conversion_kind(chain_types::transaction_trace, transaction_trace) -> narrowing_conversion;
auto serialize_as(const transaction_trace&) -> chain_types::transaction_trace;

template <typename F>
void convert(const chain_types::recurse_transaction_trace& src, eosio::transaction_trace& dst, F&& chooser) {
   convert(src.recurse, dst, chooser);
}

using chain_types::block_info;

/**
 * Validates the status of a transaction.  If expected_except is nullptr, then the
 * transaction should succeed.  Otherwise it represents a string which should be
 * part of the error message.
 */
void expect(const transaction_trace& tt, const char* expected_except = nullptr);

/**
 * Same as expect(), but errors indicate transaction was executed from rodeos
 */
void expect_rodeos(const transaction_trace& tt, const char* expected_except = nullptr);

template<std::size_t Size>
std::ostream& operator<<(std::ostream& os, const fixed_bytes<Size>& d) {
   auto arr = d.extract_as_byte_array();
   internal_use_do_not_use::hex(arr.begin(), arr.end(), os);
   return os;
}

std::ostream& operator<<(std::ostream& os, const block_timestamp& obj);
std::ostream& operator<<(std::ostream& os, const name& obj);

class test_rodeos;

/**
 * Sign a digest.  This is here and not in crypto.hpp, because it is
 * only available in the tester.
 */
signature sign(const private_key& key, const checksum256& digest);

/**
 * Manages a chain.
 * Only one test_chain can exist at a time.
 * The test chain uses simulated time starting at 2020-01-01T00:00:00.000.
 */
class test_chain {
    friend test_rodeos;

 private:
   uint32_t                               id;
   std::optional<block_info>              head_block_info;

 public:
   static const public_key  default_pub_key;
   static const private_key default_priv_key;

   test_chain(const char* snapshot = nullptr);
   test_chain(const test_chain&) = delete;
   ~test_chain();

   test_chain& operator=(const test_chain&) = delete;

   /**
    * Shuts down the chain to allow copying its state file. The chain's temporary path will
    * live until this object destructs.
    */
   void shutdown();

   /**
    * Get the temporary path which contains the chain's blocks and states directories
    */
   std::string get_path();

   /**
    * Replace the producer keys with key. This bypasses chain consensus rules.
    * This does not update owner or active keys.
    */
   void replace_producer_keys(const eosio::public_key& key);

   /**
    * Replace the keys of an account. This bypasses chain consensus rules.
    */
   void replace_account_keys(name account, name permission, const eosio::public_key& key);

   /**
    * Replace the owner and active keys of an account. This bypasses chain consensus rules.
    */
   void replace_account_keys(name account, const eosio::public_key& key);

   /**
    * Start a new pending block.  If a block is currently pending, finishes it first.
    * May push additional blocks if any time is skipped.
    *
    * @param skip_milliseconds The amount of time to skip in addition to the 500 ms block time.
    * truncated to a multiple of 500 ms.
    */
   void start_block(int64_t skip_miliseconds = 0);

   /**
    * Finish the current pending block.  If no block is pending, creates an empty block.
    */
   void finish_block();

   const block_info& get_head_block_info();

   /*
    * Set the reference block of the transaction to the head block.
    */
   void fill_tapos(transaction& t, uint32_t expire_sec = 1);

   /*
    * Creates a transaction.
    */
   transaction make_transaction(std::vector<action>&& actions = {}, std::vector<action>&& cfaction = {});

   /**
    * Pushes a transaction onto the chain.  If no block is currently pending, starts one.
    */
   [[nodiscard]]
   transaction_trace push_transaction(const transaction& trx,
                                      const std::vector<private_key>& keys = { default_priv_key },
                                      const std::vector<std::vector<char>>& context_free_data = {},
                                      const std::vector<signature>& signatures        = {});

   /**
    * Pushes a transaction onto the chain.  If no block is currently pending, starts one.
    *
    * Validates the transaction status according to @ref eosio::expect.
    */
   transaction_trace transact(std::vector<action>&& actions, const std::vector<private_key>& keys,
                              const char* expected_except = nullptr);
   transaction_trace transact(std::vector<action>&& actions, const char* expected_except = nullptr);

   /**
    * Pushes a transaction with action onto the chain and converts its return value.
    * If no block is currently pending, starts one.
    */
   template <typename Action, typename... Args>
   auto action_with_return(const Action& action, Args&&... args) {
      using Ret  = decltype(internal_use_do_not_use::get_return_type(Action::get_mem_ptr()));
      auto trace = transact({action.to_action(std::forward<Args>(args)...)});
      return convert_from_bin<Ret>(trace.action_traces[0].return_value);
   }


   template <typename Action, typename... Args>
   auto act( std::optional<std::vector<std::vector<char>>> cfd, const Action& action, Args&&... args) {
      using Ret  = decltype(internal_use_do_not_use::get_return_type(Action::get_mem_ptr()));
      auto trace = transact({action.to_action(std::forward<Args>(args)...)});
      if constexpr ( !std::is_same_v<Ret,void> ) {
         return convert_from_bin<Ret>(trace.action_traces[0].return_value);
      } else {
         return trace;
      }
   }

   template <typename Action, typename... Args>
   auto trace( std::optional<std::vector<std::vector<char> >> cfd, const Action& action, Args&&... args) {
      if( !cfd ) {
         return push_transaction( make_transaction( {action.to_action(std::forward<Args>(args)...)} ), 
                                  { default_priv_key } );
      } else {
         return push_transaction( make_transaction( {}, {action.to_action(std::forward<Args>(args)...)} ), 
                                  { default_priv_key }, *cfd );
      }
   }

   struct user_context {
      test_chain& t;
      std::vector<eosio::permission_level> level;
      std::optional<std::vector<std::vector<char>>>   context_free_data;

      user_context with_cfd( std::vector<std::vector<char>> d ) {
         user_context uc = *this;
         uc.context_free_data = std::move(d);
         return uc;
      }
      
      template <typename Action, typename... Args>
      auto act(Args&&... args) {
         if( context_free_data )
            return t.act( context_free_data, Action(), std::forward<Args>(args)... );
         else
            return t.act( context_free_data, Action(level), std::forward<Args>(args)... );
      }

      template <typename Action, typename... Args>
      auto trace(Args&&... args) {
         if( context_free_data )
            return t.trace( context_free_data, Action(), std::forward<Args>(args)... );
         else
            return t.trace( context_free_data, Action(level), std::forward<Args>(args)... );
      }
   };

   auto as( eosio::name current_user, eosio::name current_perm = "active"_n ) {
      return user_context{ *this, {1,{current_user,current_perm}} };
   }

   /**
    * Executes a single deferred transaction and returns the action trace.
    * If there are no available deferred transactions that are ready to execute
    * in the current pending block, returns an empty optional.
    *
    * If no block is currently pending, starts one.
    */
   [[nodiscard]]
   std::optional<transaction_trace> exec_deferred();

   struct get_history_result {
      /** The other members refer to memory owned here */
      std::vector<char> memory;

      ship_protocol::get_blocks_result_base              result;
      std::optional<ship_protocol::signed_block_variant> block;
      std::vector<ship_protocol::transaction_trace>      traces;
      std::vector<ship_protocol::table_delta>            deltas;
   };

   /**
    * Git SHiP history for a block. Returns nullopt if history doesn't exist for that block.
    * If block_num == 0xffff'ffff, then returns history for the last-produced block, if available.
    */
   std::optional<get_history_result> get_history(uint32_t block_num);

   transaction_trace create_account(name ac, const public_key& pub_key,
                                    const char* expected_except = nullptr);
   transaction_trace create_account(name ac, const char* expected_except = nullptr);

   /**
    * Create an account that can have a contract set on it.
    * @param account The name of the account
    * @param pub_key The public key to use for the account.  Defaults to default_pub_key.
    * @param is_priv Determines whether the contract should be privileged.  Defaults to false.
    * @param expected_except Used to validate the transaction status according to @ref eosio::expect.
    */
   transaction_trace create_code_account(name account, const public_key& pub_key, bool is_priv = false,
                                         const char* expected_except = nullptr);
   transaction_trace create_code_account(name ac, const public_key& pub_key, const char* expected_except);
   transaction_trace create_code_account(name ac, bool is_priv = false,
                                         const char* expected_except = nullptr);
   transaction_trace create_code_account(name ac, const char* expected_except);

   /*
    * Set the code for an account.
    * Validates the transaction status as with @ref eosio::expect.
    */
   transaction_trace set_code(name ac, const char* filename, const char* expected_except = nullptr);

   /**
    * Creates a new token.
    * The eosio.token contract should be deployed on the @c contract account.
    */
   transaction_trace create_token(name contract, name signer, name issuer, asset maxsupply,
                                  const char* expected_except = nullptr);

   transaction_trace issue(const name& contract, const name& issuer, const asset& amount,
                           const char* expected_except = nullptr);

   transaction_trace transfer(const name& contract, const name& from, const name& to, const asset& amount,
                              const std::string& memo = "", const char* expected_except = nullptr);

   transaction_trace issue_and_transfer(const name& contract, const name& issuer, const name& to,
                                        const asset& amount, const std::string& memo = "",
                                        const char* expected_except = nullptr);
}; // test_chain

/**
 * Manages a rodeos instance
 */
class test_rodeos {
 private:
   uint32_t    id;
   test_chain* connected = {};

 public:
   test_rodeos();
   test_rodeos(const test_rodeos&) = delete;
   ~test_rodeos();
   test_rodeos& operator=(const test_rodeos&) = delete;

   /// Connect this rodeos instance to chain. rodeos_sync_block() will receive data from this chain.
   void connect(test_chain& chain);

   /// Add a filter wasm. This will receive data from the chain everytime rodeos_sync_block() is called.
   void add_filter(eosio::name name, const char* wasm_filename);

   /// Enable query handling. If contract_dir is not empty, then wasms in
   /// contract_dir override contracts on chain.
   void enable_queries(uint32_t max_console_size, uint32_t wasm_cache_size, uint64_t max_exec_time_ms,
                       const char* contract_dir);

   /// Fetches a single block of data, if available, from chain. Returns true if data was available.
   bool sync_block();

   /// Fetches blocks of data, if available, from chain. Returns number of blocks.
   uint32_t sync_blocks();

   /// Pushes a query transaction
   [[nodiscard]] transaction_trace push_transaction(const transaction& trx, const std::vector<private_key>& keys = {},
                                                    const std::vector<std::vector<char>>& context_free_data = {},
                                                    const std::vector<signature>&         signatures        = {});

   /// Pushes a query transaction. Validates the transaction status according to @ref eosio::expect.
   transaction_trace transact(std::vector<action>&& actions, const std::vector<private_key>& keys,
                              const char* expected_except = nullptr);
   transaction_trace transact(std::vector<action>&& actions, const char* expected_except = nullptr);

   template <typename Action, typename... Args>
   auto act(std::optional<std::vector<std::vector<char>>> cfd, const Action& action, Args&&... args) {
      using Ret  = decltype(internal_use_do_not_use::get_return_type(Action::get_mem_ptr()));
      auto trace = transact({ action.to_action(std::forward<Args>(args)...) });
      if constexpr (!std::is_same_v<Ret, void>) {
         return convert_from_bin<Ret>(trace.action_traces[0].return_value);
      } else {
         return trace;
      }
   }

   template <typename Action, typename... Args>
   auto trace(std::optional<std::vector<std::vector<char>>> cfd, const Action& action, Args&&... args) {
      if (!cfd) {
         return push_transaction(connected->make_transaction({ action.to_action(std::forward<Args>(args)...) }), {});
      } else {
         return push_transaction(connected->make_transaction({}, { action.to_action(std::forward<Args>(args)...) }), {},
                                 *cfd);
      }
   }

   uint32_t          get_num_pushed_data();
   std::vector<char> get_pushed_data(uint32_t index);
   std::string       get_pushed_data_str(uint32_t index);

   struct user_context {
      test_rodeos&                                  r;
      std::vector<eosio::permission_level>          level;
      std::optional<std::vector<std::vector<char>>> context_free_data;

      user_context with_cfd(std::vector<std::vector<char>> d) {
         user_context uc      = *this;
         uc.context_free_data = std::move(d);
         return uc;
      }

      template <typename Action, typename... Args>
      auto act(Args&&... args) {
         if (context_free_data)
            return r.act(context_free_data, Action(), std::forward<Args>(args)...);
         else
            return r.act(context_free_data, Action(level), std::forward<Args>(args)...);
      }

      template <typename Action, typename... Args>
      auto trace(Args&&... args) {
         if (context_free_data)
            return r.trace(context_free_data, Action(), std::forward<Args>(args)...);
         else
            return r.trace(context_free_data, Action(level), std::forward<Args>(args)...);
      }
   };

   auto as() { return user_context{ *this }; }

}; // test_rodeos

} // namespace eosio

namespace eosio { namespace ship_protocol {

std::ostream& operator<<(std::ostream& os, transaction_status t);
std::ostream& operator<<(std::ostream& os, const account_auth_sequence& aas);
std::ostream& operator<<(std::ostream& os, const account_delta& ad);

}}
