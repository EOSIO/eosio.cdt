#include "../../../libraries/eosiolib/tester/eosio/chain_types.hpp"
#include <abieos_exception.hpp>
#include <eosio/chain/apply_context.hpp>
#include <eosio/chain/contract_table_objects.hpp>
#include <eosio/chain/controller.hpp>
#include <eosio/chain/transaction_context.hpp>
#include <eosio/chain/webassembly/common.hpp>
#include <eosio/vm/backend.hpp>
#include <fc/exception/exception.hpp>
#include <fc/io/json.hpp>
#include <stdio.h>

using namespace abieos::literals;
using namespace std::literals;

struct callbacks;
using backend_t = eosio::vm::backend<callbacks>;
using rhf_t     = eosio::vm::registered_host_functions<callbacks>;

const static int block_interval_ms = 500;
const static int block_interval_us = block_interval_ms * 1000;

struct assert_exception : std::exception {
   std::string msg;

   assert_exception(std::string&& msg) : msg(std::move(msg)) {}

   const char* what() const noexcept override { return msg.c_str(); }
};

struct intrinsic_context {
   eosio::chain::controller&                          control;
   eosio::chain::signed_transaction                   trx;
   std::unique_ptr<eosio::chain::transaction_context> trx_ctx;
   std::unique_ptr<eosio::chain::apply_context>       apply_context;

   intrinsic_context(eosio::chain::controller& control) : control{ control } {

      trx.actions.emplace_back();
      trx.actions.back().account = N(eosio.null);
      trx_ctx = std::make_unique<eosio::chain::transaction_context>(control, trx, trx.id(), fc::time_point::now());
      trx_ctx->init_for_implicit_trx(0);
      trx_ctx->exec();
      apply_context = std::make_unique<eosio::chain::apply_context>(control, *trx_ctx, 1, 0);
   }
};

struct test_chain {
   eosio::chain::private_key_type producer_key{ "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"s };
   std::unique_ptr<eosio::chain::controller::config> cfg;
   std::unique_ptr<eosio::chain::controller>         control;
   std::unique_ptr<intrinsic_context>                intr_ctx;

   test_chain() {
      std::string dir = "testchain-XXXXXX";
      if (mkdtemp(dir.data()) != dir.data())
         throw std::runtime_error("could not create directory " + dir);
      cfg                            = std::make_unique<eosio::chain::controller::config>();
      cfg->blocks_dir                = dir + "/blocks";
      cfg->state_dir                 = dir + "/state";
      cfg->contracts_console         = true;
      cfg->genesis.initial_timestamp = fc::time_point::from_iso_string("2020-01-01T00:00:00.000");

      control = std::make_unique<eosio::chain::controller>(*cfg);

      control->add_indices();
      control->startup([]() { return false; }, nullptr);

      start_block();
   }

   test_chain(const test_chain&) = delete;
   test_chain& operator=(const test_chain&) = delete;

   void mutating() { intr_ctx.reset(); }

   eosio::chain::apply_context& get_apply_context() {
      if (!intr_ctx) {
         start_if_needed();
         intr_ctx = std::make_unique<intrinsic_context>(*control);
      }
      return *intr_ctx->apply_context;
   }

   void start_block(int64_t skip_miliseconds = 0) {
      mutating();
      if (control->is_building_block())
         finish_block();
      control->start_block(control->head_block_time() + fc::microseconds(skip_miliseconds * 1000ll + block_interval_us),
                           0, {});
   }

   void start_if_needed() {
      mutating();
      if (!control->is_building_block())
         control->start_block(control->head_block_time() + fc::microseconds(block_interval_us), 0, {});
   }

   void finish_block() {
      start_if_needed();
      ilog("finish block ${n}", ("n", control->head_block_num()));
      control->finalize_block([&](eosio::chain::digest_type d) { return producer_key.sign(d); });
      control->commit_block();
   }
};

abieos::checksum256 convert(const eosio::chain::checksum_type& obj) {
   abieos::checksum256 result;
   static_assert(sizeof(result) == sizeof(obj));
   memcpy(&result, &obj, sizeof(result));
   return result;
}

chain_types::account_delta convert(const eosio::chain::account_delta& obj) {
   chain_types::account_delta result;
   result.account.value = obj.account.value;
   result.delta         = obj.delta;
   return result;
}

chain_types::action_receipt_v0 convert(const eosio::chain::action_receipt& obj) {
   chain_types::action_receipt_v0 result;
   result.receiver.value  = obj.receiver.value;
   result.act_digest      = convert(obj.act_digest);
   result.global_sequence = obj.global_sequence;
   result.recv_sequence   = obj.recv_sequence;
   for (auto& auth : obj.auth_sequence) result.auth_sequence.push_back({ abieos::name{ auth.first }, auth.second });
   result.code_sequence.value = obj.code_sequence.value;
   result.abi_sequence.value  = obj.abi_sequence.value;
   return result;
}

chain_types::action convert(const eosio::chain::action& obj) {
   chain_types::action result;
   result.account.value = obj.account.value;
   result.name.value    = obj.name.value;
   for (auto& auth : obj.authorization)
      result.authorization.push_back({ abieos::name{ auth.actor.value }, abieos::name{ auth.permission.value } });
   result.data = { obj.data.data(), obj.data.data() + obj.data.size() };
   return result;
}

chain_types::action_trace_v0 convert(const eosio::chain::action_trace& obj) {
   chain_types::action_trace_v0 result;
   result.action_ordinal.value         = obj.action_ordinal.value;
   result.creator_action_ordinal.value = obj.creator_action_ordinal.value;
   if (obj.receipt)
      result.receipt = convert(*obj.receipt);
   result.receiver.value = obj.receiver.value;
   result.act            = convert(obj.act);
   result.context_free   = obj.context_free;
   result.elapsed        = obj.elapsed.count();
   result.console        = obj.console;
   for (auto& delta : obj.account_ram_deltas) result.account_ram_deltas.push_back(convert(delta));
   if (obj.except)
      result.except = obj.except->to_string();
   if (obj.error_code)
      result.error_code = *obj.error_code;
   return result;
}

chain_types::transaction_trace_v0 convert(const eosio::chain::transaction_trace& obj) {
   chain_types::transaction_trace_v0 result{};
   result.id = convert(obj.id);
   if (obj.receipt) {
      result.status          = (chain_types::transaction_status)obj.receipt->status.value;
      result.cpu_usage_us    = obj.receipt->cpu_usage_us;
      result.net_usage_words = { obj.receipt->net_usage_words.value };
   } else {
      result.status = chain_types::transaction_status::hard_fail;
   }
   result.elapsed   = obj.elapsed.count();
   result.net_usage = obj.net_usage;
   result.scheduled = obj.scheduled;
   for (auto& at : obj.action_traces) result.action_traces.push_back(convert(at));
   if (obj.account_ram_delta)
      result.account_ram_delta = convert(*obj.account_ram_delta);
   if (obj.except)
      result.except = obj.except->to_string();
   if (obj.error_code)
      result.error_code = *obj.error_code;
   if (obj.failed_dtrx_trace)
      result.failed_dtrx_trace.push_back({ chain_types::recurse_transaction_trace{ convert(*obj.failed_dtrx_trace) } });
   return result;
}

struct contract_row {
   uint32_t             block_num   = {};
   bool                 present     = {};
   abieos::name         code        = {};
   abieos::name         scope       = {};
   abieos::name         table       = {};
   uint64_t             primary_key = {};
   abieos::name         payer       = {};
   abieos::input_buffer value       = {};
};

ABIEOS_REFLECT(contract_row) {
   ABIEOS_MEMBER(contract_row, block_num);
   ABIEOS_MEMBER(contract_row, present);
   ABIEOS_MEMBER(contract_row, code);
   ABIEOS_MEMBER(contract_row, scope);
   ABIEOS_MEMBER(contract_row, table);
   ABIEOS_MEMBER(contract_row, primary_key);
   ABIEOS_MEMBER(contract_row, payer);
   ABIEOS_MEMBER(contract_row, value);
}

struct file {
   FILE* f = nullptr;

   file(FILE* f = nullptr) : f(f) {}

   file(const file&) = delete;
   file(file&& src) { *this = std::move(src); }

   ~file() { close(); }

   file& operator=(const file&) = delete;

   file& operator=(file&& src) {
      close();
      this->f = src.f;
      src.f   = nullptr;
      return *this;
   }

   void close() {
      if (f)
         fclose(f);
      f = nullptr;
   }
};

namespace eosio { namespace vm {

   template <>
   struct wasm_type_converter<const char*> : linear_memory_access {
      auto from_wasm(void* ptr) { return (const char*)ptr; }
   };

   template <>
   struct wasm_type_converter<char*> : linear_memory_access {
      auto from_wasm(void* ptr) { return (char*)ptr; }
   };

   template <typename T>
   struct wasm_type_converter<T&> : linear_memory_access {
      auto from_wasm(uint32_t val) {
         EOS_VM_ASSERT(val != 0, wasm_memory_exception, "references cannot be created for null pointers");
         void* ptr = get_ptr(val);
         validate_ptr<T>(ptr, 1);
         return eosio::vm::aligned_ref_wrapper<T, alignof(T)>{ ptr };
      }
   };

}} // namespace eosio::vm

struct state {
   const char*                              wasm;
   eosio::vm::wasm_allocator&               wa;
   backend_t&                               backend;
   std::vector<char>                        args;
   std::vector<file>                        files;
   std::vector<std::unique_ptr<test_chain>> chains;
   std::optional<uint32_t>                  selected_chain_index;
};

struct push_trx_args {
   eosio::chain::bytes                         transaction;
   std::vector<eosio::chain::bytes>            context_free_data;
   std::vector<eosio::chain::signature_type>   signatures;
   std::vector<eosio::chain::private_key_type> keys;
};
FC_REFLECT(push_trx_args, (transaction)(context_free_data)(signatures)(keys))

#define DB_WRAPPERS_SIMPLE_SECONDARY(IDX, TYPE)                                                                        \
   int db_##IDX##_find_secondary(uint64_t code, uint64_t scope, uint64_t table, const TYPE& secondary,                 \
                                 uint64_t& primary) {                                                                  \
      return selected().IDX.find_secondary(code, scope, table, secondary, primary);                                    \
   }                                                                                                                   \
   int db_##IDX##_find_primary(uint64_t code, uint64_t scope, uint64_t table, TYPE& secondary, uint64_t primary) {     \
      return selected().IDX.find_primary(code, scope, table, secondary, primary);                                      \
   }                                                                                                                   \
   int db_##IDX##_lowerbound(uint64_t code, uint64_t scope, uint64_t table, TYPE& secondary, uint64_t& primary) {      \
      return selected().IDX.lowerbound_secondary(code, scope, table, secondary, primary);                              \
   }                                                                                                                   \
   int db_##IDX##_upperbound(uint64_t code, uint64_t scope, uint64_t table, TYPE& secondary, uint64_t& primary) {      \
      return selected().IDX.upperbound_secondary(code, scope, table, secondary, primary);                              \
   }                                                                                                                   \
   int db_##IDX##_end(uint64_t code, uint64_t scope, uint64_t table) {                                                 \
      return selected().IDX.end_secondary(code, scope, table);                                                         \
   }                                                                                                                   \
   int db_##IDX##_next(int iterator, uint64_t& primary) { return selected().IDX.next_secondary(iterator, primary); }   \
   int db_##IDX##_previous(int iterator, uint64_t& primary) {                                                          \
      return selected().IDX.previous_secondary(iterator, primary);                                                     \
   }

#define DB_WRAPPERS_ARRAY_SECONDARY(IDX, ARR_SIZE, ARR_ELEMENT_TYPE)                                                   \
   int db_##IDX##_find_secondary(uint64_t code, uint64_t scope, uint64_t table,                                        \
                                 eosio::chain::array_ptr<const ARR_ELEMENT_TYPE> data, size_t data_len,                \
                                 uint64_t& primary) {                                                                  \
      EOS_ASSERT(data_len == ARR_SIZE, eosio::chain::db_api_exception,                                                 \
                 "invalid size of secondary key array for " #IDX                                                       \
                 ": given ${given} bytes but expected ${expected} bytes",                                              \
                 ("given", data_len)("expected", ARR_SIZE));                                                           \
      return selected().IDX.find_secondary(code, scope, table, data, primary);                                         \
   }                                                                                                                   \
   int db_##IDX##_find_primary(uint64_t code, uint64_t scope, uint64_t table,                                          \
                               eosio::chain::array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len, uint64_t primary) {    \
      EOS_ASSERT(data_len == ARR_SIZE, eosio::chain::db_api_exception,                                                 \
                 "invalid size of secondary key array for " #IDX                                                       \
                 ": given ${given} bytes but expected ${expected} bytes",                                              \
                 ("given", data_len)("expected", ARR_SIZE));                                                           \
      return selected().IDX.find_primary(code, scope, table, data.value, primary);                                     \
   }                                                                                                                   \
   int db_##IDX##_lowerbound(uint64_t code, uint64_t scope, uint64_t table,                                            \
                             eosio::chain::array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len, uint64_t& primary) {     \
      EOS_ASSERT(data_len == ARR_SIZE, eosio::chain::db_api_exception,                                                 \
                 "invalid size of secondary key array for " #IDX                                                       \
                 ": given ${given} bytes but expected ${expected} bytes",                                              \
                 ("given", data_len)("expected", ARR_SIZE));                                                           \
      return selected().IDX.lowerbound_secondary(code, scope, table, data.value, primary);                             \
   }                                                                                                                   \
   int db_##IDX##_upperbound(uint64_t code, uint64_t scope, uint64_t table,                                            \
                             eosio::chain::array_ptr<ARR_ELEMENT_TYPE> data, size_t data_len, uint64_t& primary) {     \
      EOS_ASSERT(data_len == ARR_SIZE, eosio::chain::db_api_exception,                                                 \
                 "invalid size of secondary key array for " #IDX                                                       \
                 ": given ${given} bytes but expected ${expected} bytes",                                              \
                 ("given", data_len)("expected", ARR_SIZE));                                                           \
      return selected().IDX.upperbound_secondary(code, scope, table, data.value, primary);                             \
   }                                                                                                                   \
   int db_##IDX##_end(uint64_t code, uint64_t scope, uint64_t table) {                                                 \
      return selected().IDX.end_secondary(code, scope, table);                                                         \
   }                                                                                                                   \
   int db_##IDX##_next(int iterator, uint64_t& primary) { return selected().IDX.next_secondary(iterator, primary); }   \
   int db_##IDX##_previous(int iterator, uint64_t& primary) {                                                          \
      return selected().IDX.previous_secondary(iterator, primary);                                                     \
   }

#define DB_WRAPPERS_FLOAT_SECONDARY(IDX, TYPE)                                                                         \
   int db_##IDX##_find_secondary(uint64_t code, uint64_t scope, uint64_t table, const TYPE& secondary,                 \
                                 uint64_t& primary) {                                                                  \
      /* EOS_ASSERT(!softfloat_api::is_nan(secondary), transaction_exception, "NaN is not an allowed value for a       \
       * secondary key"); */                                                                                           \
      return selected().IDX.find_secondary(code, scope, table, secondary, primary);                                    \
   }                                                                                                                   \
   int db_##IDX##_find_primary(uint64_t code, uint64_t scope, uint64_t table, TYPE& secondary, uint64_t primary) {     \
      return selected().IDX.find_primary(code, scope, table, secondary, primary);                                      \
   }                                                                                                                   \
   int db_##IDX##_lowerbound(uint64_t code, uint64_t scope, uint64_t table, TYPE& secondary, uint64_t& primary) {      \
      /* EOS_ASSERT(!softfloat_api::is_nan(secondary), transaction_exception, "NaN is not an allowed value for a       \
       * secondary key"); */                                                                                           \
      return selected().IDX.lowerbound_secondary(code, scope, table, secondary, primary);                              \
   }                                                                                                                   \
   int db_##IDX##_upperbound(uint64_t code, uint64_t scope, uint64_t table, TYPE& secondary, uint64_t& primary) {      \
      /* EOS_ASSERT(!softfloat_api::is_nan(secondary), transaction_exception, "NaN is not an allowed value for a       \
       * secondary key"); */                                                                                           \
      return selected().IDX.upperbound_secondary(code, scope, table, secondary, primary);                              \
   }                                                                                                                   \
   int db_##IDX##_end(uint64_t code, uint64_t scope, uint64_t table) {                                                 \
      return selected().IDX.end_secondary(code, scope, table);                                                         \
   }                                                                                                                   \
   int db_##IDX##_next(int iterator, uint64_t& primary) { return selected().IDX.next_secondary(iterator, primary); }   \
   int db_##IDX##_previous(int iterator, uint64_t& primary) {                                                          \
      return selected().IDX.previous_secondary(iterator, primary);                                                     \
   }

struct callbacks {
   ::state& state;

   void check_bounds(const char* begin, const char* end) {
      if (begin > end)
         throw std::runtime_error("bad memory");
      // todo: check bounds
   }

   template <typename T>
   T unpack(const char* begin, const char* end) {
      fc::datastream<const char*> ds(begin, end - begin);
      T                           args;
      fc::raw::unpack(ds, args);
      return args;
   }

   template <typename T>
   T unpack_checked(const char* begin, const char* end) {
      check_bounds(begin, end);
      return unpack<T>(begin, end);
   }

   char* alloc(uint32_t cb_alloc_data, uint32_t cb_alloc, uint32_t size) {
      // todo: verify cb_alloc isn't in imports
      auto result = state.backend.get_context().execute_func_table(
            this, eosio::vm::interpret_visitor(state.backend.get_context()), cb_alloc, cb_alloc_data, size);
      if (!result || !result->is_a<eosio::vm::i32_const_t>())
         throw std::runtime_error("cb_alloc returned incorrect type");
      char* begin = state.wa.get_base_ptr<char>() + result->to_ui32();
      check_bounds(begin, begin + size);
      return begin;
   }

   template <typename T>
   void set_data(uint32_t cb_alloc_data, uint32_t cb_alloc, const T& data) {
      memcpy(alloc(cb_alloc_data, cb_alloc, data.size()), data.data(), data.size());
   }

   void abort() { throw std::runtime_error("called abort"); }

   void eosio_assert_message(bool test, const char* msg, size_t msg_len) {
      check_bounds(msg, msg + msg_len);
      if (!test)
         throw ::assert_exception(std::string(msg, msg_len));
   }

   void print_range(const char* begin, const char* end) {
      check_bounds(begin, end);
      std::cerr.write(begin, end - begin);
   }

   void get_args(uint32_t cb_alloc_data, uint32_t cb_alloc) { set_data(cb_alloc_data, cb_alloc, state.args); }

   bool reenter(const char* args_begin, const char* args_end, uint32_t f, uint32_t cb_alloc_data, uint32_t cb_alloc) {
      check_bounds(args_begin, args_end);
      try {
         // todo: verify cb isn't in imports
         eosio::vm::wasm_allocator wa;
         auto                      code = backend_t::read_wasm(this->state.wasm);
         backend_t                 backend(code);
         ::state                   state{ this->state.wasm, wa, backend, { args_begin, args_end } };
         callbacks                 cb{ state };
         backend.set_wasm_allocator(&wa);

         rhf_t::resolve(backend.get_module());
         backend.initialize(&cb);
         // backend(&cb, "env", "initialize"); // todo: needs change to eosio-cpp
         backend(&cb, "env", "start", f);
         return true;
      } catch (::assert_exception& e) {
         set_data(cb_alloc_data, cb_alloc, std::string_view{ std::string("assert failed: ") + e.what() });
         return false;
      } catch (std::exception& e) {
         set_data(cb_alloc_data, cb_alloc, std::string_view{ e.what() });
         return false;
      } catch (fc::exception& e) {
         set_data(cb_alloc_data, cb_alloc, std::string_view{ e.to_string() });
         return false;
      } catch (...) {
         set_data(cb_alloc_data, cb_alloc, std::string_view{ "unknown exception" });
         return false;
      }
   }

   int32_t open_file(const char* filename_begin, const char* filename_end, const char* mode_begin,
                     const char* mode_end) {
      check_bounds(filename_begin, filename_end);
      check_bounds(mode_begin, mode_end);
      file f = fopen(std::string{ filename_begin, filename_end }.c_str(), std::string{ mode_begin, mode_end }.c_str());
      if (!f.f)
         return -1;
      state.files.push_back(std::move(f));
      return state.files.size() - 1;
   }

   file& assert_file(int32_t file_index) {
      if (file_index < 0 || file_index >= state.files.size() || !state.files[file_index].f)
         throw std::runtime_error("file is not opened");
      return state.files[file_index];
   }

   void close_file(int32_t file_index) { assert_file(file_index).close(); }

   bool write_file(int32_t file_index, const char* content_begin, const char* content_end) {
      check_bounds(content_begin, content_end);
      auto& f = assert_file(file_index);
      return fwrite(content_begin, content_end - content_begin, 1, f.f) == 1;
   }

   bool read_whole_file(const char* filename_begin, const char* filename_end, uint32_t cb_alloc_data,
                        uint32_t cb_alloc) {
      check_bounds(filename_begin, filename_end);
      file f = fopen(std::string{ filename_begin, filename_end }.c_str(), "r");
      if (!f.f)
         return false;
      if (fseek(f.f, 0, SEEK_END))
         return false;
      auto size = ftell(f.f);
      if (size < 0 || (long)(uint32_t)size != size)
         return false;
      if (fseek(f.f, 0, SEEK_SET))
         return false;
      std::vector<char> buf(size);
      if (fread(buf.data(), size, 1, f.f) != 1)
         return false;
      set_data(cb_alloc_data, cb_alloc, buf);
      return true;
   }

   int32_t execute(const char* command_begin, const char* command_end) {
      check_bounds(command_begin, command_end);
      return system(std::string{ command_begin, command_end }.c_str());
   }

   test_chain& assert_chain(uint32_t chain) {
      if (chain >= state.chains.size() || !state.chains[chain])
         throw std::runtime_error("chain does not exist or was destroyed");
      return *state.chains[chain];
   }

   uint32_t create_chain() {
      state.chains.push_back(std::make_unique<test_chain>());
      if (state.chains.size() == 1)
         state.selected_chain_index = 0;
      return state.chains.size() - 1;
   }

   void destroy_chain(uint32_t chain) {
      assert_chain(chain);
      state.chains[chain].reset();
   }

   void start_block(uint32_t chain_index, int64_t skip_miliseconds) {
      assert_chain(chain_index).start_block(skip_miliseconds);
   }

   void finish_block(uint32_t chain_index) { assert_chain(chain_index).finish_block(); }

   void get_head_block_info(uint32_t chain_index, uint32_t cb_alloc_data, uint32_t cb_alloc) {
      auto&                   chain = assert_chain(chain_index);
      chain_types::block_info info;
      info.block_num      = chain.control->head_block_num();
      info.block_id       = convert(chain.control->head_block_id());
      info.timestamp.slot = chain.control->head_block_state()->header.timestamp.slot;
      set_data(cb_alloc_data, cb_alloc, abieos::native_to_bin(info));
   }

   void push_transaction(uint32_t chain_index, const char* args_begin, const char* args_end, uint32_t cb_alloc_data,
                         uint32_t cb_alloc) {
      auto                             args        = unpack_checked<push_trx_args>(args_begin, args_end);
      auto                             transaction = unpack<eosio::chain::transaction>(args.transaction.data(),
                                                           args.transaction.data() + args.transaction.size());
      eosio::chain::signed_transaction signed_trx{ std::move(transaction), std::move(args.signatures),
                                                   std::move(args.context_free_data) };
      auto&                            chain = assert_chain(chain_index);
      chain.mutating();
      chain.start_if_needed();
      for (auto& key : args.keys) signed_trx.sign(key, chain.control->get_chain_id());
      auto mtrx =
            std::make_shared<eosio::chain::transaction_metadata>(signed_trx, eosio::chain::packed_transaction::none);
      eosio::chain::transaction_metadata::start_recover_keys(
            mtrx, chain.control->get_thread_pool(), chain.control->get_chain_id(), fc::microseconds::maximum());
      auto result = chain.control->push_transaction(mtrx, fc::time_point::maximum(), 2000);
      // ilog("${r}", ("r", fc::json::to_pretty_string(result)));
      set_data(cb_alloc_data, cb_alloc, abieos::native_to_bin(chain_types::transaction_trace{ convert(*result) }));
   }

   void query_database(uint32_t chain_index, const char* req_begin, const char* req_end, uint32_t cb_alloc_data,
                       uint32_t cb_alloc) {
      auto& chain = assert_chain(chain_index);
      check_bounds(req_begin, req_end);
      abieos::input_buffer query_bin{ req_begin, req_end };
      abieos::name         query_name;
      abieos::bin_to_native(query_name, query_bin);
      if (query_name == "cr.ctsp"_n)
         return set_data(cb_alloc_data, cb_alloc, query_contract_row_range_code_table_scope_pk(chain, query_bin));
      throw std::runtime_error("query_database: unknown query: " + (std::string)query_name);
   }

   std::vector<char> query_contract_row_range_code_table_scope_pk(test_chain& chain, abieos::input_buffer query_bin) {
      auto snapshot_block    = abieos::bin_to_native<uint32_t>(query_bin);
      auto first_code        = eosio::chain::name{ abieos::bin_to_native<abieos::name>(query_bin).value };
      auto first_table       = eosio::chain::name{ abieos::bin_to_native<abieos::name>(query_bin).value };
      auto first_scope       = eosio::chain::name{ abieos::bin_to_native<abieos::name>(query_bin).value };
      auto first_primary_key = abieos::bin_to_native<uint64_t>(query_bin);
      auto last_code         = eosio::chain::name{ abieos::bin_to_native<abieos::name>(query_bin).value };
      auto last_table        = eosio::chain::name{ abieos::bin_to_native<abieos::name>(query_bin).value };
      auto last_scope        = eosio::chain::name{ abieos::bin_to_native<abieos::name>(query_bin).value };
      auto last_primary_key  = abieos::bin_to_native<uint64_t>(query_bin);
      auto max_results       = abieos::bin_to_native<uint32_t>(query_bin);

      if (first_code != last_code)
         throw std::runtime_error("query_database: first.code != last.code");
      if (first_table != last_table)
         throw std::runtime_error("query_database: first.table != last.table");

      auto&       db          = chain.control->db();
      const auto& table_index = db.get_index<eosio::chain::table_id_multi_index, eosio::chain::by_code_scope_table>();
      const auto& kv_index    = db.get_index<eosio::chain::key_value_index, eosio::chain::by_scope_primary>();

      std::vector<std::vector<char>> rows;
      for (auto table_it = table_index.lower_bound(std::make_tuple(first_code, first_scope, eosio::chain::name()));
           table_it != table_index.end(); ++table_it) {
         if (table_it->code != first_code || table_it->scope > last_scope || rows.size() >= max_results)
            break;
         if (table_it->table != first_table)
            continue;
         for (auto kv_it = kv_index.lower_bound(std::make_tuple(table_it->id, first_primary_key));
              kv_it != kv_index.end(); ++kv_it) {
            if (kv_it->t_id != table_it->id || kv_it->primary_key > last_primary_key || rows.size() >= max_results)
               break;
            rows.emplace_back(abieos::native_to_bin(
                  contract_row{ uint32_t(0),
                                bool(true),
                                abieos::name{ table_it->code.value },
                                abieos::name{ table_it->scope.value },
                                abieos::name{ table_it->table.value },
                                kv_it->primary_key,
                                abieos::name{ kv_it->payer.value },
                                { kv_it->value.data(), kv_it->value.data() + kv_it->value.size() } }));
         };
      }
      return abieos::native_to_bin(rows);
   } // query_contract_row_range_code_table_scope_pk

   void select_chain_for_db(uint32_t chain_index) {
      assert_chain(chain_index);
      state.selected_chain_index = chain_index;
   }

   eosio::chain::apply_context& selected() {
      if (!state.selected_chain_index || *state.selected_chain_index >= state.chains.size() ||
          !state.chains[*state.selected_chain_index])
         throw std::runtime_error("select_chain_for_db() must be called before using multi_index");
      return state.chains[*state.selected_chain_index]->get_apply_context();
   }

   // clang-format off
    int db_get_i64(int iterator, char* buffer, size_t buffer_size)                      {return selected().db_get_i64(iterator, buffer, buffer_size);}
    int db_next_i64(int iterator, uint64_t& primary)                                    {return selected().db_next_i64(iterator, primary);}
    int db_previous_i64(int iterator, uint64_t& primary)                                {return selected().db_previous_i64(iterator, primary);}
    int db_find_i64(uint64_t code, uint64_t scope, uint64_t table, uint64_t id)         {return selected().db_find_i64(code, scope, table, id);}
    int db_lowerbound_i64(uint64_t code, uint64_t scope, uint64_t table, uint64_t id)   {return selected().db_lowerbound_i64(code, scope, table, id);}
    int db_upperbound_i64(uint64_t code, uint64_t scope, uint64_t table, uint64_t id)   {return selected().db_upperbound_i64(code, scope, table, id);}
    int db_end_i64(uint64_t code, uint64_t scope, uint64_t table)                       {return selected().db_end_i64(code, scope, table);}

    DB_WRAPPERS_SIMPLE_SECONDARY(idx64,  uint64_t)
    DB_WRAPPERS_SIMPLE_SECONDARY(idx128, unsigned __int128)
    DB_WRAPPERS_ARRAY_SECONDARY(idx256, 2, unsigned __int128)
    DB_WRAPPERS_FLOAT_SECONDARY(idx_double, float64_t)
    DB_WRAPPERS_FLOAT_SECONDARY(idx_long_double, float128_t)
   // clang-format on
}; // callbacks

#define DB_REGISTER_SECONDARY(IDX)                                                                                     \
   rhf_t::add<callbacks, &callbacks::db_##IDX##_find_secondary, eosio::vm::wasm_allocator>("env", "db_" #IDX           \
                                                                                                  "_find_secondary");  \
   rhf_t::add<callbacks, &callbacks::db_##IDX##_find_primary, eosio::vm::wasm_allocator>("env",                        \
                                                                                         "db_" #IDX "_find_primary");  \
   rhf_t::add<callbacks, &callbacks::db_##IDX##_lowerbound, eosio::vm::wasm_allocator>("env",                          \
                                                                                       "db_" #IDX "_lowerbound");      \
   rhf_t::add<callbacks, &callbacks::db_##IDX##_upperbound, eosio::vm::wasm_allocator>("env",                          \
                                                                                       "db_" #IDX "_upperbound");      \
   rhf_t::add<callbacks, &callbacks::db_##IDX##_end, eosio::vm::wasm_allocator>("env", "db_" #IDX "_end");             \
   rhf_t::add<callbacks, &callbacks::db_##IDX##_next, eosio::vm::wasm_allocator>("env", "db_" #IDX "_next");           \
   rhf_t::add<callbacks, &callbacks::db_##IDX##_previous, eosio::vm::wasm_allocator>("env", "db_" #IDX "_previous");

void register_callbacks() {
   rhf_t::add<callbacks, &callbacks::abort, eosio::vm::wasm_allocator>("env", "abort");
   rhf_t::add<callbacks, &callbacks::eosio_assert_message, eosio::vm::wasm_allocator>("env", "eosio_assert_message");
   rhf_t::add<callbacks, &callbacks::print_range, eosio::vm::wasm_allocator>("env", "print_range");
   rhf_t::add<callbacks, &callbacks::get_args, eosio::vm::wasm_allocator>("env", "get_args");
   rhf_t::add<callbacks, &callbacks::reenter, eosio::vm::wasm_allocator>("env", "reenter");
   rhf_t::add<callbacks, &callbacks::open_file, eosio::vm::wasm_allocator>("env", "open_file");
   rhf_t::add<callbacks, &callbacks::close_file, eosio::vm::wasm_allocator>("env", "close_file");
   rhf_t::add<callbacks, &callbacks::write_file, eosio::vm::wasm_allocator>("env", "write_file");
   rhf_t::add<callbacks, &callbacks::read_whole_file, eosio::vm::wasm_allocator>("env", "read_whole_file");
   rhf_t::add<callbacks, &callbacks::execute, eosio::vm::wasm_allocator>("env", "execute");
   rhf_t::add<callbacks, &callbacks::create_chain, eosio::vm::wasm_allocator>("env", "create_chain");
   rhf_t::add<callbacks, &callbacks::destroy_chain, eosio::vm::wasm_allocator>("env", "destroy_chain");
   rhf_t::add<callbacks, &callbacks::start_block, eosio::vm::wasm_allocator>("env", "start_block");
   rhf_t::add<callbacks, &callbacks::finish_block, eosio::vm::wasm_allocator>("env", "finish_block");
   rhf_t::add<callbacks, &callbacks::get_head_block_info, eosio::vm::wasm_allocator>("env", "get_head_block_info");
   rhf_t::add<callbacks, &callbacks::push_transaction, eosio::vm::wasm_allocator>("env", "push_transaction");
   rhf_t::add<callbacks, &callbacks::query_database, eosio::vm::wasm_allocator>("env", "query_database_chain");
   rhf_t::add<callbacks, &callbacks::select_chain_for_db, eosio::vm::wasm_allocator>("env", "select_chain_for_db");

   rhf_t::add<callbacks, &callbacks::db_get_i64, eosio::vm::wasm_allocator>("env", "db_get_i64");
   rhf_t::add<callbacks, &callbacks::db_next_i64, eosio::vm::wasm_allocator>("env", "db_next_i64");
   rhf_t::add<callbacks, &callbacks::db_previous_i64, eosio::vm::wasm_allocator>("env", "db_previous_i64");
   rhf_t::add<callbacks, &callbacks::db_find_i64, eosio::vm::wasm_allocator>("env", "db_find_i64");
   rhf_t::add<callbacks, &callbacks::db_lowerbound_i64, eosio::vm::wasm_allocator>("env", "db_lowerbound_i64");
   rhf_t::add<callbacks, &callbacks::db_upperbound_i64, eosio::vm::wasm_allocator>("env", "db_upperbound_i64");
   rhf_t::add<callbacks, &callbacks::db_end_i64, eosio::vm::wasm_allocator>("env", "db_end_i64");
   DB_REGISTER_SECONDARY(idx64)
   DB_REGISTER_SECONDARY(idx128)
   // DB_REGISTER_SECONDARY(idx256)
   DB_REGISTER_SECONDARY(idx_double)
   DB_REGISTER_SECONDARY(idx_long_double)
}

static void run(const char* wasm, const std::vector<std::string>& args) {
   eosio::vm::wasm_allocator wa;
   auto                      code = backend_t::read_wasm(wasm);
   backend_t                 backend(code);
   ::state                   state{ wasm, wa, backend, abieos::native_to_bin(args) };
   callbacks                 cb{ state };
   backend.set_wasm_allocator(&wa);

   rhf_t::resolve(backend.get_module());
   backend.initialize(&cb);
   // backend(&cb, "env", "initialize"); // todo: needs change to eosio-cpp
   backend(&cb, "env", "start", 0);
}

const char usage[] = "usage: tester [-h or --help] [-v or --verbose] file.wasm [args for wasm]\n";

int main(int argc, char* argv[]) {
   fc::logger::get(DEFAULT_LOGGER).set_log_level(fc::log_level::off);

   bool show_usage = false;
   bool error      = false;
   int  next_arg   = 1;
   while (next_arg < argc && argv[next_arg][0] == '-') {
      if (!strcmp(argv[next_arg], "-h") || !strcmp(argv[next_arg], "--help"))
         show_usage = true;
      else if (!strcmp(argv[next_arg], "-v") || !strcmp(argv[next_arg], "--verbose"))
         fc::logger::get(DEFAULT_LOGGER).set_log_level(fc::log_level::debug);
      else {
         std::cerr << "unknown option: " << argv[next_arg] << "\n";
         error = true;
      }
      ++next_arg;
   }
   if (next_arg >= argc)
      error = true;
   if (show_usage || error) {
      std::cerr << usage;
      return error;
   }
   try {
      std::vector<std::string> args{ argv + next_arg + 1, argv + argc };
      register_callbacks();
      run(argv[next_arg], args);
      return 0;
   } catch (::assert_exception& e) {
      std::cerr << "assert failed: " << e.what() << "\n";
   } catch (eosio::vm::exception& e) {
      std::cerr << "vm::exception: " << e.detail() << "\n";
   } catch (std::exception& e) { std::cerr << "std::exception: " << e.what() << "\n"; } catch (fc::exception& e) {
      std::cerr << "fc::exception: " << e.to_string() << "\n";
   }
   return 1;
}
