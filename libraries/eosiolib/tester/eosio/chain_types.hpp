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

#if 0
template <typename T>
T read_raw(abieos::input_buffer& bin) {
   T           result{};
   std::string error;
   if (!abieos::read_raw<T>(bin, error, result))
      report_error(error);
   return result;
}
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

ABIEOS_REFLECT(extension) {
   ABIEOS_MEMBER(extension, type)
   ABIEOS_MEMBER(extension, data)
}

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

#if 0
inline bool bin_to_native(transaction_status& status, abieos::bin_to_native_state& state, bool) {
   status = transaction_status(read_raw<uint8_t>(state.bin));
   return true;
}

inline bool json_to_native(transaction_status&, abieos::json_to_native_state& state, abieos::event_type, bool) {
   state.error = "json_to_native: transaction_status unsupported";
   return false;
}

inline void native_to_bin(const transaction_status& obj, std::vector<char>& bin) {
   abieos::push_raw(bin, static_cast<uint8_t>(obj));
}
#endif

struct permission_level {
   abieos::name actor      = {};
   abieos::name permission = {};
};

ABIEOS_REFLECT(permission_level) {
   ABIEOS_MEMBER(permission_level, actor)
   ABIEOS_MEMBER(permission_level, permission)
}

struct account_auth_sequence {
   abieos::name account  = {};
   uint64_t     sequence = {};
};

ABIEOS_REFLECT(account_auth_sequence) {
   ABIEOS_MEMBER(account_auth_sequence, account)
   ABIEOS_MEMBER(account_auth_sequence, sequence)
}

struct account_delta {
   abieos::name account = {};
   int64_t      delta   = {};
};

EOSIO_REFLECT(account_delta, account, delta)

#if 0
ABIEOS_REFLECT(account_delta) {
   ABIEOS_MEMBER(account_delta, account)
   ABIEOS_MEMBER(account_delta, delta)
}
#endif

struct action_receipt_v0 {
   abieos::name                       receiver        = {};
   abieos::checksum256                act_digest      = {};
   uint64_t                           global_sequence = {};
   uint64_t                           recv_sequence   = {};
   std::vector<account_auth_sequence> auth_sequence   = {};
   abieos::varuint32                  code_sequence   = {};
   abieos::varuint32                  abi_sequence    = {};
};

ABIEOS_REFLECT(action_receipt_v0) {
   ABIEOS_MEMBER(action_receipt_v0, receiver)
   ABIEOS_MEMBER(action_receipt_v0, act_digest)
   ABIEOS_MEMBER(action_receipt_v0, global_sequence)
   ABIEOS_MEMBER(action_receipt_v0, recv_sequence)
   ABIEOS_MEMBER(action_receipt_v0, auth_sequence)
   ABIEOS_MEMBER(action_receipt_v0, code_sequence)
   ABIEOS_MEMBER(action_receipt_v0, abi_sequence)
}

using action_receipt = std::variant<action_receipt_v0>;

struct action {
   abieos::name                  account       = {};
   abieos::name                  name          = {};
   std::vector<permission_level> authorization = {};
   abieos::input_buffer          data          = {};
};

ABIEOS_REFLECT(action) {
   ABIEOS_MEMBER(action, account)
   ABIEOS_MEMBER(action, name)
   ABIEOS_MEMBER(action, authorization)
   ABIEOS_MEMBER(action, data)
}

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

ABIEOS_REFLECT(action_trace_v0) {
   ABIEOS_MEMBER(action_trace_v0, action_ordinal)
   ABIEOS_MEMBER(action_trace_v0, creator_action_ordinal)
   ABIEOS_MEMBER(action_trace_v0, receipt)
   ABIEOS_MEMBER(action_trace_v0, receiver)
   ABIEOS_MEMBER(action_trace_v0, act)
   ABIEOS_MEMBER(action_trace_v0, context_free)
   ABIEOS_MEMBER(action_trace_v0, elapsed)
   ABIEOS_MEMBER(action_trace_v0, console)
   ABIEOS_MEMBER(action_trace_v0, account_ram_deltas)
   ABIEOS_MEMBER(action_trace_v0, except)
   ABIEOS_MEMBER(action_trace_v0, error_code)
}

using action_trace = std::variant<action_trace_v0>;

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

ABIEOS_REFLECT(partial_transaction_v0) {
   ABIEOS_MEMBER(partial_transaction_v0, expiration)
   ABIEOS_MEMBER(partial_transaction_v0, ref_block_num)
   ABIEOS_MEMBER(partial_transaction_v0, ref_block_prefix)
   ABIEOS_MEMBER(partial_transaction_v0, max_net_usage_words)
   ABIEOS_MEMBER(partial_transaction_v0, max_cpu_usage_ms)
   ABIEOS_MEMBER(partial_transaction_v0, delay_sec)
   ABIEOS_MEMBER(partial_transaction_v0, transaction_extensions)
   ABIEOS_MEMBER(partial_transaction_v0, signatures)
   ABIEOS_MEMBER(partial_transaction_v0, context_free_data)
}

using partial_transaction = std::variant<partial_transaction_v0>;

struct recurse_transaction_trace;

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
   std::vector<recurse_transaction_trace> failed_dtrx_trace   = {};
   std::optional<partial_transaction>     reserved_do_not_use = {};
};

ABIEOS_REFLECT(transaction_trace_v0) {
   ABIEOS_MEMBER(transaction_trace_v0, id)
   ABIEOS_MEMBER(transaction_trace_v0, status)
   ABIEOS_MEMBER(transaction_trace_v0, cpu_usage_us)
   ABIEOS_MEMBER(transaction_trace_v0, net_usage_words)
   ABIEOS_MEMBER(transaction_trace_v0, elapsed)
   ABIEOS_MEMBER(transaction_trace_v0, net_usage)
   ABIEOS_MEMBER(transaction_trace_v0, scheduled)
   ABIEOS_MEMBER(transaction_trace_v0, action_traces)
   ABIEOS_MEMBER(transaction_trace_v0, account_ram_delta)
   ABIEOS_MEMBER(transaction_trace_v0, except)
   ABIEOS_MEMBER(transaction_trace_v0, error_code)
   ABIEOS_MEMBER(transaction_trace_v0, failed_dtrx_trace)
   ABIEOS_MEMBER(transaction_trace_v0, reserved_do_not_use)
}

using transaction_trace = std::variant<transaction_trace_v0>;

struct recurse_transaction_trace {
   transaction_trace recurse = {};
};

template<typename S>
eosio::result<void> from_bin(recurse_transaction_trace& obj, S& stream) {
   return from_bin(obj.recurse, stream);
}

template<typename S>
eosio::result<void> to_bin(const recurse_transaction_trace& obj, S& stream) {
   return to_bin(obj.recurse, stream);
}

#if 0
inline bool bin_to_native(recurse_transaction_trace& obj, abieos::bin_to_native_state& state, bool start) {
   return abieos::bin_to_native(obj.recurse, state, start);
}

inline bool json_to_native(recurse_transaction_trace& obj, abieos::json_to_native_state& state,
                           abieos::event_type event, bool start) {
   return abieos::json_to_native(obj.recurse, state, event, start);
}

inline void native_to_bin(const recurse_transaction_trace& obj, std::vector<char>& bin) {
   abieos::native_to_bin(obj.recurse, bin);
}
#endif

struct producer_key {
   abieos::name       producer_name     = {};
   abieos::public_key block_signing_key = {};
};

ABIEOS_REFLECT(producer_key) {
   ABIEOS_MEMBER(producer_key, producer_name)
   ABIEOS_MEMBER(producer_key, block_signing_key)
}

struct producer_schedule {
   uint32_t                  version   = {};
   std::vector<producer_key> producers = {};
};

ABIEOS_REFLECT(producer_schedule) {
   ABIEOS_MEMBER(producer_schedule, version)
   ABIEOS_MEMBER(producer_schedule, producers)
}

struct transaction_receipt_header {
   transaction_status status          = {};
   uint32_t           cpu_usage_us    = {};
   abieos::varuint32  net_usage_words = {};
};

ABIEOS_REFLECT(transaction_receipt_header) {
   ABIEOS_MEMBER(transaction_receipt_header, status)
   ABIEOS_MEMBER(transaction_receipt_header, cpu_usage_us)
   ABIEOS_MEMBER(transaction_receipt_header, net_usage_words)
}

struct packed_transaction {
   std::vector<abieos::signature> signatures               = {};
   uint8_t                        compression              = {};
   abieos::input_buffer           packed_context_free_data = {};
   abieos::input_buffer           packed_trx               = {};
};

ABIEOS_REFLECT(packed_transaction) {
   ABIEOS_MEMBER(packed_transaction, signatures)
   ABIEOS_MEMBER(packed_transaction, compression)
   ABIEOS_MEMBER(packed_transaction, packed_context_free_data)
   ABIEOS_MEMBER(packed_transaction, packed_trx)
}

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

ABIEOS_REFLECT(signed_block_header) {
   ABIEOS_BASE(block_header)
   ABIEOS_MEMBER(signed_block_header, producer_signature)
}

struct signed_block : signed_block_header {
   std::vector<transaction_receipt> transactions     = {};
   std::vector<extension>           block_extensions = {};
};

ABIEOS_REFLECT(signed_block) {
   ABIEOS_BASE(signed_block_header)
   ABIEOS_MEMBER(signed_block, transactions)
   ABIEOS_MEMBER(signed_block, block_extensions)
}

struct block_info {
   uint32_t                block_num = {};
   abieos::checksum256     block_id  = {};
   abieos::block_timestamp timestamp = {};
};

ABIEOS_REFLECT(block_info) {
   ABIEOS_MEMBER(block_info, block_num)
   ABIEOS_MEMBER(block_info, block_id)
   ABIEOS_MEMBER(block_info, timestamp)
}

} // namespace chain_types
