#include <eosio/tester.hpp>
#include <eosio/from_string.hpp>

#define TESTER_INTRINSIC extern "C" __attribute__((eosio_wasm_import))

namespace {

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

   TESTER_INTRINSIC uint32_t create_chain(const char* snapshot);
   TESTER_INTRINSIC void     destroy_chain(uint32_t chain);
   TESTER_INTRINSIC void     shutdown_chain(uint32_t chain);
   TESTER_INTRINSIC uint32_t get_chain_path(uint32_t chain, char* dest, uint32_t size);
   TESTER_INTRINSIC void     replace_producer_keys(uint32_t chain, const char* key_begin, uint32_t key_size);
   TESTER_INTRINSIC void     replace_account_keys(uint32_t chain, uint64_t account, const char* key_begin, uint32_t key_size);
   TESTER_INTRINSIC void     start_block(uint32_t chain, int64_t skip_miliseconds);
   TESTER_INTRINSIC void     finish_block(uint32_t chain);
   TESTER_INTRINSIC void     get_head_block_info(uint32_t chain, void* cb_alloc_data,
                                                 void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC void     push_transaction(uint32_t chain, const char* args_begin, const char* args_end,
                                              void* cb_alloc_data, void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC uint32_t get_history(uint32_t chain_index, uint32_t block_num, char* dest, uint32_t size);
   TESTER_INTRINSIC bool     exec_deferred(uint32_t chain, void* cb_alloc_data,
                                           void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC void     query_database_chain(uint32_t chain, const void* req_begin, const void* req_end, void* cb_alloc_data,
                                                  void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC void     select_chain_for_db(uint32_t chain_index);

   TESTER_INTRINSIC uint32_t create_rodeos();
   TESTER_INTRINSIC void     destroy_rodeos(uint32_t rodeos);
   TESTER_INTRINSIC void     rodeos_add_filter(uint32_t rodeos, uint64_t name, const char* wasm_filename);
   TESTER_INTRINSIC void     rodeos_enable_queries(uint32_t rodeos, uint32_t max_console_size, uint32_t wasm_cache_size,
                                                   uint64_t max_exec_time_ms, const char* contract_dir);
   TESTER_INTRINSIC void     connect_rodeos(uint32_t rodeos, uint32_t chain);
   TESTER_INTRINSIC bool     rodeos_sync_block(uint32_t rodeos);
   TESTER_INTRINSIC void     rodeos_push_transaction(uint32_t rodeos, const char* args_begin, const char* args_end,
                                                     void* cb_alloc_data,
                                                     void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   TESTER_INTRINSIC uint32_t rodeos_get_num_pushed_data(uint32_t rodeos);
   TESTER_INTRINSIC uint32_t rodeos_get_pushed_data(uint32_t rodeos, uint32_t index, char* dest, uint32_t size);

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

   template <typename Alloc_fn>
   inline void rodeos_push_transaction(uint32_t rodeos, const char* args_begin, const char* args_end,
                                       Alloc_fn alloc_fn) {
      rodeos_push_transaction(rodeos, args_begin, args_end, &alloc_fn, [](void* cb_alloc_data, size_t size) -> void* {
         return (*reinterpret_cast<Alloc_fn*>(cb_alloc_data))(size);
      });
   }

} // namespace

std::vector<char> eosio::internal_use_do_not_use::query_database_chain(uint32_t chain, const std::vector<char>& req_data) {
   std::vector<char> result;
   ::query_database_chain(chain, req_data.data(), req_data.data() + req_data.size(), &result,
                          [](void* cb_alloc_data, size_t size) -> void* {
                             auto& result = *static_cast<std::vector<char>*>(cb_alloc_data);
                             result.resize(size);
                             return result.data();
                          });
   return result;
}

const std::vector<std::string>& eosio::get_args() {
   static std::optional<std::vector<std::string>> args;
   if (!args) {
      auto&                bytes = ::get_args();
      args.emplace();
      args = convert_from_bin<std::vector<std::string>>(bytes);
   }
   return *args;
}

std::vector<char> eosio::read_whole_file(std::string_view filename) {
   std::vector<char> result;
   if (!::read_whole_file(filename.data(), filename.data() + filename.size(), [&](size_t size) {
          result.resize(size);
          return result.data();
       }))
      check(false, "read " + std::string(filename) + " failed");
   return result;
}

int32_t eosio::execute(std::string_view command) {
   return ::execute(command.data(), command.data() + command.size());
}

eosio::asset eosio::string_to_asset(const char* s) {
   return eosio::convert_from_string<asset>(s);
}

namespace {

// TODO: move
struct tester_permission_level_weight {
   eosio::permission_level permission = {};
   uint16_t                weight     = {};
};

EOSIO_REFLECT(tester_permission_level_weight, permission, weight);

// TODO: move
struct tester_wait_weight {
   uint32_t wait_sec = {};
   uint16_t weight   = {};
};

EOSIO_REFLECT(tester_wait_weight, wait_sec, weight);

// TODO: move
struct tester_authority {
   uint32_t                                    threshold = {};
   std::vector<eosio::key_weight>              keys      = {};
   std::vector<tester_permission_level_weight> accounts  = {};
   std::vector<tester_wait_weight>             waits     = {};
};

EOSIO_REFLECT(tester_authority, threshold, keys, accounts, waits);

}

/**
 * Validates the status of a transaction.  If expected_except is nullptr, then the
 * transaction should succeed.  Otherwise it represents a string which should be
 * part of the error message.
 */
void eosio::expect(const transaction_trace& tt, const char* expected_except) {
   if (expected_except) {
      if (tt.status == transaction_status::executed)
         eosio::check(false, "transaction succeeded, but was expected to fail with: " + std::string(expected_except));
      if (!tt.except)
         eosio::check(false, "transaction has no failure message. expected: " + std::string(expected_except));
      if (tt.except->find(expected_except) == std::string::npos)
         eosio::check(false, "transaction failed with <<<" + *tt.except + ">>>, but was expected to fail with: <<<" +
                                   expected_except + ">>>");
   } else {
      if (tt.status == transaction_status::executed)
         return;
      if (tt.except)
         eosio::print("transaction has exception: ", *tt.except, "\n");
      eosio::check(false, "transaction failed with status " + to_string(tt.status));
   }
}

void eosio::expect_rodeos(const transaction_trace& tt, const char* expected_except) {
   if (expected_except) {
      if (tt.status == transaction_status::executed)
         eosio::check(false,
                      "rodeos transaction succeeded, but was expected to fail with: " + std::string(expected_except));
      if (!tt.except)
         eosio::check(false, "rodeos transaction has no failure message. expected: " + std::string(expected_except));
      if (tt.except->find(expected_except) == std::string::npos)
         eosio::check(false, "rodeos transaction failed with <<<" + *tt.except +
                                   ">>>, but was expected to fail with: <<<" + expected_except + ">>>");
   } else {
      if (tt.status == transaction_status::executed)
         return;
      if (tt.except)
         eosio::print("rodeos transaction has exception: ", *tt.except, "\n");
      eosio::check(false, "rodeos transaction failed with status " + to_string(tt.status));
   }
}

void eosio::internal_use_do_not_use::hex(const uint8_t* begin, const uint8_t* end, std::ostream& os) {
    std::ostreambuf_iterator<char> dest(os.rdbuf());
    auto nibble = [&dest](uint8_t i) {
        if (i <= 9)
            *dest++ = '0' + i;
        else
            *dest++ = 'A' + i - 10;
    };
    while (begin != end) {
        nibble(((uint8_t)*begin) >> 4);
        nibble(((uint8_t)*begin) & 0xf);
        ++begin;
    }
}

std::ostream& eosio::operator<<(std::ostream& os, const block_timestamp& obj) {
   return os << obj.slot;
}

std::ostream& eosio::operator<<(std::ostream& os, const name& obj) {
   return os << obj.to_string();
}

const eosio::public_key  eosio::test_chain::default_pub_key  = public_key_from_string("EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV");
const eosio::private_key eosio::test_chain::default_priv_key = private_key_from_string("5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3");

// We only allow one chain to exist at a time in the tester.
// If we ever find that we need multiple chains, this will
// need to be kept in sync with whatever updates the native layer.
static eosio::test_chain* current_chain = nullptr;

eosio::test_chain::test_chain(const char* snapshot) : id{ ::create_chain(snapshot ? snapshot : "") } {
   current_chain = this;
}

eosio::test_chain::~test_chain() {
   current_chain = nullptr;
   ::destroy_chain(id);
}

void eosio::test_chain::shutdown() { ::shutdown_chain(id); }

std::string eosio::test_chain::get_path() {
   size_t      len = get_chain_path(id, nullptr, 0);
   std::string result(len, 0);
   get_chain_path(id, result.data(), len);
   return result;
}

void eosio::test_chain::replace_producer_keys(const eosio::public_key& key) {
   std::vector<char> packed = pack(key);
   ::replace_producer_keys(id, packed.data(), packed.size());
}

void eosio::test_chain::replace_account_keys(name account, const eosio::public_key& key) {
   std::vector<char> packed = pack(key);
   ::replace_account_keys(id, account.value, packed.data(), packed.size());
}

void eosio::test_chain::start_block(int64_t skip_miliseconds) {
   head_block_info.reset();
   if (skip_miliseconds >= 500) {
      // Guarantee that there is a recent block for fill_tapos to use.
      ::start_block(id, skip_miliseconds - 500);
      ::start_block(id, 0);
   } else {
      ::start_block(id, skip_miliseconds);
   }
}

void eosio::test_chain::finish_block() {
   head_block_info.reset();
   ::finish_block(id);
}

const eosio::block_info& eosio::test_chain::get_head_block_info() {
   if (!head_block_info) {
      std::vector<char> bin;
      ::get_head_block_info(id, [&](size_t size) {
         bin.resize(size);
         return bin.data();
      });
      head_block_info = convert_from_bin<block_info>(bin);
   }
   return *head_block_info;
}

void eosio::test_chain::fill_tapos(transaction& t, uint32_t expire_sec) {
   auto& info      = get_head_block_info();
   t.expiration    = time_point_sec(info.timestamp) + expire_sec;
   t.ref_block_num = info.block_num;
   memcpy(&t.ref_block_prefix, info.block_id.extract_as_byte_array().data() + 8, sizeof(t.ref_block_prefix));
}

eosio::transaction eosio::test_chain::make_transaction(std::vector<action>&& actions, std::vector<action>&& cfa  ) {
   transaction t{ time_point_sec{} };
   fill_tapos(t);
   t.actions = std::move(actions);
   t.context_free_actions = std::move(cfa);
   return t;
}

[[nodiscard]]
eosio::transaction_trace eosio::test_chain::push_transaction(const transaction& trx, const std::vector<private_key>& keys,
                                                             const std::vector<std::vector<char>>& context_free_data,
                                                             const std::vector<signature>& signatures) {

   std::vector<char> packed_trx = pack(trx);
   std::vector<char> args;
   (void)convert_to_bin(packed_trx, args);
   (void)convert_to_bin(context_free_data, args);
   (void)convert_to_bin(signatures, args);
   (void)convert_to_bin(keys, args);
   std::vector<char> bin;
   ::push_transaction(id, args.data(), args.data() + args.size(), [&](size_t size) {
      bin.resize(size);
      return bin.data();
   });
   return convert_from_bin<transaction_trace>(bin);
}

eosio::transaction_trace eosio::test_chain::transact(std::vector<action>&& actions, const std::vector<private_key>& keys,
                                              const char* expected_except) {
   auto trace = push_transaction(make_transaction(std::move(actions)), keys);
   expect(trace, expected_except);
   return trace;
}

eosio::transaction_trace eosio::test_chain::transact(std::vector<action>&& actions, const char* expected_except) {
   return transact(std::move(actions), { default_priv_key }, expected_except);
}

[[nodiscard]]
std::optional<eosio::transaction_trace> eosio::test_chain::exec_deferred() {
   std::vector<char> bin;
   if (!::exec_deferred(id, [&](size_t size) {
          bin.resize(size);
          return bin.data();
       }))
      return {};
   return convert_from_bin<transaction_trace>(bin);
}

std::optional<eosio::test_chain::get_history_result> eosio::test_chain::get_history(uint32_t block_num) {
   std::optional<get_history_result> ret;
   auto                              size = ::get_history(id, block_num, nullptr, 0);
   if (!size)
      return ret;
   ret.emplace();
   ret->memory.resize(size);
   ::get_history(id, block_num, ret->memory.data(), size);
   ship_protocol::result r;
   (void)convert_from_bin(r, ret->memory);
   auto p = std::get_if<ship_protocol::get_blocks_result_v0>(&r);
   check(!!p, "test_chain::get_history: expected ship_protocol::get_blocks_result_v0");
   ret->result = *p;
   if (p->block) {
      ret->block.emplace();
      (void)from_bin(*ret->block, *p->block);
   }
   if (p->traces) {
      ret->traces.emplace();
      (void)from_bin(*ret->traces, *p->traces);
   }
   if (p->deltas) {
      ret->deltas.emplace();
      (void)from_bin(*ret->deltas, *p->deltas);
   }
   return ret;
}

eosio::transaction_trace eosio::test_chain::create_account(name ac, const public_key& pub_key,
                                                           const char* expected_except) {
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

eosio::transaction_trace eosio::test_chain::create_account(name ac, const char* expected_except) {
   return create_account(ac, default_pub_key, expected_except);
}

eosio::transaction_trace eosio::test_chain::create_code_account(name account, const public_key& pub_key, bool is_priv,
                                                                const char* expected_except) {
   tester_authority simple_auth{
      .threshold = 1,
      .keys      = { { pub_key, 1 } },
   };
   tester_authority code_auth{
      .threshold = 1,
      .keys      = { { pub_key, 1 } },
      .accounts  = { { { account, "eosio.code"_n }, 1 } },
   };
   return transact(
         {
               action{ { { "eosio"_n, "active"_n } },
                      "eosio"_n,
                       "newaccount"_n,
                       std::make_tuple("eosio"_n, account, simple_auth, code_auth) },
               action{ { { "eosio"_n, "active"_n } }, "eosio"_n, "setpriv"_n, std::make_tuple(account, is_priv) },
         },
         expected_except);
}

eosio::transaction_trace eosio::test_chain::create_code_account(name ac, const public_key& pub_key, const char* expected_except) {
   return create_code_account(ac, pub_key, false, expected_except);
}

eosio::transaction_trace eosio::test_chain::create_code_account(name ac, bool is_priv,
                                                         const char* expected_except) {
   return create_code_account(ac, default_pub_key, is_priv, expected_except);
}

eosio::transaction_trace eosio::test_chain::create_code_account(name ac, const char* expected_except) {
   return create_code_account(ac, default_pub_key, false, expected_except);
}

eosio::transaction_trace eosio::test_chain::set_code(name ac, const char* filename, const char* expected_except) {
   return transact({ action{ { { ac, "active"_n } },
                             "eosio"_n,
                             "setcode"_n,
                             std::make_tuple(ac, uint8_t{ 0 }, uint8_t{ 0 }, read_whole_file(filename)) } },
                   expected_except);
}

eosio::transaction_trace eosio::test_chain::create_token(name contract, name signer, name issuer, asset maxsupply,
                                                  const char* expected_except) {
   return transact(
         { action{ { { signer, "active"_n } }, contract, "create"_n, std::make_tuple(issuer, maxsupply) } },
         expected_except);
}

eosio::transaction_trace eosio::test_chain::issue(const name& contract, const name& issuer, const asset& amount,
                                                  const char* expected_except) {
   return transact({ action{ { { issuer, "active"_n } },
                             contract,
                             "issue"_n,
                             std::make_tuple(issuer, amount, std::string{ "issuing" }) } },
                   expected_except);
}

eosio::transaction_trace eosio::test_chain::transfer(const name& contract, const name& from, const name& to, const asset& amount,
                                              const std::string& memo, const char* expected_except) {

      return transact(
            { action{ { { from, "active"_n } }, contract, "transfer"_n, std::make_tuple(from, to, amount, memo) } },
            expected_except);
   }

eosio::transaction_trace eosio::test_chain::issue_and_transfer(const name& contract, const name& issuer, const name& to,
                                                        const asset& amount, const std::string& memo,
                                                        const char* expected_except) {
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

eosio::test_rodeos::test_rodeos() : id{ create_rodeos() } {}

eosio::test_rodeos::~test_rodeos() { destroy_rodeos(id); }

void eosio::test_rodeos::connect(test_chain& chain) {
   connected = &chain;
   connect_rodeos(id, chain.id);
}

void eosio::test_rodeos::add_filter(eosio::name name, const char* wasm_filename) {
   rodeos_add_filter(id, name.value, wasm_filename);
}

void eosio::test_rodeos::enable_queries(uint32_t max_console_size, uint32_t wasm_cache_size, uint64_t max_exec_time_ms,
                                        const char* contract_dir) {
   rodeos_enable_queries(id, max_console_size, wasm_cache_size, max_exec_time_ms, contract_dir ? contract_dir : "");
}

bool eosio::test_rodeos::sync_block() { return rodeos_sync_block(id); }

uint32_t eosio::test_rodeos::sync_blocks() {
   uint32_t n = 0;
   while (sync_block()) ++n;
   return n;
}

[[nodiscard]] eosio::transaction_trace
eosio::test_rodeos::push_transaction(const transaction& trx, const std::vector<private_key>& keys,
                                     const std::vector<std::vector<char>>& context_free_data,
                                     const std::vector<signature>&         signatures) {
   std::vector<char> packed_trx = pack(trx);
   std::vector<char> args;
   (void)convert_to_bin(packed_trx, args);
   (void)convert_to_bin(context_free_data, args);
   (void)convert_to_bin(signatures, args);
   (void)convert_to_bin(keys, args);
   std::vector<char> bin;
   rodeos_push_transaction(id, args.data(), args.data() + args.size(), [&](size_t size) {
      bin.resize(size);
      return bin.data();
   });
   return convert_from_bin<transaction_trace>(bin);
}

eosio::transaction_trace eosio::test_rodeos::transact(std::vector<action>&&           actions,
                                                      const std::vector<private_key>& keys,
                                                      const char*                     expected_except) {
   auto trace = push_transaction(connected->make_transaction(std::move(actions)), keys);
   expect_rodeos(trace, expected_except);
   return trace;
}

eosio::transaction_trace eosio::test_rodeos::transact(std::vector<action>&& actions, const char* expected_except) {
   return transact(std::move(actions), {}, expected_except);
}

uint32_t eosio::test_rodeos::get_num_pushed_data() {
    return rodeos_get_num_pushed_data(id);
}

std::vector<char> eosio::test_rodeos::get_pushed_data(uint32_t index) {
    size_t            len = rodeos_get_pushed_data(id, index, nullptr, 0);
    std::vector<char> result(len);
    rodeos_get_pushed_data(id, index, result.data(), len);
    return result;
}

std::string eosio::test_rodeos::get_pushed_data_str(uint32_t index) {
    size_t      len = rodeos_get_pushed_data(id, index, nullptr, 0);
    std::string result(len, 0);
    rodeos_get_pushed_data(id, index, result.data(), len);
    return result;
}

std::ostream& chain_types::operator<<(std::ostream& os, const account_auth_sequence& aas) {
   return os << eosio::convert_to_json(aas);
}

std::ostream& chain_types::operator<<(std::ostream& os, const account_delta& ad) {
   return os << eosio::convert_to_json(ad);
}

extern "C" {
   void send_inline(char *serialized_action, size_t size) {
      eosio::check(current_chain != nullptr, "Cannot send an action without a blockchain");
      current_chain->transact({ eosio::unpack<eosio::action>(serialized_action, size) });
   }
}
