/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/native/tester.hpp>
#include <eosiolib/time.hpp>

using eosio::microseconds;
using eosio::time_point;
using eosio::time_point_sec;
using eosio::block_timestamp;

const uint32_t u32min = std::numeric_limits<uint32_t>::min(); // 0
const uint32_t u32max = std::numeric_limits<uint32_t>::max(); // 18446744073709551615

const int64_t i64min = std::numeric_limits<int64_t>::min(); // -9223372036854775808
const int64_t i64max = std::numeric_limits<int64_t>::max(); //  9223372036854775807

// Definitions in `eosio.cdt/libraries/eosiolib/time.hpp`
EOSIO_TEST_BEGIN(microseconds_type_test)
   silence_output(false);

   //// explicit microseconds(uint64_t)/int64_t count()
   REQUIRE_EQUAL( microseconds{}.count(), 0ULL )
   REQUIRE_EQUAL( microseconds{i64max}.count(), i64max )
   REQUIRE_EQUAL( microseconds{i64min}.count(), i64min )

   // -----------------------------
   // static microseconds maximum()
   REQUIRE_EQUAL( microseconds::maximum().count(), microseconds{0x7FFFFFFFFFFFFFFFLL}.count() )

   // ------------------------------------------------------------------------
   // friend microseconds operator+(const  microseconds&, const microseconds&)
   REQUIRE_EQUAL( (microseconds{0LL} + microseconds{ 1LL}), microseconds{ 1LL} )
   REQUIRE_EQUAL( (microseconds{1LL} + microseconds{-1LL}), microseconds{ 0LL} )

   // ------------------------------------------------------------------------
   // friend microseconds operator-(const  microseconds&, const microseconds&)
   REQUIRE_EQUAL( (microseconds{0LL} - microseconds{ 1LL}), microseconds{-1LL} )
   REQUIRE_EQUAL( (microseconds{1LL} - microseconds{-1LL}), microseconds{ 2LL} )

   // ----------------------------------------------
   // microseconds& operator+=(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{0LL} += microseconds{ 1LL}), microseconds{ 1LL} )
   REQUIRE_EQUAL( (microseconds{1LL} += microseconds{-1LL}), microseconds{ 0LL} )

   // ----------------------------------------------
   // microseconds& operator-=(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{0LL} -= microseconds{ 1LL}), microseconds{-1LL} )
   REQUIRE_EQUAL( (microseconds{1LL} -= microseconds{-1LL}), microseconds{ 2LL} )

   // -------------------------------------
   // bool operator==(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{1LL} == microseconds{1LL}), true )
   REQUIRE_EQUAL( (microseconds{0LL} == microseconds{1LL}), false )

   // -------------------------------------
   // bool operator!=(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{1LL} != microseconds{0LL}), true )
   REQUIRE_EQUAL( (microseconds{0LL} != microseconds{0LL}), false )

   // -------------------------------------
   // bool operator<(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{0LL} <  microseconds{1LL}), true )
   REQUIRE_EQUAL( (microseconds{1LL} <  microseconds{1LL}), false )

   // -------------------------------------
   // bool operator<=(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{1LL} <= microseconds{1LL}), true )
   REQUIRE_EQUAL( (microseconds{2LL} <= microseconds{1LL}), false )

   // -------------------------------------
   // bool operator>(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{1LL} >  microseconds{0LL}), true )
   REQUIRE_EQUAL( (microseconds{1LL} >  microseconds{1LL}), false )

   // -------------------------------------
   // bool operator>=(const  microseconds&)
   REQUIRE_EQUAL( (microseconds{1LL} >= microseconds{1LL}), true )
   REQUIRE_EQUAL( (microseconds{1LL} >= microseconds{2LL}), false )

   // --------------------
   // int64_t to_seconds()
   REQUIRE_EQUAL( (microseconds{i64min-1}.to_seconds()), i64max / 1000000 )
   REQUIRE_EQUAL( (microseconds{i64max}.to_seconds()), i64max / 1000000 )

   // -----------------------------------------
   // inline microseconds milliseconds(int64_t)
   REQUIRE_EQUAL( eosio::milliseconds(0LL), microseconds{0LL} )
   REQUIRE_EQUAL( eosio::milliseconds(1LL), microseconds{1000LL} )
   REQUIRE_EQUAL( eosio::milliseconds(60LL), microseconds{60LL*1000LL} )

   // ------------------------------------
   // inline microseconds seconds(int64_t)
   REQUIRE_EQUAL( eosio::seconds(0LL), microseconds{0LL} )
   REQUIRE_EQUAL( eosio::seconds(1LL), microseconds{1000000LL} )
   REQUIRE_EQUAL( eosio::seconds(60LL), microseconds{60LL*1000000LL} )

   // ------------------------------------
   // inline microseconds minutes(int64_t)
   REQUIRE_EQUAL( eosio::minutes(0LL), microseconds{0LL} )
   REQUIRE_EQUAL( eosio::minutes(1LL), microseconds{60LL*1000000LL} )
   REQUIRE_EQUAL( eosio::minutes(60LL), microseconds{60LL*60LL*1000000LL} )

   // ----------------------------------
   // inline microseconds hours(int64_t)
   REQUIRE_EQUAL( eosio::hours(0LL), microseconds{0LL} )
   REQUIRE_EQUAL( eosio::hours(1LL), microseconds{60LL*60LL*1000000LL} )
   REQUIRE_EQUAL( eosio::hours(60LL), microseconds{60LL*60LL*60LL*1000000LL} )

   // ---------------------------------
   // inline microseconds days(int64_t)
   REQUIRE_EQUAL( eosio::days(0LL), microseconds{0LL} )
   REQUIRE_EQUAL( eosio::days(1LL), microseconds{24LL*60LL*60LL*1000000LL} )
   REQUIRE_EQUAL( eosio::days(60LL), microseconds{24LL*60LL*60LL*60LL*1000000LL} )
   
   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(time_point_type_test)
   silence_output(false);

   microseconds ms0{ 0LL};
   microseconds ms1{ 1LL};
   microseconds ms2{-1LL};
   microseconds ms_max{i64max};
   microseconds ms_min{i64min};
   
   //// explicit time_point(microseconds)/microseconds& time_since_epoch()
   REQUIRE_EQUAL( time_point{ms0}.time_since_epoch(), ms0 )
   REQUIRE_EQUAL( time_point{ms1}.time_since_epoch(), ms1 )

   // --------------------------
   // uint32_t sec_since_epoch()
   REQUIRE_EQUAL( time_point{ms0}.sec_since_epoch(), 0 / 1000000 )
   REQUIRE_EQUAL( time_point{ms1}.sec_since_epoch(), 1 / 1000000 )
   REQUIRE_EQUAL( time_point{ms_max}.sec_since_epoch(), i64max / 1000000 )
   REQUIRE_EQUAL( time_point{ms_min}.sec_since_epoch(), i64min / 1000000 )

   // ------------------------------------------
   // time_point operator+(const  microseconds&)
   REQUIRE_EQUAL( (time_point{ms0} + ms1), time_point{ms1} )
   REQUIRE_EQUAL( (time_point{ms2} + ms1), time_point{ms0} )

   // ----------------------------------------
   // time_point operator+(const  time_point&)
   REQUIRE_EQUAL( (time_point{ms0} + time_point{ms1}), time_point{ms1} )
   REQUIRE_EQUAL( (time_point{ms2} + time_point{ms1}), time_point{ms0} )

   // ------------------------------------------
   // time_point operator-(const  microseconds&)
   REQUIRE_EQUAL( (time_point{ms0} - ms1), time_point{ms2} )
   REQUIRE_EQUAL( (time_point{ms0} - ms2), time_point{ms1} )

   // ------------------------------------------
   // microseconds operator-(const  time_point&)
   REQUIRE_EQUAL( (time_point{ms0} - time_point{ms1}), ms2 )
   REQUIRE_EQUAL( (time_point{ms0} - time_point{ms2}), ms1 )

   // --------------------------------------------
   // time_point& operator+=(const  microseconds&)
   REQUIRE_EQUAL( (time_point{ms0} += ms1), time_point{ms1} )
   REQUIRE_EQUAL( (time_point{ms2} += ms1), time_point{ms0} )
   
   // --------------------------------------------
   // time_point& operator-=(const  microseconds&)
   REQUIRE_EQUAL( (time_point{ms0} -= ms1), time_point{ms2} )
   REQUIRE_EQUAL( (time_point{ms0} -= ms2), time_point{ms1} )

   // ----------------------------------
   // bool operator==(const time_point&)
   REQUIRE_EQUAL( (time_point{ms0} == time_point{ms0}), true )
   REQUIRE_EQUAL( (time_point{ms0} == time_point{ms1}), false )

   // ----------------------------------
   // bool operator!=(const time_point&)
   REQUIRE_EQUAL( (time_point{ms1} != time_point{ms0}), true )
   REQUIRE_EQUAL( (time_point{ms1} != time_point{ms1}), false )

   // ----------------------------------
   // bool operator<(const  time_point&)
   REQUIRE_EQUAL( (time_point{ms0} <  time_point{ms1}), true )
   REQUIRE_EQUAL( (time_point{ms1} <  time_point{ms1}), false )

   // ----------------------------------
   // bool operator<=(const time_point&)
   REQUIRE_EQUAL( (time_point{ms1} <= time_point{ms1}), true )
   REQUIRE_EQUAL( (time_point{ms1} <= time_point{ms0}), false )

   // ----------------------------------
   // bool operator>(const  time_point&)
   REQUIRE_EQUAL( (time_point{ms1} > time_point{ms0}), true )
   REQUIRE_EQUAL( (time_point{ms1} > time_point{ms1}), false )

   // ----------------------------------
   // bool operator>=(const time_point&)
   REQUIRE_EQUAL( (time_point{ms1} >= time_point{ms1}), true )
   REQUIRE_EQUAL( (time_point{ms0} >= time_point{ms1}), false )

   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(time_point_sec_type_test)
   silence_output(true);

   microseconds ms0{ 0LL};
   microseconds ms1{ 1LL};
   microseconds ms2{-1LL};
   microseconds ms_max{i64max};
   microseconds ms_min{i64min};

   time_point tp0{ms0};
   time_point tp1{ms1};
   time_point tp2{ms2};
   time_point tp_max{ms_max};
   time_point tp_min{ms_min};

   //// time_point_sec()/uint32_t sec_since_epoch()const
   REQUIRE_EQUAL( time_point_sec{}.sec_since_epoch(), 0 )

   //// explicit time_point_sec(uint32_t)
   REQUIRE_EQUAL( time_point_sec{u32max}.sec_since_epoch(), u32max )
   REQUIRE_EQUAL( time_point_sec{u32max + 1}.sec_since_epoch(), 0 )

   //// time_point_sec(const time_point&)
   REQUIRE_EQUAL( time_point_sec{tp0}.sec_since_epoch(),  0LL / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp1}.sec_since_epoch(),  1LL / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp2}.sec_since_epoch(), -1LL / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp_max}.sec_since_epoch(), i64max / 1000000LL )
   REQUIRE_EQUAL( time_point_sec{tp_min}.sec_since_epoch(), i64min / 1000000LL )

   // -------------------------------
   // static time_point_sec maximum()
   REQUIRE_EQUAL( time_point_sec{}.maximum().utc_seconds, 0xFFFFFFFF )
   REQUIRE_EQUAL( time_point_sec{}.maximum().utc_seconds != 1, true )

   // ---------------------------
   // static time_point_sec min()
   REQUIRE_EQUAL( time_point_sec{}.min().utc_seconds, 0 )
   REQUIRE_EQUAL( time_point_sec{}.min().utc_seconds != 1, true )

   // --------------------------
   // operator time_point()const
   REQUIRE_EQUAL( (time_point{time_point_sec{tp0}}.elapsed.count()), 0 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp1}}.elapsed.count()), 0 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp2}}.elapsed.count()), 0 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp_max}}.elapsed.count() / 1000000), ms_max.count() / 1000000 )
   REQUIRE_EQUAL( (time_point{time_point_sec{tp_min}}.elapsed.count() / 1000000), ms_min.count() / 1000000 )
   
   // -------------------------------------------
   // time_point_sec operator=(const time_point&)
   REQUIRE_EQUAL( (time_point_sec{} = time_point{tp0}), time_point_sec{} )
   REQUIRE_EQUAL( (time_point_sec{} = time_point{tp1}), time_point_sec{} )
   REQUIRE_EQUAL( (time_point_sec{} = time_point{tp_max}), time_point_sec{tp_max} )

   // ---------------------------------------
   // time_point_sec operator+(uint32_t)const
   REQUIRE_EQUAL( (time_point_sec{} + u32min), time_point_sec{u32min} )
   REQUIRE_EQUAL( (time_point_sec{} + u32max), time_point_sec{u32max} )

   // -----------------------------------------------------------------------
   // friend time_point operator+(const time_point_sec&, const microseconds&)
   REQUIRE_EQUAL( (time_point_sec{} + ms1), time_point{tp1} )
   REQUIRE_EQUAL( (time_point_sec{} + ms2), time_point{tp2} )

   // ---------------------------------------
   // time_point_sec operator-(uint32_t)const
   REQUIRE_EQUAL( (time_point_sec{u32max} - u32max), time_point_sec{} )
   REQUIRE_EQUAL( (time_point_sec{u32min} - 1UL), time_point_sec{u32max} )
   
   // -----------------------------------------------------------------------
   // friend time_point operator-(const time_point_sec&, const microseconds&)
   // REQUIRE_EQUAL( (time_point_sec{u32max} - microseconds{i32max}), time_point{u32max-i32max} )
   // REQUIRE_EQUAL( (time_point_sec{u32min} - microseconds{}), time_point{} ) // new
   
   // // ---------------------------------------------------------------------------
   // // friend microseconds operator-(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{} - time_point_sec{}), microseconds{} )
   // REQUIRE_EQUAL( (time_point_sec{} - time_point_sec{}), microseconds{} ) // new
   
   // // -----------------------------------------------------------------------
   // // friend microseconds operator-(const time_point&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point{} - time_point_sec{}), microseconds{} )
   // REQUIRE_EQUAL( (time_point{} - time_point_sec{}), microseconds{} ) // new

   // // ------------------------------------
   // // time_point_sec& operator+=(uint32_t)
   // REQUIRE_EQUAL( (time_point_sec{} += i64max), time_point_sec{i64max} )
   // REQUIRE_EQUAL( (time_point_sec{} += i64max), time_point_sec{i64max} ) // new

   // // ----------------------------------------
   // // time_point_sec& operator+=(microseconds)
   // REQUIRE_EQUAL( (time_point_sec{} += microseconds{i64max}), time_point_sec{tp_max} )
   // REQUIRE_EQUAL( (time_point_sec{} += microseconds{i64max}), time_point_sec{tp_max} ) // new

   // // ------------------------------------------
   // // time_point_sec& operator+=(time_point_sec)
   // REQUIRE_EQUAL( (time_point_sec{} += time_point_sec{i64max}), time_point_sec{i64max} )
   // REQUIRE_EQUAL( (time_point_sec{} += time_point_sec{i64max}), time_point_sec{i64max} ) // new

   // // ------------------------------------
   // // time_point_sec& operator-=(uint32_t)
   // REQUIRE_EQUAL( (time_point_sec{i64max} -= i64max), time_point_sec{} )
   // REQUIRE_EQUAL( (time_point_sec{i64max} -= i64max), time_point_sec{} ) // new

   // // ----------------------------------------
   // // time_point_sec& operator-=(microseconds)
   // REQUIRE_EQUAL( (time_point_sec{i64max} -= microseconds{i64max}), time_point_sec{i64max - (i64max / 1000000LL)} )
   // REQUIRE_EQUAL( (time_point_sec{i64max} -= microseconds{i64max}), time_point_sec{i64max - (i64max / 1000000LL)} ) // new

   // // ------------------------------------------
   // // time_point_sec& operator-=(time_point_sec)
   // REQUIRE_EQUAL( (time_point_sec{i64max} -= time_point_sec{i64max}), time_point_sec{} )
   // REQUIRE_EQUAL( (time_point_sec{i64max} -= time_point_sec{i64max}), time_point_sec{} ) // new

   // // --------------------------------------------------------------------
   // // friend bool operator==(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{tp0} == time_point{tp0}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp0} == time_point{tp0}), true ) // new

   // // --------------------------------------------------------------------
   // // friend bool operator!=(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{tp_max} != time_point{tp0}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp_max} != time_point{tp0}), true ) // new

   // // -------------------------------------------------------------------
   // // friend bool operator<(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{tp0} < time_point{tp_max}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp0} < time_point{tp_max}), true ) // new

   // // --------------------------------------------------------------------
   // // friend bool operator<=(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{tp0} < time_point{tp_max}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp0} < time_point{tp_max}), true ) // new

   // // -------------------------------------------------------------------
   // // friend bool operator>(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{tp_max} > time_point{tp0}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp_max} > time_point{tp0}), true ) // new

   // // --------------------------------------------------------------------
   // // friend bool operator>=(const time_point_sec&, const time_point_sec&)
   // REQUIRE_EQUAL( (time_point_sec{tp_max} >= time_point{tp_max}), true )
   // REQUIRE_EQUAL( (time_point_sec{tp_max} >= time_point{tp_max}), true ) // new

   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(block_timestamp_type_test)
   silence_output(false);

   microseconds ms0{ 0LL};
   microseconds ms1{ 1LL};
   microseconds ms2{-1LL};
   microseconds ms_max{i64max};
   microseconds ms_min{i64min};

   time_point tp0{ms0};
   time_point tp1{ms1};
   time_point tp2{ms2};
   time_point tp_max{ms_max};
   time_point tp_min{ms_min};

   time_point_sec tps0{tp0};
   time_point_sec tps1{tp1};
   time_point_sec tps2{tp2};
   time_point_sec tps_max{tp_max};
   time_point_sec tps_min{tp_min};

   //// explicit block_timestamp(uint32_t)
   REQUIRE_EQUAL( block_timestamp{}.slot == block_timestamp{0}.slot, true )

   // NOTE: No invariant established. `block_timestamp` gets wonky with certain inputs.
   // Steps to reproduce:
   // microseconds ms0{0LL};
   // time_point tp0{ms0};
   // block_timestamp{tp0};
   // eosio::print_f("%", block_timestamp.slot); // output: 18446744071816182016
   // Reason for erros:
   // set_time_point(time_point{microseconds{0LL}});
   // int64_t micro_since_epoch = t.time_since_epoch().count(); // 0
   // int64_t msec_since_epoch  = micro_since_epoch / 1000;     // 0 / 1000 = 0
   // slot = uint32_t(( msec_since_epoch - block_timestamp_epoch ) / int64_t(block_interval_ms)); // 0 - 946684800000ll / 500
   
   //// block_timestamp(const time_point&)
   REQUIRE_EQUAL( block_timestamp{tp0}.slot == block_timestamp{0}.slot, true )

   //// block_timestamp(const time_point_sec&)
   REQUIRE_EQUAL( block_timestamp{tps0}.slot == block_timestamp{0}.slot, true )

   // --------------------------------
   // static block_timestamp maximum()
   REQUIRE_EQUAL( block_timestamp{}.maximum(), block_timestamp{0xFFFF} )

   // ----------------------------
   // static block_timestamp min()
   REQUIRE_EQUAL( block_timestamp{}.min(), block_timestamp{0} )

   // ----------------------
   // block_timestamp next()
   REQUIRE_EQUAL( (block_timestamp{}.next()), block_timestamp{1} )

   // NOTE: This function will never throw unless the value lands on `0`
   // For example:
   // block_timestamp{std::numeric_limits<uint32_t>::max();
   // REQUIRE_ASSERT( "block timestamp overflow", ([]() {block_timestamp{std::numeric_limits<uint32_t>::max()-1}.next();}) );

   // ----------------------------------------------------------
   // time_point to_time_point()const/operator time_point()const
   block_timestamp bt0{tps0};
   block_timestamp bt1{tps1};
   block_timestamp bt2{tps2};
   block_timestamp bt_max{tps_max};
   block_timestamp bt_min{tps_min};

   REQUIRE_EQUAL( (bt0.to_time_point()), static_cast<time_point>(tps0) )
   REQUIRE_EQUAL( (bt1.to_time_point()), static_cast<time_point>(tps1) )
   REQUIRE_EQUAL( (bt2.to_time_point()), static_cast<time_point>(tps2) )
   REQUIRE_EQUAL( (bt_max.to_time_point()), static_cast<time_point>(tps_max) )
   REQUIRE_EQUAL( (bt_min.to_time_point()), static_cast<time_point>(tps_min) )

   // --------------------------------------
   // void set_time_point(const time_point&)
   // REQUIRE_EQUAL( bt0.set_time_point{tp0},  )

   // ------------------------------------------
   // void set_time_point(const time_point_sec&)

   // ---------------------------------
   // void operator=(const time_point&)

   // ---------------------------------------
   // bool operator==(const block_timestamp&)
   REQUIRE_EQUAL( bt0 == block_timestamp{tps0}, true )
   REQUIRE_EQUAL( bt0 == block_timestamp{tps0}, true ) // new
   
   // bool operator!=(const block_timestamp&)
   REQUIRE_EQUAL( bt_max != block_timestamp{tps1}, true )
   REQUIRE_EQUAL( bt_max != block_timestamp{tps1}, true ) // new
   
   // bool operator<(const block_timestamp&)
   REQUIRE_EQUAL( bt0 < block_timestamp{tps_max}, true )
   REQUIRE_EQUAL( bt0 < block_timestamp{tps_max}, true ) // new
   
   // bool operator<=(const block_timestamp&)
   REQUIRE_EQUAL( bt_max <= block_timestamp{tps_max}, true )
   REQUIRE_EQUAL( bt_max <= block_timestamp{tps_max}, true ) // new
   
   // bool operator>(const block_timestamp&)
   REQUIRE_EQUAL( bt_max > block_timestamp{tps0}, true )
   REQUIRE_EQUAL( bt_max > block_timestamp{tps0}, true ) // new
   
   // bool operator>=(const block_timestamp&)
   REQUIRE_EQUAL( bt_max >= block_timestamp{bt_max}, true )
   REQUIRE_EQUAL( bt_max >= block_timestamp{bt_max}, true ) // new
   
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   // EOSIO_TEST(microseconds_type_test);
   // EOSIO_TEST(time_point_type_test);
   EOSIO_TEST(time_point_sec_type_test);
   // EOSIO_TEST(block_timestamp_type_test);
   return has_failed();
}
