#pragma once
#define FMT_HEADER_ONLY

#include <eosio/chain_types.hpp>
#include <eosio/database.hpp>
#include <eosio/eosio.hpp>
#include <eosio/temp_placeholders.hpp>
#include <eosio/transaction.hpp>
#include <wchar.h>

// TODO: move to <wchar.h>
namespace std {
inline int fputws(const wchar_t* ws, FILE* stream) { return ::fputws(ws, stream); }
} // namespace std

#include <fmt/format.h>

#define TESTER_STRINGIFY1(x) #x
#define TESTER_STRINGIFY(x) TESTER_STRINGIFY1(x)

#define TESTER_REQUIRE_EQUAL(EXPECTED, ACTUAL)                                                                         \
   if ((EXPECTED) != (ACTUAL))                                                                                         \
      eosio::check(false, "expected value doesn't match at " __FILE__ ":" TESTER_STRINGIFY(__LINE__));

#define TESTER_REQUIRE(ACTUAL)                                                                                         \
   if (!(ACTUAL))                                                                                                      \
      eosio::check(false, "expected value doesn't match at " __FILE__ ":" TESTER_STRINGIFY(__LINE__));

namespace eosio {
namespace internal_use_do_not_use {

   extern "C" void    get_args(void* cb_alloc_data, void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   extern "C" bool    reenter(const char* args_begin, const char* args_end, void (*cb)(), void* cb_alloc_data,
                              void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   extern "C" int32_t open_file(const char* filename_begin, const char* filename_end, const char* mode_begin,
                                const char* mode_end);
   extern "C" void    close_file(int32_t file_index);
   extern "C" bool    write_file(int32_t file_index, const char* content_begin, const char* content_end);
   extern "C" bool    read_whole_file(const char* filename_begin, const char* filename_end, void* cb_alloc_data,
                                      void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   extern "C" int32_t execute(const char* command_begin, const char* command_end);

   extern "C" uint32_t create_chain();
   extern "C" void     destroy_chain(uint32_t chain);
   extern "C" void     start_block(uint32_t chain, int64_t skip_miliseconds);
   extern "C" void     finish_block(uint32_t chain);
   extern "C" void     get_head_block_info(uint32_t chain, void* cb_alloc_data,
                                           void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   extern "C" void push_transaction(uint32_t chain, const char* args_begin, const char* args_end, void* cb_alloc_data,
                                    void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   extern "C" void query_database_chain(uint32_t chain, void* req_begin, void* req_end, void* cb_alloc_data,
                                        void* (*cb_alloc)(void* cb_alloc_data, size_t size));
   extern "C" void select_chain_for_db(uint32_t chain_index);

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

inline int32_t open_file(std::string_view filename, std::string_view mode) {
   return internal_use_do_not_use::open_file(filename.data(), filename.data() + filename.size(), mode.data(),
                                             mode.data() + mode.size());
}

inline void close_file(int32_t file_index) { return internal_use_do_not_use::close_file(file_index); }

inline bool write_file(int32_t file_index, std::string_view content) {
   return internal_use_do_not_use::write_file(file_index, content.data(), content.data() + content.size());
}

struct file {
   int32_t     file_index = -1;
   std::string fmt_buffer;

   file(std::string_view filename, std::string_view mode, bool check = true) {
      file_index = open_file(filename, mode);
      if (check && file_index < 0)
         eosio::check(false, "open " + std::string(filename) + " failed");
   }

   file(const file&) = delete;
   file(file&&)      = delete;

   ~file() { close(); }

   void close() {
      if (file_index >= 0)
         close_file(file_index);
      file_index = -1;
   }

   void write(std::string_view content) { write_file(file_index, content); }

   template <typename S, typename... Args>
   void format(const S& format_str, Args&&... args) {
      fmt_buffer.clear();
      fmt::format_to(std::back_inserter(fmt_buffer), format_str, std::forward<Args>(args)...);
      write(fmt_buffer);
   }
};

template <typename S, typename... Args>
inline void print_fmt(const S& format_str, Args&&... args) {
   static std::string fmt_buffer;
   fmt_buffer.clear();
   fmt::format_to(std::back_inserter(fmt_buffer), format_str, std::forward<Args>(args)...);
   print(fmt_buffer);
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

   // TODO: move definition to someplace else
   struct key_weight {
      public_key key    = {};
      uint16_t   weight = {};
   };

   // TODO: move definition to someplace else
   struct permission_level_weight {
      permission_level permission = {};
      uint16_t         weight     = {};
   };

   // TODO: move definition to someplace else
   struct wait_weight {
      uint32_t wait_sec = {};
      uint16_t weight   = {};
   };

   // TODO: move definition to someplace else
   struct authority {
      uint32_t                             threshold = {};
      std::vector<key_weight>              keys      = {};
      std::vector<permission_level_weight> accounts  = {};
      std::vector<wait_weight>             waits     = {};
   };

   void start_block(int64_t skip_miliseconds = 0) {
      head_block_info.reset();
      if (skip_miliseconds > 500) {
         internal_use_do_not_use::start_block(id, skip_miliseconds - 500);
         internal_use_do_not_use::start_block(id, 0);
      } else {
         internal_use_do_not_use::start_block(id, skip_miliseconds);
      }
   }

   void finish_block() {
      head_block_info.reset();
      internal_use_do_not_use::finish_block(id);
   }

   void select_chain_for_db() { internal_use_do_not_use::select_chain_for_db(id); }

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

   chain_types::transaction_trace push_transaction(const transaction& trx) {
      return push_transaction(trx, { default_priv_key });
   }

   chain_types::transaction_trace transact(std::vector<action>&& actions, const std::vector<abieos::private_key>& keys,
                                           const char* expected_except = nullptr) {
      auto trace = push_transaction(make_transaction(std::move(actions)), keys);
      expect(trace, expected_except);
      return trace;
   }

   chain_types::transaction_trace transact(std::vector<action>&& actions, const char* expected_except = nullptr) {
      return transact(std::move(actions), { default_priv_key }, expected_except);
   }

   chain_types::transaction_trace create_account(name ac, const public_key& pub_key,
                                                 const char* expected_except = nullptr) {
      authority simple_auth{
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

   chain_types::transaction_trace create_code_account(name ac, const public_key& pub_key,
                                                      const char* expected_except = nullptr) {
      authority simple_auth{
         .threshold = 1,
         .keys      = { { pub_key, 1 } },
      };
      authority code_auth{
         .threshold = 1,
         .keys      = { { pub_key, 1 } },
         .accounts  = { { { ac, "eosio.code"_n }, 1 } },
      };
      return transact({ action{ { { "eosio"_n, "active"_n } },
                                "eosio"_n,
                                "newaccount"_n,
                                std::make_tuple("eosio"_n, ac, simple_auth, code_auth) } },
                      expected_except);
   }

   chain_types::transaction_trace create_code_account(name ac, const char* expected_except = nullptr) {
      return create_code_account(ac, convert(default_pub_key), expected_except);
   }

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

struct test_case {
   const char* name;
   void (*f)() = {};

   test_case(const char* name, void (*f)()) : name{ name }, f{ f } { get_tests().push_back(this); }
   test_case(const test_case&) = delete;
   test_case(test_case&&)      = delete;

   static std::vector<test_case*>& get_tests() {
      static std::vector<test_case*> tests;
      return tests;
   }
};
#define TEST_CASE(N, ...) eosio::test_case N{ #N, __VA_ARGS__ };

struct comparison_file {
   std::string filename;
   eosio::file file;

   comparison_file(const std::string& filename) : filename(filename), file(filename + ".actual", "w") {}

   ~comparison_file() { close(); }

   void write(std::string_view content) { file.write(content); }

   template <typename S, typename... Args>
   void format(const S& format_str, Args&&... args) {
      file.format(format_str, std::forward<Args>(args)...);
   }

   void close() {
      file.close();
      bool write = false;
      for (auto& arg : get_args())
         if (arg == "--write")
            write = true;
      if (write) {
         auto cmd = "cp " + filename + ".actual " + filename + ".expected";
         if (execute(cmd))
            check(false, "copy failed");
      } else {
         check(!execute("colordiff -u " + filename + ".expected " + filename + ".actual"),
               "files are different; use --write to overwrite");
      }
   }
};

inline void run_tests(void (*f)()) {
   if (f)
      return f();
   int  num_passed = 0;
   int  num_failed = 0;
   auto run        = [&](auto& test) {
      auto ok = reenter(test->f, [&](auto& error) { print(error, "\n"); });
      if (ok) {
         ++num_passed;
         print_fmt("\033[96m{:60} \033[32mpassed\033[0m\n", std::string{ test->name });
      } else {
         ++num_failed;
         print_fmt("\033[96m{:60} \033[31mfailed\033[0m\n", std::string{ test->name });
      }
   };
   bool  manual = false;
   auto& args   = get_args();
   for (size_t i = 0; i + 1 < args.size(); ++i) {
      if (args[i] == "-t") {
         manual     = true;
         bool found = false;
         for (auto* test : test_case::get_tests()) {
            if (test->name == args[i + 1]) {
               found = true;
               run(test);
            }
         }
         if (!found) {
            ++num_failed;
            print_fmt("\033[96m{:60} \033[31mnot found\033[0m\n", args[i + 1]);
         }
      }
   }
   if (!manual)
      for (auto* test : test_case::get_tests()) run(test);
   if (num_passed)
      print_fmt("\033[32m{} tests passed\033[0m", num_passed);
   if (num_failed) {
      if (num_passed)
         print_fmt(", ");
      print_fmt("\033[31m{} tests failed\033[0m\n", num_failed);
      check(false, "test(s) failed");
   } else {
      print_fmt("\n");
   }
}

} // namespace eosio

#define TEST_ENTRY                                                                                                     \
   extern "C" __attribute__((eosio_wasm_entry)) void initialize() {}                                                   \
   extern "C" __attribute__((eosio_wasm_entry)) void run_query(void (*f)()) { eosio::run_tests(f); }
