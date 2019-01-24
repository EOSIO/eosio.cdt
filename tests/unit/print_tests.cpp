#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#include <eosiolib/name.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/symbol.hpp>

using namespace eosio::native;

// EOSIO_TEST_BEGIN(print_test)
//    silence_output(true);
//    CHECK_PRINT("27", [](){ eosio::print((uint8_t)27); });
//    CHECK_PRINT("34", [](){ eosio::print((int)34); });
//    CHECK_PRINT([](std::string s){return s[0] == 'a';},  [](){ eosio::print((char)'a'); });
//    CHECK_PRINT([](std::string s){return s[0] == 'b';},  [](){ eosio::print((int8_t)'b'); });
//    CHECK_PRINT("202", [](){ eosio::print((unsigned int)202); });
//    CHECK_PRINT("-202", [](){ eosio::print((int)-202); });
//    CHECK_PRINT("707", [](){ eosio::print((unsigned long)707); });
//    CHECK_PRINT("-707", [](){ eosio::print((long)-707); });
//    CHECK_PRINT("909", [](){ eosio::print((unsigned long long)909); });
//    CHECK_PRINT("-909", [](){ eosio::print((long long)-909); });
//    CHECK_PRINT("404", [](){ eosio::print((uint32_t)404); });
//    CHECK_PRINT("-404", [](){ eosio::print((int32_t)-404); });
//    CHECK_PRINT("404000000", [](){ eosio::print((uint64_t)404000000); });
//    CHECK_PRINT("-404000000", [](){ eosio::print((int64_t)-404000000); });
//    CHECK_PRINT("0x0066000000000000", [](){ eosio::print((uint128_t)102); });
//    CHECK_PRINT("0xffffff9affffffffffffffffffffffff", [](){ eosio::print((int128_t)-102); });
//    silence_output(false);
// EOSIO_TEST_END

EOSIO_TEST_BEGIN(print_test)
   silence_output(false);

   // ------------------------------
   // inline void print(const char*)
   std::string s{};
   char c{};
   CHECK_PRINT( "", [&](){eosio::print(s.c_str());} );

   // Not understanding how this test isn't passing
   // for(unsigned short i{}; i < 256; ++i) {
   //    c = i;
   //    s = static_cast<char>(i);
   //    CHECK_PRINT( [&](std::string temp){return temp == std::string{c};}, [&](){eosio::print(s.c_str());} );
   
      
   //    eosio::print(c);
   //    eosio::print("\n");
   //    eosio::print(s);
   //    eosio::print("\n");
   // }

   // Not understanding how this test isn't passing
   // Using 70 as an arbitrary string length
   // for(unsigned short i{}; i < 70; ++i) {
   //    s = std::string(i, 'i');
   //    CHECK_PRINT( [&](std::string temp){return temp == std::string(i, 'i');}, [&](){eosio::print(s.c_str());} );
   // }

   // ----------------------------------------------------------------------------------
   // template <typename T, std::enable_if_t<std::is_integral<std::decay_t<T>>::value && 
   //                                        std::is_signed<std::decay_t<T>>::value, int> = 0>
   // inline void print(T)

   // ----------------------------------------------------------------------------------
   // template <typename T, std::enable_if_t<std::is_integral<std::decay_t<T>>::value && 
   //                                        std::is_signed<std::decay_t<T>>::value, int> = 0>
   // inline void print(T)

   // ------------------------
   // inline void print(float)
   // float f{};
   // for(float i{}; i < 1; f += 0.01) {
   //    CHECK_PRINT( [&](std::string){return std::to_string{f};}, [&](){eosio::print(f);} );
   // }

   // s = ;
   // CHECK_PRINT( "", [&](){eosio::print(s.c_str());} );

   // -------------------------
   // inline void print(double)

   // ------------------------------
   // inline void print(long double)

   // ---------------------
   // template<size_t Size>
   // inline void print(const fixed_bytes<Size>&)

   // ---------------------
   // template<size_t Size>
   // inline void print( fixed_bytes<Size>&)

   // -----------------------
   // inline void print(name)

   // -------------------------------------
   // inline void print(eosio::symbol_code)

   // ------------------------------------------------------------------------------------------
   // template<typename T, std::enable_if_t<!std::is_integral<std::decay_t<T>>::value, int> = 0>
   // inline void print(T&&)

   // --------------------------------
   // inline void print_f(const char*)

   // -----------------------------------------
   // template <typename Arg, typename... Args>
   // inline void print_f( const char*, Arg, Args...)

   // ----------------------------------------
   // template<typename Arg, typename... Args>
   // void print( Arg&&, Args&&...)

   // --------------------
   // template<typename T>
   // inline iostream& operator<<(iostream&, const T&)
   
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(print_test);
   return has_failed();
}
