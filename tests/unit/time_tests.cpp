#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#include <eosiolib/time.hpp>

using eosio::microseconds;
using eosio::time_point;
using eosio::time_point_sec;

using namespace eosio::native;

const uint32_t u32MAX = std::numeric_limits<uint32_t>::max(); // 18446744073709551615
const uint32_t u32MIN = std::numeric_limits<uint32_t>::min(); // 0

const int64_t i64MAX = std::numeric_limits<int64_t>::max(); // 9223372036854775807
const int64_t i64MIN = std::numeric_limits<int64_t>::min(); // -9223372036854775808

// This is interesting. `print_f` won't print the '-' character.
// `printf` won't print anything.
// `printf("%\n",i64MIN);`

// Note that there are no invariants established to protect this type against overflow.
// For example: if you define a microsecond type from i64MAX and then check that value in
// Or if you check the `seconds` of a (u64MAX / 1000000 + 1), it will overflow.
// Potentially could be abused in a contract?

// Defined in `eosio.cdt/libraries/eosiolib/time.hpp`
EOSIO_TEST_BEGIN(microseconds_type_test)
   silence_output(false);

   // ------------
   // constructors

   // -----------------------------------------------
   /// explicit microseconds(uint64_t)/int64_t count()
   REQUIRE_EQUAL( microseconds{}.count(), 0ULL )
   REQUIRE_EQUAL( microseconds{i64MAX}.count(), i64MAX )
   REQUIRE_EQUAL( microseconds{i64MIN}.count(), i64MIN )

   // -----------------------------
   // static microseconds maximum()
   REQUIRE_EQUAL( microseconds::maximum().count(), microseconds{0x7FFFFFFFFFFFFFFFLL}.count() )

   // ------------------------------------------------------------------------
   // friend microseconds operator+(const  microseconds&, const microseconds&)
   // friend microseconds operator-(const  microseconds&, const microseconds&)
   REQUIRE_EQUAL( (microseconds{0LL} + microseconds{1LL}), microseconds{1LL} )
   REQUIRE_EQUAL( (microseconds{0LL} - microseconds{1LL}), microseconds{-1LL} )

   // -------------------------------------
   // bool operator==(const  microseconds&)
   // bool operator!=(const  microseconds&)
   // bool operator<(const  microseconds&)
   // bool operator<=(const  microseconds&)
   // bool operator>(const  microseconds&)
   // bool operator>=(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{1LL} == microseconds{1LL}), true )
   REQUIRE_EQUAL( (microseconds{1LL} != microseconds{0LL}), true )
   REQUIRE_EQUAL( (microseconds{0LL} < microseconds{1LL}), true )
   REQUIRE_EQUAL( (microseconds{1LL} <= microseconds{1LL}), true )
   REQUIRE_EQUAL( (microseconds{1LL} > microseconds{0LL}), true )
   REQUIRE_EQUAL( (microseconds{1LL} >= microseconds{1LL}), true )

   // ----------------------------------------------
   // microseconds& operator+=(const  microseconds&)
   // microseconds& operator-=(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{0LL} += microseconds{1LL}), microseconds{1LL} )
   REQUIRE_EQUAL( (microseconds{0LL} -= microseconds{1LL}), microseconds{-1LL} )

   // --------------------
   // int64_t to_seconds()
   REQUIRE_EQUAL( (microseconds{i64MAX}.to_seconds()), i64MAX / 1000000 )

   // ------------------------------------
   // inline microseconds seconds(int64_t)
   // inline microseconds milliseconds(int64_t)
   // inline microseconds minutes(int64_t)
   // inline microseconds hours(int64_t)
   // inline microseconds days(int64_t)
   REQUIRE_EQUAL( eosio::seconds(60LL), microseconds{60LL*1000000LL} )
   REQUIRE_EQUAL( eosio::milliseconds(60LL), microseconds{60LL*1000LL} )
   REQUIRE_EQUAL( eosio::minutes(60LL), microseconds{60LL*60LL*1000000LL} )
   REQUIRE_EQUAL( eosio::hours(60LL), microseconds{60LL*60LL*60LL*1000000LL} )
   REQUIRE_EQUAL( eosio::days(60LL), microseconds{24LL*60LL*60LL*60LL*1000000LL} )

   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(time_point_type_test)
   silence_output(true);

   // ------------
   // constructors
   microseconds ms0{0LL};
   microseconds ms1{1LL};
   microseconds ms2{-1LL};
   microseconds ms_max{i64MAX};
   microseconds ms_min{i64MIN};
   
   /// explicit time_point(microseconds)/microseconds& time_since_epoch()
   REQUIRE_EQUAL( time_point{ms0}.time_since_epoch(), ms0 )
   REQUIRE_EQUAL( time_point{ms1}.time_since_epoch(), ms1 )

   // --------------------------
   // uint32_t sec_since_epoch()
   REQUIRE_EQUAL( time_point{ms0}.sec_since_epoch(), 0 / 1000000 )
   REQUIRE_EQUAL( time_point{ms1}.sec_since_epoch(), 1 / 1000000 )
   REQUIRE_EQUAL( time_point{ms_max}.sec_since_epoch(), i64MAX / 1000000 )
   REQUIRE_EQUAL( time_point{ms_min}.sec_since_epoch(), i64MIN / 1000000 )

   // -----------------------------------
   // bool operator==(const  time_point&)
   // bool operator!=(const  time_point&)
   // bool operator<(const  time_point&)
   // bool operator<=(const  time_point&)
   // bool operator>(const  time_point&)
   // bool operator>=(const  time_point&)
   REQUIRE_EQUAL( (time_point{ms0} == time_point{ms0}), true )
   REQUIRE_EQUAL( (time_point{ms1} != time_point{ms0}), true )
   REQUIRE_EQUAL( (time_point{ms0} < time_point{ms1}), true )
   REQUIRE_EQUAL( (time_point{ms1} <= time_point{ms1}), true )
   REQUIRE_EQUAL( (time_point{ms1} > time_point{ms0}), true )
   REQUIRE_EQUAL( (time_point{ms1} >= time_point{ms1}), true )

   // --------------------------------------------
   // time_point& operator+=(const  microseconds&)
   // time_point& operator-=(const  microseconds&)
   REQUIRE_EQUAL( (time_point{ms0} += ms1), time_point{ms1} )
   REQUIRE_EQUAL( (time_point{ms0} -= ms1), time_point{ms2} )

   // ------------------------------------------
   // time_point operator+(const  microseconds&)
   // time_point operator+(const  time_point&)
   // time_point operator-(const  microseconds&)
   // microseconds operator-(const  time_point&)
   REQUIRE_EQUAL( (time_point{ms0} + ms1), time_point{ms1} )
   REQUIRE_EQUAL( (time_point{ms0} + time_point{ms1}), time_point{ms1} )
   REQUIRE_EQUAL( (time_point{ms0} - ms1), time_point{ms2} )
   REQUIRE_EQUAL( (time_point{ms0} - time_point{ms1}), ms2 )

   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(time_point_sec_type_test)
   silence_output(false);

   // ------------
   // constructors
   microseconds ms0{0LL};
   microseconds ms1{1LL};
   microseconds ms2{-1LL};
   microseconds ms_max{i64MAX};
   microseconds ms_min{i64MIN};

   time_point tp0{ms0};
   time_point tp1{ms1};
   time_point tp2{ms2};
   time_point tp_max{ms_max};
   time_point tp_min{ms_min};

   /// time_point_sec()/uint32_t sec_since_epoch()const
   REQUIRE_EQUAL( time_point_sec{}.sec_since_epoch(), 0 )

   /// explicit time_point_sec(uint32_t)
   REQUIRE_EQUAL( time_point_sec{u32MAX}.sec_since_epoch(), u32MAX )
   REQUIRE_EQUAL( time_point_sec{u32MAX + 1}.sec_since_epoch(), 0 )

   /// time_point_sec(const time_point&)
   REQUIRE_EQUAL( time_point_sec{tp0}.sec_since_epoch(), 0LL / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp1}.sec_since_epoch(), 1LL / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp2}.sec_since_epoch(), -1LL / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp_max}.sec_since_epoch(), i64MAX / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp_min}.sec_since_epoch(), i64MIN / 1000000LL )

   // -------------------------------
   // static time_point_sec maximum()
   REQUIRE_EQUAL( time_point_sec{}.maximum().utc_seconds, 0xFFFFFFFF )
   REQUIRE_EQUAL( time_point_sec{}.maximum().utc_seconds != 1, true )

   // ---------------------------
   // static time_point_sec min()
   REQUIRE_EQUAL( time_point_sec{}.min().utc_seconds, 0 )
   REQUIRE_EQUAL( time_point_sec{}.min().utc_seconds != 1, true )

   // Millisecond data is lost when using these data types in certain ways.
   // For Example:
   // microseconds mms0{1};
   // eosio::print_f("_count: %\n",mms0._count);
   // time_point ttp0{mms0};
   // eosio::print_f("_count: %\n",ttp0.elapsed._count);
   // time_point_sec ttps0{ttp0};
   // eosio::print_f("utc_seconds: %\n",ttps0.utc_seconds);
   // ttp0 = ttps0;
   // eosio::print_f("_count: %\n",ttp0.elapsed._count);
   // 
   // Also may have found a `print_f` bug/
   // To replicate:
   // eosio::print_f("%", time_point{time_point_sec{tp_max}}.elapsed.count());
   // eosio::print_f("%", 9223372036854775807 / 1000000);
   // --------------------------
   // operator time_point()const
   REQUIRE_EQUAL( (time_point{time_point_sec{tp0}}.elapsed.count()), 0 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp1}}.elapsed.count()), 0 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp2}}.elapsed.count()), 0 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp_max}}.elapsed._count / 1000000), ms_max.count() / 1000000 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp_min}}.elapsed._count / 1000000), ms_min.count() / 1000000 )
   
   // -------------------------------------------
   // time_point_sec operator=(const time_point&)

   // --------------------------------------------------------------------
   // friend bool operator==(const time_point_sec&, const time_point_sec&)
   // friend bool operator!=(const time_point_sec&, const time_point_sec&)
   // friend bool operator<(const time_point_sec&, const time_point_sec&)
   // friend bool operator<=(const time_point_sec&, const time_point_sec&)
   // friend bool operator>(const time_point_sec&, const time_point_sec&)
   // friend bool operator>=(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{tp0} == time_point{tp0}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp1} != time_point{tp0}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp0} < time_point{tp1}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp1} <= time_point{tp1}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp1} > time_point{tp0}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp1} >= time_point{tp1}), true )


   // ------------------------------------
   // time_point_sec& operator+=(uint32_t)
   // time_point_sec& operator+=(microseconds)
   // time_point_sec& operator+=(time_point_sec)
   // time_point_sec& operator-=(uint32_t)
   // time_point_sec& operator-=(microseconds)
   // time_point_sec& operator-=(time_point_sec)
   // time_point_sec operator+(uint32_t)const
   // time_point_sec operator-(uint32_t)const


   // -----------------------------------------------------------------------
   // friend time_point operator+(const time_point_sec&, const microseconds&)
   // friend time_point operator-(const time_point_sec&, const microseconds&)
   // friend microseconds operator-(const time_point_sec&, const time_point_sec&)
   // friend microseconds operator-(const time_point&, const time_point_sec&)


   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(block_timestamp_type_test)
   silence_output(true);

   // ------------
   // constructors

   /// explicit block_timestamp(uint32_t)

   /// block_timestamp(const time_point&)

   /// block_timestamp(const time_point&)

   // --------------------------------
   // static block_timestamp maximum()

   // ----------------------------
   // static block_timestamp min()

   // ----------------------
   // block_timestamp next()

   // -------------------------------
   // time_point to_time_point()const

   // --------------------------
   // operator time_point()const

   // --------------------------------------
   // void set_time_point(const time_point&)

   // ------------------------------------------
   // void set_time_point(const time_point_sec&)

   // ---------------------------------
   // void operator=(const time_point&)

   // ---------------------------------------
   // bool operator==(const block_timestamp&)
   // bool operator!=(const block_timestamp&)
   // bool operator<(const block_timestamp&)
   // bool operator<=(const block_timestamp&)
   // bool operator>(const block_timestamp&)
   // bool operator>=(const block_timestamp&)

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   // EOSIO_TEST(microseconds_type_test);
   // EOSIO_TEST(time_point_type_test);
   EOSIO_TEST(time_point_sec_type_test);
   // EOSIO_TEST(block_timestamp_type_test);
   return has_failed();
}
