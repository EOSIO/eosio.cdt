#pragma once

#include <abieos.hpp>
#include <eosio/stream.hpp>

#ifdef EOSIO_CDT_COMPILATION
#   include <eosio/check.hpp>
#endif

namespace chain_types {

#ifdef EOSIO_CDT_COMPILATION
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Winvalid-noreturn"
[[noreturn]] inline void report_error(const std::string& s) { eosio::check(false, s); }
#   pragma clang diagnostic pop
#else
[[noreturn]] inline void report_error(const std::string& s) { throw std::runtime_error(s); }
#endif

template <typename T>
T assert_bin_to_native(const std::vector<char>& bin) {
   return eosio::check(eosio::convert_from_bin<T>(bin)).value();
}

template<typename T>
auto assert_native_to_bin(const T& t) {
   return eosio::check(eosio::convert_to_bin(t)).value();
}

struct extension {
   uint16_t             type = {};
   abieos::input_buffer data = {};
};

EOSIO_REFLECT(extension, type, data);

enum class transaction_status : uint8_t {
   executed  = 0, // succeed, no error handler executed
   soft_fail = 1, // objectively failed (not executed), error handler executed
   hard_fail = 2, // objectively failed and error handler objectively failed thus no state change
   delayed   = 3, // transaction delayed/deferred/scheduled for future execution
   expired   = 4, // transaction expired and storage space refunded to user
};

inline std::string to_string(transaction_status status) {
   switch (status) {
      case transaction_status::executed: return "executed";
      case transaction_status::soft_fail: return "soft_fail";
      case transaction_status::hard_fail: return "hard_fail";
      case transaction_status::delayed: return "delayed";
      case transaction_status::expired: return "expired";
   }
   report_error("unknown status: " + std::to_string((uint8_t)status));
}

inline transaction_status get_transaction_status(const std::string& s) {
   if (s == "executed")
      return transaction_status::executed;
   if (s == "soft_fail")
      return transaction_status::soft_fail;
   if (s == "hard_fail")
      return transaction_status::hard_fail;
   if (s == "delayed")
      return transaction_status::delayed;
   if (s == "expired")
      return transaction_status::expired;
   report_error("unknown status: " + s);
}

struct permission_level {
   abieos::name actor      = {};
   abieos::name permission = {};
};

EOSIO_REFLECT(permission_level, actor, permission);

struct account_auth_sequence {
   abieos::name account  = {};
   uint64_t     sequence = {};
};

EOSIO_REFLECT(account_auth_sequence, account, sequence);

struct account_delta {
   abieos::name account = {};
   int64_t      delta   = {};
};

EOSIO_REFLECT(account_delta, account, delta);

struct action_receipt_v0 {
   abieos::name                       receiver        = {};
   abieos::checksum256                act_digest      = {};
   uint64_t                           global_sequence = {};
   uint64_t                           recv_sequence   = {};
   std::vector<account_auth_sequence> auth_sequence   = {};
   abieos::varuint32                  code_sequence   = {};
   abieos::varuint32                  abi_sequence    = {};
};

EOSIO_REFLECT(action_receipt_v0, receiver, act_digest, global_sequence, recv_sequence, auth_sequence, code_sequence, abi_sequence);

template<typename T>
struct variant : T {
   constexpr variant() = default;
   constexpr variant(T&& obj) : T(std::move(obj)) {}
   constexpr variant(const T& obj) : T(obj) {}
};

template<typename T, typename S>
eosio::result<void> to_bin(const variant<T>& obj, S& stream) {
   uint8_t idx = 0;
   OUTCOME_TRY(to_bin(idx, stream));
   return to_bin(static_cast<const T&>(obj), stream);
}

template<typename T, typename S>
eosio::result<void> from_bin(variant<T>& obj, S& stream) {
   uint8_t idx = 0;
   OUTCOME_TRY(from_bin(idx, stream));
   if(idx != 0) {
      return eosio::stream_error::bad_variant_index;
   }
   return from_bin(static_cast<T&>(obj), stream);
}

using action_receipt = variant<action_receipt_v0>;

struct action {
   abieos::name                  account       = {};
   abieos::name                  name          = {};
   std::vector<permission_level> authorization = {};
   abieos::input_buffer          data          = {};
};

EOSIO_REFLECT(action, account, name, authorization, data);

struct action_trace_v0 {
   abieos::varuint32             action_ordinal         = {};
   abieos::varuint32             creator_action_ordinal = {};
   std::optional<action_receipt> receipt                = {};
   abieos::name                  receiver               = {};
   action                        act                    = {};
   bool                          context_free           = {};
   int64_t                       elapsed                = {};
   std::string                   console                = {};
   std::vector<account_delta>    account_ram_deltas     = {};
   std::optional<std::string>    except                 = {};
   std::optional<uint64_t>       error_code             = {};
};

EOSIO_REFLECT(action_trace_v0, action_ordinal, creator_action_ordinal, receipt, receiver, act,
              context_free, elapsed, console, account_ram_deltas, except, error_code);

using action_trace = variant<action_trace_v0>;

struct partial_transaction_v0 {
   abieos::time_point_sec            expiration             = {};
   uint16_t                          ref_block_num          = {};
   uint32_t                          ref_block_prefix       = {};
   abieos::varuint32                 max_net_usage_words    = {};
   uint8_t                           max_cpu_usage_ms       = {};
   abieos::varuint32                 delay_sec              = {};
   std::vector<extension>            transaction_extensions = {};
   std::vector<abieos::signature>    signatures             = {};
   std::vector<abieos::input_buffer> context_free_data      = {};
};

EOSIO_REFLECT(partial_transaction_v0, expiration, ref_block_num, ref_block_prefix, max_net_usage_words,
              max_cpu_usage_ms, delay_sec, transaction_extensions, signatures, context_free_data);

using partial_transaction = variant<partial_transaction_v0>;

struct recurse_transaction_trace;

struct transaction_trace_v0;
using transaction_trace = variant<transaction_trace_v0>;

struct transaction_trace_v0 {
   abieos::checksum256                    id                  = {};
   transaction_status                     status              = {};
   uint32_t                               cpu_usage_us        = {};
   abieos::varuint32                      net_usage_words     = {};
   int64_t                                elapsed             = {};
   uint64_t                               net_usage           = {};
   bool                                   scheduled           = {};
   std::vector<action_trace>              action_traces       = {};
   std::optional<account_delta>           account_ram_delta   = {};
   std::optional<std::string>             except              = {};
   std::optional<uint64_t>                error_code          = {};
   std::vector<transaction_trace>         failed_dtrx_trace   = {};
   std::optional<partial_transaction>     reserved_do_not_use = {};
};

EOSIO_REFLECT(transaction_trace_v0, id, status, cpu_usage_us, elapsed, net_usage_words, scheduled, action_traces,
              account_ram_delta, except, error_code, failed_dtrx_trace, reserved_do_not_use);

struct producer_key {
   abieos::name       producer_name     = {};
   abieos::public_key block_signing_key = {};
};

EOSIO_REFLECT(producer_key, producer_name, block_signing_key);

struct producer_schedule {
   uint32_t                  version   = {};
   std::vector<producer_key> producers = {};
};

EOSIO_REFLECT(producer_schedule, version, producers);

struct transaction_receipt_header {
   transaction_status status          = {};
   uint32_t           cpu_usage_us    = {};
   abieos::varuint32  net_usage_words = {};
};

EOSIO_REFLECT(transaction_receipt_header, status, cpu_usage_us, net_usage_words);

struct packed_transaction {
   std::vector<abieos::signature> signatures               = {};
   uint8_t                        compression              = {};
   abieos::input_buffer           packed_context_free_data = {};
   abieos::input_buffer           packed_trx               = {};
};

EOSIO_REFLECT(packed_transaction, signatures, compression, packed_context_free_data, packed_trx);

using transaction_variant = std::variant<abieos::checksum256, packed_transaction>;

struct transaction_receipt : transaction_receipt_header {
   transaction_variant trx = {};
};

EOSIO_REFLECT(transaction_receipt, base transaction_receipt_header, trx);

struct block_header {
   abieos::block_timestamp          timestamp         = {};
   abieos::name                     producer          = {};
   uint16_t                         confirmed         = {};
   abieos::checksum256              previous          = {};
   abieos::checksum256              transaction_mroot = {};
   abieos::checksum256              action_mroot      = {};
   uint32_t                         schedule_version  = {};
   std::optional<producer_schedule> new_producers     = {};
   std::vector<extension>           header_extensions = {};
};

EOSIO_REFLECT(block_header,
   timestamp, producer, confirmed, previous, transaction_mroot, action_mroot,
   schedule_version, new_producers, header_extensions
)

struct signed_block_header : block_header {
   abieos::signature producer_signature = {};
};

EOSIO_REFLECT(signed_block_header, base block_header, producer_signature);

struct signed_block : signed_block_header {
   std::vector<transaction_receipt> transactions     = {};
   std::vector<extension>           block_extensions = {};
};

EOSIO_REFLECT(signed_block, base signed_block_header, transactions, block_extensions);

struct block_info {
   uint32_t                block_num = {};
   abieos::checksum256     block_id  = {};
   abieos::block_timestamp timestamp = {};
};

EOSIO_REFLECT(block_info, block_num, block_id, timestamp);

} // namespace chain_types
