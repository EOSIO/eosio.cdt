#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#include <eosiolib/varint.hpp>

using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/varint.hpp`
EOSIO_TEST_BEGIN(unsigned_int_type_test)
   silence_output(false);

   // ------------
   // constructors
   
   /// unsigned_int(uint32_t)
   

   /// template<typename T>
   /// unsigned_int(T)


   //  template<typename T>
   //  operator T()const

   // ---------------------------------
   // unsigned_int& operator=(uint32_t)

   // ------------------------------------------------------------
   // friend bool operator==(const unsigned_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator==(const uint32_t&, const unsigned_int&)

   // ----------------------------------------------------------------
   // friend bool operator==(const unsigned_int&, const unsigned_int&)

   // ------------------------------------------------------------
   // friend bool operator!=(const unsigned_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator!=(const uint32_t&, const unsigned_int&)

   // ----------------------------------------------------------------
   // friend bool operator!=(const unsigned_int&, const unsigned_int&)

   // ------------------------------------------------------------
   // friend bool operator< (const unsigned_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator< (const uint32_t&, const unsigned_int&)

   // ----------------------------------------------------------------
   // friend bool operator< (const unsigned_int&, const unsigned_int&)

   // ------------------------------------------------------------
   // friend bool operator>=(const unsigned_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator>=(const uint32_t&, const unsigned_int&)

   // ----------------------------------------------------------------
   // friend bool operator>=(const unsigned_int&, const unsigned_int&)

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator<<(DataStream&, const unsigned_int&)

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator>>(DataStream&, unsigned_int&)   

   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/varint.hpp`
EOSIO_TEST_BEGIN(signed_int_type_test)
   silence_output(false);

   // ------------
   // constructors
   
   /// signed_int(uint32_t)

   //  template<typename T>
   //  signed_int& operator=(const T&)

   // --------------------------
   // signed_int operator++(int)

   // ------------------------
   // signed_int& operator++()

   // ------------------------------------------------------------
   // friend bool operator==(const signed_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator==(const uint32_t&, const signed_int&)

   // ----------------------------------------------------------------
   // friend bool operator==(const signed_int&, const signed_int&)

   // ------------------------------------------------------------
   // friend bool operator!=(const signed_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator!=(const uint32_t&, const signed_int&)

   // ----------------------------------------------------------------
   // friend bool operator!=(const signed_int&, const signed_int&)

   // ------------------------------------------------------------
   // friend bool operator< (const signed_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator< (const uint32_t&, const signed_int&)

   // ----------------------------------------------------------------
   // friend bool operator< (const signed_int&, const signed_int&)

   // ------------------------------------------------------------
   // friend bool operator>=(const signed_int&, const uint32_t&)

   // ------------------------------------------------------------
   // friend bool operator>=(const uint32_t&, const signed_int&)

   // ----------------------------------------------------------------
   // friend bool operator>=(const signed_int&, const signed_int&)

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator<<(DataStream&, const signed_int&)

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator>>(DataStream&, signed_int&)

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(signed_int_type_test);
   // EOSIO_TEST(signed_int_type_test);
   return has_failed();
}
