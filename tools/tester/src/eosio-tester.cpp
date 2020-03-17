#include "../../../libraries/eosiolib/tester/eosio/chain_types.hpp"
#include <eosio/chain/apply_context.hpp>
#include <eosio/chain/contract_table_objects.hpp>
#include <eosio/chain/controller.hpp>
#include <eosio/chain/generated_transaction_object.hpp>
#include <eosio/chain/transaction_context.hpp>
#include <eosio/chain/webassembly/common.hpp>
#include <eosio/eosio_outcome.hpp>
#include <eosio/from_bin.hpp>
#include <eosio/history-tools/embedded_rodeos.hpp>
#include <eosio/state_history/create_deltas.hpp>
#include <eosio/state_history/serialization.hpp>
#include <eosio/state_history/trace_converter.hpp>
#include <eosio/vm/backend.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/crypto/sha1.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/sha512.hpp>
#include <fc/exception/exception.hpp>
#include <fc/io/json.hpp>
#include <stdio.h>

using namespace eosio::literals;
using namespace std::literals;
using boost::signals2::scoped_connection;
using eosio::check;
using eosio::convert_to_bin;
using eosio::chain::block_state_ptr;
using eosio::chain::builtin_protocol_feature_t;
using eosio::chain::digest_type;
using eosio::chain::kv_bad_db_id;
using eosio::chain::kv_bad_iter;
using eosio::chain::kv_context;
using eosio::chain::kv_iterator;
using eosio::chain::kvdisk_id;
using eosio::chain::kvram_id;
using eosio::chain::protocol_feature_exception;
using eosio::chain::protocol_feature_set;
using eosio::chain::signed_transaction;
using eosio::chain::transaction_trace_ptr;
using eosio::state_history::block_position;
using eosio::state_history::create_deltas;
using eosio::state_history::get_blocks_result_v0;
using eosio::state_history::state_result;
using eosio::state_history::trace_converter;

struct callbacks;
using backend_t = eosio::vm::backend<callbacks, eosio::vm::jit>;
using rhf_t     = eosio::vm::registered_host_functions<callbacks>;

inline constexpr int      block_interval_ms   = 500;
inline constexpr int      block_interval_us   = block_interval_ms * 1000;
inline constexpr uint32_t billed_cpu_time_use = 2000;

// Handle eosio version differences
namespace {
template<typename T>
auto to_uint64_t(T n) -> std::enable_if_t<std::is_same_v<T, eosio::chain::name>, decltype(n.value)> { return n.value; }
template<typename T>
auto to_uint64_t(T n) -> std::enable_if_t<std::is_same_v<T, eosio::chain::name>, decltype(n.to_uint64_t())> { return n.to_uint64_t(); }

template<typename C, typename F0, typename F1, typename G>
auto do_startup(C&& control, F0&&, F1&& f1, G&&) -> std::enable_if_t<std::is_constructible_v<std::decay_t<decltype(*control)>, eosio::chain::controller::config, protocol_feature_set>> {
   return control->startup([]() { return false; }, nullptr);
}
template<typename C, typename F0, typename F1, typename G>
auto do_startup(C&& control, F0&&, F1&& f1, G&& genesis) -> decltype(control->startup(f1, genesis)) {
   return control->startup([]() { return false; }, genesis);
}
template<typename C, typename F0, typename F1, typename G>
auto do_startup(C&& control, F0&& f0, F1&& f1, G&& genesis) -> decltype(control->startup(f0, f1, genesis)) {
   return control->startup(f0, f1, genesis);
}
}

struct assert_exception : std::exception {
   std::string msg;

   assert_exception(std::string&& msg) : msg(std::move(msg)) {}

   const char* what() const noexcept override { return msg.c_str(); }
};

// HACK: UB.  Unfortunately, I can't think of a way to allow a transaction_context
// to be constructed outside of controller in 2.0 that doesn't have undefined behavior.
// A better solution would be to factor database access out of apply_context, but
// that can't really be backported to 2.0 at this point.
namespace {
   struct __attribute__((__may_alias__)) xxx_transaction_checktime_timer {
      std::atomic_bool& expired;
      eosio::chain::platform_timer& _timer;
   };
   struct transaction_checktime_factory {
      eosio::chain::platform_timer timer;
      std::atomic_bool expired;
      eosio::chain::transaction_checktime_timer get() {
         xxx_transaction_checktime_timer result{expired, timer};
         return std::move(*reinterpret_cast<eosio::chain::transaction_checktime_timer*>(&result));
      }
   };
};

template<typename ApplyContext, typename Enable=void>
struct intrinsic_context_impl {
   eosio::chain::controller&                          control;
   eosio::chain::signed_transaction                   trx;
   std::unique_ptr<eosio::chain::transaction_context> trx_ctx;
   std::unique_ptr<eosio::chain::apply_context>       apply_context;

   intrinsic_context_impl(eosio::chain::controller& control) : control{ control } {
      static transaction_checktime_factory xxx_timer;

      trx.actions.emplace_back();
      trx.actions.back().account = N(eosio.null);
      trx.actions.back().authorization.push_back({N(eosio), N(active)});
      trx_ctx = std::make_unique<eosio::chain::transaction_context>(control, trx, trx.id(), xxx_timer.get(), fc::time_point::now());
      trx_ctx->init_for_implicit_trx(0);
      trx_ctx->exec();
      apply_context = std::make_unique<eosio::chain::apply_context>(control, *trx_ctx, 1, 0);
      apply_context->exec_one();
   }
};

/*
template<typename ApplyContext>
struct intrinsic_context_impl<ApplyContext, std::void_t<typename ApplyContext::primary_index_read_only>> :
   ApplyContext::primary_index_read_only
{
   typename ApplyContext::template generic_index_read_only<eosio::chain::index64_object>                            idx64;
   typename ApplyContext::template generic_index_read_only<eosio::chain::index128_object>                           idx128;
   typename ApplyContext::template generic_index_read_only<eosio::chain::index256_object,
                                                          eosio::chain::uint128_t*, const eosio::chain::uint128_t*> idx256;
   typename ApplyContext::template generic_index_read_only<eosio::chain::index_double_object>                       idx_double;
   typename ApplyContext::template generic_index_read_only<eosio::chain::index_long_double_object>                  idx_long_double;

   std::unique_ptr<kv_context>                                    kv_ram;
   std::unique_ptr<kv_context>                                    kv_disk;
   std::vector<std::unique_ptr<kv_iterator>>                      kv_iterators;
   std::vector<size_t>                                            kv_destroyed_iterators;
   intrinsic_context_impl* apply_context;

   intrinsic_context_impl(eosio::chain::controller& control) :
      ApplyContext::primary_index_read_only{control.db()},
      idx64{control.db()},
      idx128{control.db()},
      idx256{control.db()},
      idx_double{control.db()},
      idx_long_double{control.db()},
      apply_context{this}
   {}
};
*/

using intrinsic_context = intrinsic_context_impl<eosio::chain::apply_context>;

protocol_feature_set make_protocol_feature_set() {
   protocol_feature_set                                        pfs;
   std::map<builtin_protocol_feature_t, optional<digest_type>> visited_builtins;

   std::function<digest_type(builtin_protocol_feature_t)> add_builtins =
         [&pfs, &visited_builtins, &add_builtins](builtin_protocol_feature_t codename) -> digest_type {
      auto res = visited_builtins.emplace(codename, optional<digest_type>());
      if (!res.second) {
         EOS_ASSERT(res.first->second, protocol_feature_exception,
                    "invariant failure: cycle found in builtin protocol feature dependencies");
         return *res.first->second;
      }

      auto f = protocol_feature_set::make_default_builtin_protocol_feature(
            codename, [&add_builtins](builtin_protocol_feature_t d) { return add_builtins(d); });

      const auto& pf    = pfs.add_feature(f);
      res.first->second = pf.feature_digest;

      return pf.feature_digest;
   };

   for (const auto& p : eosio::chain::builtin_protocol_feature_codenames) { add_builtins(p.first); }

   return pfs;
}

struct test_chain;

struct test_chain_ref {
   test_chain* chain = {};

   test_chain_ref() = default;
   test_chain_ref(test_chain&);
   test_chain_ref(const test_chain_ref&) = delete;
   ~test_chain_ref();

   test_chain_ref& operator=(test_chain_ref&&);
};

struct test_chain {
   eosio::chain::private_key_type producer_key{ "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"s };

   fc::temp_directory                                dir;
   std::unique_ptr<eosio::chain::controller::config> cfg;
   std::unique_ptr<eosio::chain::controller>         control;
   fc::optional<scoped_connection>                   applied_transaction_connection;
   fc::optional<scoped_connection>                   accepted_block_connection;
   trace_converter                                   trace_converter;
   fc::optional<block_position>                      prev_block;
   std::map<uint32_t, std::vector<char>>             history;
   std::unique_ptr<intrinsic_context>                intr_ctx;
   std::set<test_chain_ref*>                         refs;

   test_chain() {
      eosio::chain::genesis_state genesis;
      genesis.initial_timestamp = fc::time_point::from_iso_string("2020-01-01T00:00:00.000");
      cfg                       = std::make_unique<eosio::chain::controller::config>();
      cfg->blocks_dir           = dir.path() / "blocks";
      cfg->state_dir            = dir.path() / "state";
      cfg->contracts_console    = true;

      control =
            std::make_unique<eosio::chain::controller>(*cfg, make_protocol_feature_set(), genesis.compute_chain_id());
      control->add_indices();

      applied_transaction_connection.emplace(control->applied_transaction.connect(
            [&](std::tuple<const transaction_trace_ptr&, const signed_transaction&> t) {
               on_applied_transaction(std::get<0>(t), std::get<1>(t));
            }));
      accepted_block_connection.emplace(
            control->accepted_block.connect([&](const block_state_ptr& p) { on_accepted_block(p); }));

      do_startup(
            control, [] {}, [] { return false; }, genesis);
      control->start_block(control->head_block_time() + fc::microseconds(block_interval_us), 0,
                           { *control->get_protocol_feature_manager().get_builtin_digest(
                                 eosio::chain::builtin_protocol_feature_t::preactivate_feature) });
   }

   test_chain(const test_chain&) = delete;
   test_chain& operator=(const test_chain&) = delete;

   ~test_chain() {
      for (auto* ref : refs) ref->chain = nullptr;
   }

   void on_applied_transaction(const transaction_trace_ptr& p, const signed_transaction& t) {
      trace_converter.add_transaction(p, t);
   }

   void on_accepted_block(const block_state_ptr& block_state) {
      auto traces_bin = trace_converter.pack(control->db(), false, block_state);
      auto deltas_bin = fc::raw::pack(create_deltas(control->db(), !prev_block));

      get_blocks_result_v0 message;
      message.head = block_position{ control->head_block_num(), control->head_block_id() };
      message.last_irreversible =
            block_position{ control->last_irreversible_block_num(), control->last_irreversible_block_id() };
      message.this_block = block_position{ block_state->block->block_num(), block_state->block->id() };
      message.prev_block = prev_block;
      message.traces     = std::move(traces_bin);
      message.deltas     = std::move(deltas_bin);

      prev_block                         = message.this_block;
      history[control->head_block_num()] = fc::raw::pack(state_result{message});
   }

   void mutating() { intr_ctx.reset(); }

   auto& get_apply_context() {
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
                           0);
   }

   void start_if_needed() {
      mutating();
      if (!control->is_building_block())
         control->start_block(control->head_block_time() + fc::microseconds(block_interval_us), 0, {});
   }

   void finish_block() {
      start_if_needed();
      ilog("finish block ${n}", ("n", control->head_block_num()));
      control->finalize_block([&](eosio::chain::digest_type d) { return std::vector{producer_key.sign(d)}; });
      control->commit_block();
   }
};

test_chain_ref::test_chain_ref(test_chain& chain) {
   chain.refs.insert(this);
   this->chain = &chain;
}

test_chain_ref::~test_chain_ref() {
   if (chain)
      chain->refs.erase(this);
}

test_chain_ref& test_chain_ref::operator=(test_chain_ref&& rhs) { std::swap(this->chain, rhs.chain); return *this; }

struct test_rodeos {
   fc::temp_directory                        dir;
   embedded_rodeos::context                  context;
   std::optional<embedded_rodeos::partition> partition;
   std::optional<embedded_rodeos::snapshot>  write_snapshot;
   std::optional<embedded_rodeos::filter>    filter;
   test_chain_ref                            chain;
   uint32_t                                  next_block = 0;

   test_rodeos() {
      context.open_db(dir.path().string().c_str(), true);
      partition.emplace(context, "", 0);
      write_snapshot.emplace(partition->obj, true);
   }
};

eosio::checksum256 convert(const eosio::chain::checksum_type& obj) {
   std::array<uint8_t, 32> bytes;
   static_assert(bytes.size() == sizeof(obj));
   memcpy(bytes.data(), &obj, bytes.size());
   return eosio::checksum256(bytes);
}

chain_types::account_delta convert(const eosio::chain::account_delta& obj) {
   chain_types::account_delta result;
   result.account.value = to_uint64_t(obj.account);
   result.delta         = obj.delta;
   return result;
}

chain_types::action_receipt_v0 convert(const eosio::chain::action_receipt& obj) {
   chain_types::action_receipt_v0 result;
   result.receiver.value  = to_uint64_t(obj.receiver);
   result.act_digest      = convert(obj.act_digest);
   result.global_sequence = obj.global_sequence;
   result.recv_sequence   = obj.recv_sequence;
   for (auto& auth : obj.auth_sequence) result.auth_sequence.push_back({ eosio::name{ to_uint64_t(auth.first) }, auth.second });
   result.code_sequence.value = obj.code_sequence.value;
   result.abi_sequence.value  = obj.abi_sequence.value;
   return result;
}

chain_types::action convert(const eosio::chain::action& obj) {
   chain_types::action result;
   result.account.value = to_uint64_t(obj.account);
   result.name.value    = to_uint64_t(obj.name);
   for (auto& auth : obj.authorization)
      result.authorization.push_back({ eosio::name{ to_uint64_t(auth.actor) }, eosio::name{ to_uint64_t(auth.permission) } });
   result.data = { obj.data.data(), obj.data.data() + obj.data.size() };
   return result;
}

chain_types::action_trace_v1 convert(const eosio::chain::action_trace& obj) {
   chain_types::action_trace_v1 result;
   result.action_ordinal.value         = obj.action_ordinal.value;
   result.creator_action_ordinal.value = obj.creator_action_ordinal.value;
   if (obj.receipt)
      result.receipt = convert(*obj.receipt);
   result.receiver.value = to_uint64_t(obj.receiver);
   result.act            = convert(obj.act);
   result.context_free   = obj.context_free;
   result.elapsed        = obj.elapsed.count();
   result.console        = obj.console;
   for (auto& delta : obj.account_ram_deltas) result.account_ram_deltas.push_back(convert(delta));
   for (auto& delta : obj.account_disk_deltas) result.account_disk_deltas.push_back(convert(delta));
   if (obj.except)
      result.except = obj.except->to_string();
   if (obj.error_code)
      result.error_code = *obj.error_code;
   if (obj.return_value)
      result.return_value.emplace(obj.return_value->data(), obj.return_value->size());
   return result;
}

chain_types::transaction_trace_v0 convert(const eosio::chain::transaction_trace& obj) {
   chain_types::transaction_trace_v0 result{};
   result.id = convert(obj.id);
   if (obj.receipt) {
      result.status          = (chain_types::transaction_status)obj.receipt->status.value;
      result.cpu_usage_us    = obj.receipt->cpu_usage_us;
      result.net_usage_words = obj.receipt->net_usage_words.value;
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
      result.failed_dtrx_trace.push_back({ convert(*obj.failed_dtrx_trace) });
   return result;
}

struct contract_row {
   uint32_t             block_num   = {};
   bool                 present     = {};
   eosio::name          code        = {};
   eosio::name          scope       = {};
   eosio::name          table       = {};
   uint64_t             primary_key = {};
   eosio::name          payer       = {};
   eosio::input_stream  value       = {};
};

EOSIO_REFLECT(contract_row, block_num, present, code, scope, table, primary_key, payer, value);

struct file {
   FILE *f = nullptr;
   bool owns = false;

   file(FILE* f = nullptr, bool owns = true) : f(f), owns(owns) {}

   file(const file&) = delete;
   file(file&& src) { *this = std::move(src); }

   ~file() { close(); }

   file& operator=(const file&) = delete;

   file& operator=(file&& src) {
      close();
      this->f = src.f;
      this->owns = src.owns;
      src.f   = nullptr;
      src.owns = false;
      return *this;
   }

   void close() {
      if (owns)
         fclose(f);
      f = nullptr;
      owns = false;
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
   const char*                               wasm;
   eosio::vm::wasm_allocator&                wa;
   backend_t&                                backend;
   std::vector<char>                         args;
   std::vector<file>                         files;
   std::vector<std::unique_ptr<test_chain>>  chains;
   std::vector<std::unique_ptr<test_rodeos>> rodeoses;
   std::optional<uint32_t>                   selected_chain_index;
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
                                 eosio::chain::array_ptr<const ARR_ELEMENT_TYPE> data, uint32_t data_len,              \
                                 uint64_t& primary) {                                                                  \
      EOS_ASSERT(data_len == ARR_SIZE, eosio::chain::db_api_exception,                                                 \
                 "invalid size of secondary key array for " #IDX                                                       \
                 ": given ${given} bytes but expected ${expected} bytes",                                              \
                 ("given", data_len)("expected", ARR_SIZE));                                                           \
      return selected().IDX.find_secondary(code, scope, table, data, primary);                                         \
   }                                                                                                                   \
   int db_##IDX##_find_primary(uint64_t code, uint64_t scope, uint64_t table,                                          \
                               eosio::chain::array_ptr<ARR_ELEMENT_TYPE> data, uint32_t data_len, uint64_t primary) {  \
      EOS_ASSERT(data_len == ARR_SIZE, eosio::chain::db_api_exception,                                                 \
                 "invalid size of secondary key array for " #IDX                                                       \
                 ": given ${given} bytes but expected ${expected} bytes",                                              \
                 ("given", data_len)("expected", ARR_SIZE));                                                           \
      return selected().IDX.find_primary(code, scope, table, data.value, primary);                                     \
   }                                                                                                                   \
   int db_##IDX##_lowerbound(uint64_t code, uint64_t scope, uint64_t table,                                            \
                             eosio::chain::array_ptr<ARR_ELEMENT_TYPE> data, uint32_t data_len, uint64_t& primary) {   \
      EOS_ASSERT(data_len == ARR_SIZE, eosio::chain::db_api_exception,                                                 \
                 "invalid size of secondary key array for " #IDX                                                       \
                 ": given ${given} bytes but expected ${expected} bytes",                                              \
                 ("given", data_len)("expected", ARR_SIZE));                                                           \
      return selected().IDX.lowerbound_secondary(code, scope, table, data.value, primary);                             \
   }                                                                                                                   \
   int db_##IDX##_upperbound(uint64_t code, uint64_t scope, uint64_t table,                                            \
                             eosio::chain::array_ptr<ARR_ELEMENT_TYPE> data, uint32_t data_len, uint64_t& primary) {   \
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
      if (begin > end) {
        std::cerr << (void*)begin << " " << (void*)end << std::endl;
         throw std::runtime_error("bad memory");
      }
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
      if (state.backend.get_module().tables.size() < 0 || state.backend.get_module().tables[0].table.size() < cb_alloc)
         throw std::runtime_error("cb_alloc is out of range");
      auto result = state.backend.get_context().execute( //
            this, eosio::vm::jit_visitor(42), state.backend.get_module().tables[0].table[cb_alloc], cb_alloc_data,
            size);
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

   void eosio_assert_message(bool test, const char* msg, uint32_t msg_len) {
      check_bounds(msg, msg + msg_len);
      if (!test)
         throw ::assert_exception(std::string(msg, msg_len));
   }

   void print_range(const char* begin, const char* end) {
      check_bounds(begin, end);
      std::cerr.write(begin, end - begin);
   }

   void get_args(uint32_t cb_alloc_data, uint32_t cb_alloc) { set_data(cb_alloc_data, cb_alloc, state.args); }

   int32_t clock_gettime(int32_t id, void* data) {
      check_bounds((char*)data, (char*)data+8);
      std::chrono::nanoseconds result;
      if(id == 0) { // CLOCK_REALTIME
         result  = std::chrono::system_clock::now().time_since_epoch();
      } else if(id == 1) { // CLOCK_MONOTONIC
         result = std::chrono::steady_clock::now().time_since_epoch();
      } else {
         return -1;
      }
      int32_t sec = result.count()/1000000000;
      int32_t nsec = result.count()%1000000000;
      fc::datastream<char*> ds((char*)data, 8);
      fc::raw::pack(ds, sec);
      fc::raw::pack(ds, nsec);
      return 0;
   }

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
         state.files.emplace_back(stdin, false);
         state.files.emplace_back(stdout, false);
         state.files.emplace_back(stderr, false);

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

   bool isatty(int32_t file_index) {
      return !assert_file(file_index).owns;
   }

   void close_file(int32_t file_index) { assert_file(file_index).close(); }

   bool write_file(int32_t file_index, const char* content_begin, const char* content_end) {
      check_bounds(content_begin, content_end);
      auto& f = assert_file(file_index);
      return fwrite(content_begin, content_end - content_begin, 1, f.f) == 1;
   }

   int32_t read_file(int32_t file_index, char* content_begin, char * content_end) {
      check_bounds(content_begin, content_end);
      auto& f = assert_file(file_index);
      return fread(content_begin, 1, content_end - content_begin, f.f);
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
      else
         throw std::runtime_error("Chain already exists");
      return state.chains.size() - 1;
   }

   void destroy_chain(uint32_t chain) {
      assert_chain(chain);
      state.chains[chain].reset();
      while(!state.chains.empty() && !state.chains.back()) {
         state.chains.pop_back();
      }
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
      set_data(cb_alloc_data, cb_alloc, check(convert_to_bin(info)).value());
   }

   void push_transaction(uint32_t chain_index, const char* args_begin, const char* args_end, uint32_t cb_alloc_data,
                         uint32_t cb_alloc) {
      auto                             args        = unpack_checked<push_trx_args>(args_begin, args_end);
      auto                             transaction = unpack<eosio::chain::transaction>(args.transaction.data(),
                                                           args.transaction.data() + args.transaction.size());
      eosio::chain::signed_transaction signed_trx{ std::move(transaction), std::move(args.signatures),
                                                   std::move(args.context_free_data) };
      auto&                            chain = assert_chain(chain_index);
      chain.start_if_needed();
      for (auto& key : args.keys) signed_trx.sign(key, chain.control->get_chain_id());
      auto ptrx = std::make_shared<eosio::chain::packed_transaction>(signed_trx, eosio::chain::packed_transaction::compression_type::none);
      auto fut = eosio::chain::transaction_metadata::start_recover_keys(
            ptrx, chain.control->get_thread_pool(), chain.control->get_chain_id(), fc::microseconds::maximum());
      auto result = chain.control->push_transaction( fut.get(), fc::time_point::maximum(), 2000, true);
      // ilog("${r}", ("r", fc::json::to_pretty_string(result)));
      set_data(cb_alloc_data, cb_alloc, check(convert_to_bin(chain_types::transaction_trace{ convert(*result) })).value());
   }

   bool exec_deferred(uint32_t chain_index, uint32_t cb_alloc_data, uint32_t cb_alloc) {
      auto& chain = assert_chain(chain_index);
      chain.start_if_needed();
      const auto& idx =
            chain.control->db().get_index<eosio::chain::generated_transaction_multi_index, eosio::chain::by_delay>();
      auto itr = idx.begin();
      if (itr != idx.end() && itr->delay_until <= chain.control->pending_block_time()) {
         auto trace =
               chain.control->push_scheduled_transaction(itr->trx_id, fc::time_point::maximum(), billed_cpu_time_use, true);
         set_data(cb_alloc_data, cb_alloc, check(convert_to_bin(chain_types::transaction_trace{ convert(*trace) })).value());
         return true;
      }
      return false;
   }

   // todo: remove
   void query_database(uint32_t chain_index, const char* req_begin, const char* req_end, uint32_t cb_alloc_data,
                       uint32_t cb_alloc) {
      auto& chain = assert_chain(chain_index);
      check_bounds(req_begin, req_end);
      eosio::input_stream query_bin{ req_begin, req_end };
      eosio::name         query_name;
      eosio::check_discard(from_bin(query_name, query_bin));
      if (query_name == "cr.ctsp"_n)
         return set_data(cb_alloc_data, cb_alloc, query_contract_row_range_code_table_scope_pk(chain, query_bin));
      throw std::runtime_error("query_database: unknown query: " + (std::string)query_name);
   }

   // todo: remove
   std::vector<char> query_contract_row_range_code_table_scope_pk(test_chain& chain, eosio::input_stream& query_bin) {
      using eosio::from_bin; // ADL required
      auto snapshot_block    = eosio::check(from_bin<uint32_t>(query_bin)).value();
      auto first_code        = eosio::chain::name{ eosio::check(from_bin<eosio::name>(query_bin)).value().value };
      auto first_table       = eosio::chain::name{ eosio::check(from_bin<eosio::name>(query_bin)).value().value };
      auto first_scope       = eosio::chain::name{ eosio::check(from_bin<eosio::name>(query_bin)).value().value };
      auto first_primary_key = eosio::check(from_bin<uint64_t>(query_bin)).value();
      auto last_code         = eosio::chain::name{ eosio::check(from_bin<eosio::name>(query_bin)).value().value };
      auto last_table        = eosio::chain::name{ eosio::check(from_bin<eosio::name>(query_bin)).value().value };
      auto last_scope        = eosio::chain::name{ eosio::check(from_bin<eosio::name>(query_bin)).value().value };
      auto last_primary_key  = eosio::check(from_bin<uint64_t>(query_bin)).value();
      auto max_results       = eosio::check(from_bin<uint32_t>(query_bin)).value();

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
            rows.emplace_back(eosio::check(eosio::convert_to_bin(
                  contract_row{ uint32_t(0),
                                bool(true),
                                eosio::name{ to_uint64_t(table_it->code) },
                                eosio::name{ to_uint64_t(table_it->scope) },
                                eosio::name{ to_uint64_t(table_it->table) },
                                kv_it->primary_key,
                                eosio::name{ to_uint64_t(kv_it->payer) },
                                { kv_it->value.data(), kv_it->value.data() + kv_it->value.size() } })).value());
         };
      }
      return eosio::check(eosio::convert_to_bin(rows)).value();
   } // query_contract_row_range_code_table_scope_pk

   void select_chain_for_db(uint32_t chain_index) {
      assert_chain(chain_index);
      state.selected_chain_index = chain_index;
   }

   auto& selected() {
      if (!state.selected_chain_index || *state.selected_chain_index >= state.chains.size() ||
          !state.chains[*state.selected_chain_index])
         throw std::runtime_error("select_chain_for_db() must be called before using multi_index");
      return state.chains[*state.selected_chain_index]->get_apply_context();
   }

   test_rodeos& assert_rodeos(uint32_t rodeos) {
      if (rodeos >= state.rodeoses.size() || !state.rodeoses[rodeos])
         throw std::runtime_error("rodeos does not exist or was destroyed");
      return *state.rodeoses[rodeos];
   }

   uint32_t create_rodeos() {
      state.rodeoses.push_back(std::make_unique<test_rodeos>());
      return state.rodeoses.size() - 1;
   }

   void destroy_rodeos(uint32_t rodeos) {
      assert_rodeos(rodeos);
      state.rodeoses[rodeos].reset();
      while (!state.rodeoses.empty() && !state.rodeoses.back()) { state.rodeoses.pop_back(); }
   }

   void rodeos_set_filter(uint32_t rodeos, const char* wasm_filename) {
      auto& r = assert_rodeos(rodeos);
      r.filter.emplace(wasm_filename);
   }

   void connect_rodeos(uint32_t rodeos, uint32_t chain) {
      auto& r = assert_rodeos(rodeos);
      auto& c = assert_chain(chain);
      if (r.chain.chain)
         throw std::runtime_error("rodeos is already connected");
      r.chain = test_chain_ref{ c };
   }

   bool rodeos_push_block(uint32_t rodeos) {
      auto& r = assert_rodeos(rodeos);
      if (!r.chain.chain)
         throw std::runtime_error("rodeos is not connected to a chain");
      auto it = r.chain.chain->history.lower_bound(r.next_block);
      if (it == r.chain.chain->history.end())
         return false;
      r.write_snapshot->start_block(it->second.data(), it->second.size());
      r.write_snapshot->write_deltas(it->second.data(), it->second.size(), [] { return false; });
      if (r.filter)
         r.filter->run(*r.write_snapshot, it->second.data(), it->second.size());
      r.write_snapshot->end_block(it->second.data(), it->second.size(), true);
      r.next_block = it->first + 1;
      return true;
   }

   // clang-format off
    int db_get_i64(int iterator, char* buffer, uint32_t buffer_size)                    {return selected().db_get_i64(iterator, buffer, buffer_size);}
    int db_next_i64(int iterator, uint64_t& primary)                                    {return selected().db_next_i64(iterator, primary);}
    int db_previous_i64(int iterator, uint64_t& primary)                                {return selected().db_previous_i64(iterator, primary);}
    int db_find_i64(uint64_t code, uint64_t scope, uint64_t table, uint64_t id)         {return selected().db_find_i64(eosio::chain::name{code}, eosio::chain::name{scope}, eosio::chain::name{table}, id);}
    int db_lowerbound_i64(uint64_t code, uint64_t scope, uint64_t table, uint64_t id)   {return selected().db_lowerbound_i64(eosio::chain::name{code}, eosio::chain::name{scope}, eosio::chain::name{table}, id);}
    int db_upperbound_i64(uint64_t code, uint64_t scope, uint64_t table, uint64_t id)   {return selected().db_upperbound_i64(eosio::chain::name{code}, eosio::chain::name{scope}, eosio::chain::name{table}, id);}
    int db_end_i64(uint64_t code, uint64_t scope, uint64_t table)                       {return selected().db_end_i64(eosio::chain::name{code}, eosio::chain::name{scope}, eosio::chain::name{table});}

    DB_WRAPPERS_SIMPLE_SECONDARY(idx64,  uint64_t)
    DB_WRAPPERS_SIMPLE_SECONDARY(idx128, unsigned __int128)
    DB_WRAPPERS_ARRAY_SECONDARY(idx256, 2, unsigned __int128)
    DB_WRAPPERS_FLOAT_SECONDARY(idx_double, float64_t)
    DB_WRAPPERS_FLOAT_SECONDARY(idx_long_double, float128_t)
   // clang-format on

   bool kv_get(uint64_t db, uint64_t contract, const char* key, uint32_t key_size, uint32_t& value_size) {
      return kv_get_db(db).kv_get(contract, key, key_size, value_size);
   }

   uint32_t kv_get_data(uint64_t db, uint32_t offset, char* data, uint32_t data_size) {
      return kv_get_db(db).kv_get_data(offset, data, data_size);
   }

   uint32_t kv_it_create(uint64_t db, uint64_t contract, const char* prefix, uint32_t size) {
      auto& kdb = kv_get_db(db);
      uint32_t itr;
      if (!selected().kv_destroyed_iterators.empty()) {
         itr = selected().kv_destroyed_iterators.back();
         selected().kv_destroyed_iterators.pop_back();
      } else {
         // Sanity check in case the per-database limits are set poorly
         EOS_ASSERT(selected().kv_iterators.size() <= 0xFFFFFFFFu, kv_bad_iter, "Too many iterators");
         itr = selected().kv_iterators.size();
         selected().kv_iterators.emplace_back();
      }
      selected().kv_iterators[itr] = kdb.kv_it_create(contract, prefix, size);
      return itr;
   }

   void kv_it_destroy(uint32_t itr) {
      kv_check_iterator(itr);
      selected().kv_destroyed_iterators.push_back(itr);
      selected().kv_iterators[itr].reset();
   }

   int32_t kv_it_status(uint32_t itr) {
      kv_check_iterator(itr);
      return static_cast<int32_t>(selected().kv_iterators[itr]->kv_it_status());
   }

   int32_t kv_it_compare(uint32_t itr_a, uint32_t itr_b) {
      kv_check_iterator(itr_a);
      kv_check_iterator(itr_b);
      return selected().kv_iterators[itr_a]->kv_it_compare(*selected().kv_iterators[itr_b]);
   }

   int32_t kv_it_key_compare(uint32_t itr, const char* key, uint32_t size) {
      kv_check_iterator(itr);
      return selected().kv_iterators[itr]->kv_it_key_compare(key, size);
   }

   int32_t kv_it_move_to_end(uint32_t itr) {
      kv_check_iterator(itr);
      return static_cast<int32_t>(selected().kv_iterators[itr]->kv_it_move_to_end());
   }

   int32_t kv_it_next(uint32_t itr) {
      kv_check_iterator(itr);
      return static_cast<int32_t>(selected().kv_iterators[itr]->kv_it_next());
   }

   int32_t kv_it_prev(uint32_t itr) {
      kv_check_iterator(itr);
      return static_cast<int32_t>(selected().kv_iterators[itr]->kv_it_prev());
   }

   int32_t kv_it_lower_bound(uint32_t itr, const char* key, uint32_t size) {
      kv_check_iterator(itr);
      return static_cast<int32_t>(selected().kv_iterators[itr]->kv_it_lower_bound(key, size));
   }

   int32_t kv_it_key(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size) {
      kv_check_iterator(itr);
      return static_cast<int32_t>(selected().kv_iterators[itr]->kv_it_key(offset, dest, size, actual_size));
   }

   int32_t kv_it_value(uint32_t itr, uint32_t offset, char* dest, uint32_t size, uint32_t& actual_size) {
      kv_check_iterator(itr);
      return static_cast<int32_t>(selected().kv_iterators[itr]->kv_it_value(offset, dest, size, actual_size));
   }

   kv_context& kv_get_db(uint64_t db) {
      if (db == kvram_id.to_uint64_t())
         return *selected().kv_ram;
      else if (db == kvdisk_id.to_uint64_t())
         return *selected().kv_disk;
      EOS_ASSERT(false, kv_bad_db_id, "Bad key-value database ID");
   }

   void kv_check_iterator(uint32_t itr) {
      EOS_ASSERT(itr < selected().kv_iterators.size() && selected().kv_iterators[itr], kv_bad_iter, "Bad key-value iterator");
   }

    void sha1(const char* data, uint32_t datalen, void* hash_val) {
      check_bounds(data, data + datalen);
      auto hash = fc::sha1::hash( data, datalen );
      check_bounds((char*)hash_val, (char*)hash_val + hash.data_size());
      std::memcpy(hash_val, hash.data(), hash.data_size());
    }

    void sha256(const char* data, uint32_t datalen, void* hash_val) {
      check_bounds(data, data + datalen);
      auto hash = fc::sha256::hash( data, datalen );
      check_bounds((char*)hash_val, (char*)hash_val + hash.data_size());
      std::memcpy(hash_val, hash.data(), hash.data_size());
    }

    void sha512(const char* data, uint32_t datalen, void* hash_val) {
      check_bounds(data, data + datalen);
      auto hash = fc::sha512::hash( data, datalen );
      check_bounds((char*)hash_val, (char*)hash_val + hash.data_size());
      std::memcpy(hash_val, hash.data(), hash.data_size());
    }

    void ripemd160(const char* data, uint32_t datalen, void* hash_val) {
      check_bounds(data, data + datalen);
      auto hash = fc::ripemd160::hash( data, datalen );
      check_bounds((char*)hash_val, (char*)hash_val + hash.data_size());
      std::memcpy(hash_val, hash.data(), hash.data_size());
    }
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
   rhf_t::add<callbacks, &callbacks::clock_gettime, eosio::vm::wasm_allocator>("env", "clock_gettime");
   rhf_t::add<callbacks, &callbacks::reenter, eosio::vm::wasm_allocator>("env", "reenter");
   rhf_t::add<callbacks, &callbacks::open_file, eosio::vm::wasm_allocator>("env", "open_file");
   rhf_t::add<callbacks, &callbacks::isatty, eosio::vm::wasm_allocator>("env", "isatty");
   rhf_t::add<callbacks, &callbacks::close_file, eosio::vm::wasm_allocator>("env", "close_file");
   rhf_t::add<callbacks, &callbacks::write_file, eosio::vm::wasm_allocator>("env", "write_file");
   rhf_t::add<callbacks, &callbacks::read_file, eosio::vm::wasm_allocator>("env", "read_file");
   rhf_t::add<callbacks, &callbacks::read_whole_file, eosio::vm::wasm_allocator>("env", "read_whole_file");
   rhf_t::add<callbacks, &callbacks::execute, eosio::vm::wasm_allocator>("env", "execute");
   rhf_t::add<callbacks, &callbacks::create_chain, eosio::vm::wasm_allocator>("env", "create_chain");
   rhf_t::add<callbacks, &callbacks::destroy_chain, eosio::vm::wasm_allocator>("env", "destroy_chain");
   rhf_t::add<callbacks, &callbacks::start_block, eosio::vm::wasm_allocator>("env", "start_block");
   rhf_t::add<callbacks, &callbacks::finish_block, eosio::vm::wasm_allocator>("env", "finish_block");
   rhf_t::add<callbacks, &callbacks::get_head_block_info, eosio::vm::wasm_allocator>("env", "get_head_block_info");
   rhf_t::add<callbacks, &callbacks::push_transaction, eosio::vm::wasm_allocator>("env", "push_transaction");
   rhf_t::add<callbacks, &callbacks::exec_deferred, eosio::vm::wasm_allocator>("env", "exec_deferred");
   rhf_t::add<callbacks, &callbacks::query_database, eosio::vm::wasm_allocator>("env", "query_database_chain");
   rhf_t::add<callbacks, &callbacks::select_chain_for_db, eosio::vm::wasm_allocator>("env", "select_chain_for_db");

   rhf_t::add<callbacks, &callbacks::create_rodeos, eosio::vm::wasm_allocator>("env", "create_rodeos");
   rhf_t::add<callbacks, &callbacks::destroy_rodeos, eosio::vm::wasm_allocator>("env", "destroy_rodeos");
   rhf_t::add<callbacks, &callbacks::rodeos_set_filter, eosio::vm::wasm_allocator>("env", "rodeos_set_filter");
   rhf_t::add<callbacks, &callbacks::connect_rodeos, eosio::vm::wasm_allocator>("env", "connect_rodeos");
   rhf_t::add<callbacks, &callbacks::rodeos_push_block, eosio::vm::wasm_allocator>("env", "rodeos_push_block");

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
   rhf_t::add<callbacks, &callbacks::kv_get, eosio::vm::wasm_allocator>("env", "kv_get");
   rhf_t::add<callbacks, &callbacks::kv_get_data, eosio::vm::wasm_allocator>("env", "kv_get_data");
   rhf_t::add<callbacks, &callbacks::kv_it_create, eosio::vm::wasm_allocator>("env", "kv_it_create");
   rhf_t::add<callbacks, &callbacks::kv_it_destroy, eosio::vm::wasm_allocator>("env", "kv_it_destroy");
   rhf_t::add<callbacks, &callbacks::kv_it_status, eosio::vm::wasm_allocator>("env", "kv_it_status");
   rhf_t::add<callbacks, &callbacks::kv_it_compare, eosio::vm::wasm_allocator>("env", "kv_it_compare");
   rhf_t::add<callbacks, &callbacks::kv_it_key_compare, eosio::vm::wasm_allocator>("env", "kv_it_key_compare");
   rhf_t::add<callbacks, &callbacks::kv_it_move_to_end, eosio::vm::wasm_allocator>("env", "kv_it_move_to_end");
   rhf_t::add<callbacks, &callbacks::kv_it_next, eosio::vm::wasm_allocator>("env", "kv_it_next");
   rhf_t::add<callbacks, &callbacks::kv_it_prev, eosio::vm::wasm_allocator>("env", "kv_it_prev");
   rhf_t::add<callbacks, &callbacks::kv_it_lower_bound, eosio::vm::wasm_allocator>("env", "kv_it_lower_bound");
   rhf_t::add<callbacks, &callbacks::kv_it_key, eosio::vm::wasm_allocator>("env", "kv_it_key");
   rhf_t::add<callbacks, &callbacks::kv_it_value, eosio::vm::wasm_allocator>("env", "kv_it_value");
   rhf_t::add<callbacks, &callbacks::sha1, eosio::vm::wasm_allocator>("env", "sha1");
   rhf_t::add<callbacks, &callbacks::sha256, eosio::vm::wasm_allocator>("env", "sha256");
   rhf_t::add<callbacks, &callbacks::sha512, eosio::vm::wasm_allocator>("env", "sha512");
   rhf_t::add<callbacks, &callbacks::ripemd160, eosio::vm::wasm_allocator>("env", "ripemd160");
}

static void run(const char* wasm, const std::vector<std::string>& args) {
   eosio::vm::wasm_allocator wa;
   auto                      code = backend_t::read_wasm(wasm);
   backend_t                 backend(code);
   ::state                   state{ wasm, wa, backend, eosio::check(eosio::convert_to_bin(args)).value() };
   callbacks                 cb{ state };
   state.files.emplace_back(stdin, false);
   state.files.emplace_back(stdout, false);
   state.files.emplace_back(stderr, false);
   backend.set_wasm_allocator(&wa);

   rhf_t::resolve(backend.get_module());
   backend.initialize(&cb);
   // backend(&cb, "env", "initialize"); // todo: needs change to eosio-cpp
   backend(&cb, "env", "start", 0);
}

const char usage[] = "usage: eosio-tester [-h or --help] [-v or --verbose] file.wasm [args for wasm]\n";

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
