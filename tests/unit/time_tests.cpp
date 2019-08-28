/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/tester.hpp>
#include <eosio/time.hpp>

using std::numeric_limits;

using eosio::days;
using eosio::hours;
using eosio::microseconds;
using eosio::milliseconds;
using eosio::minutes;
using eosio::seconds;
using eosio::time_point;
using eosio::time_point_sec;
using eosio::block_timestamp;

static constexpr int64_t i64min = numeric_limits<int64_t>::min(); // -9223372036854775808
static constexpr int64_t i64max = numeric_limits<int64_t>::max(); //  9223372036854775807

static constexpr uint32_t u32min = numeric_limits<uint32_t>::min(); // 0
static constexpr uint32_t u32max = numeric_limits<uint32_t>::max(); // 4294967295

// Definitions in `eosio.cdt/libraries/eosio/time.hpp`
EOSIO_TEST_BEGIN(microseconds_type_test)
   //// explicit microseconds(uint64_t)/int64_t count()
   CHECK_EQUAL( microseconds{}._count, 0ULL )
   CHECK_EQUAL( microseconds{i64max}._count, i64max )
   CHECK_EQUAL( microseconds{i64min}._count, i64min )

   // -----------------------------
   // static microseconds maximum()
   CHECK_EQUAL( microseconds::maximum()._count, microseconds{0x7FFFFFFFFFFFFFFFLL}.count() )

   // ------------------------------------------------------------------------
   // friend microseconds operator+(const  microseconds&, const microseconds&)
   CHECK_EQUAL( (microseconds{0LL} + microseconds{ 1LL}), microseconds{ 1LL} )
   CHECK_EQUAL( (microseconds{1LL} + microseconds{-1LL}), microseconds{ 0LL} )

   // ------------------------------------------------------------------------
   // friend microseconds operator-(const  microseconds&, const microseconds&)
   CHECK_EQUAL( (microseconds{0LL} - microseconds{ 1LL}), microseconds{-1LL} )
   CHECK_EQUAL( (microseconds{1LL} - microseconds{-1LL}), microseconds{ 2LL} )

   // ----------------------------------------------
   // microseconds& operator+=(const  microseconds&)
   CHECK_EQUAL( (microseconds{0LL} += microseconds{ 1LL}), microseconds{ 1LL} )
   CHECK_EQUAL( (microseconds{1LL} += microseconds{-1LL}), microseconds{ 0LL} )

   // ----------------------------------------------
   // microseconds& operator-=(const  microseconds&)
   CHECK_EQUAL( (microseconds{0LL} -= microseconds{ 1LL}), microseconds{-1LL} )
   CHECK_EQUAL( (microseconds{1LL} -= microseconds{-1LL}), microseconds{ 2LL} )

   // -------------------------------------
   // bool operator==(const  microseconds&)
   CHECK_EQUAL( (microseconds{1LL} == microseconds{1LL}), true  )
   CHECK_EQUAL( (microseconds{0LL} == microseconds{1LL}), false )

   // -------------------------------------
   // bool operator!=(const  microseconds&)
   CHECK_EQUAL( (microseconds{1LL} != microseconds{1LL}), false )
   CHECK_EQUAL( (microseconds{0LL} != microseconds{1LL}), true  )

   // -------------------------------------
   // bool operator<(const  microseconds&)
   CHECK_EQUAL( (microseconds{0LL} <  microseconds{1LL}), true  )
   CHECK_EQUAL( (microseconds{1LL} <  microseconds{1LL}), false )

   // -------------------------------------
   // bool operator<=(const  microseconds&)
   CHECK_EQUAL( (microseconds{1LL} <= microseconds{1LL}), true  )
   CHECK_EQUAL( (microseconds{2LL} <= microseconds{1LL}), false )

   // -------------------------------------
   // bool operator>(const  microseconds&)
   CHECK_EQUAL( (microseconds{1LL} >  microseconds{0LL}), true  )
   CHECK_EQUAL( (microseconds{1LL} >  microseconds{1LL}), false )

   // -------------------------------------
   // bool operator>=(const  microseconds&)
   CHECK_EQUAL( (microseconds{1LL} >= microseconds{1LL}), true  )
   CHECK_EQUAL( (microseconds{1LL} >= microseconds{2LL}), false )

   // --------------------
   // int64_t to_seconds()
   CHECK_EQUAL( (microseconds{i64max}.to_seconds()), i64max / 1000000 )

   // -----------------------------------------
   // inline microseconds milliseconds(int64_t)
   CHECK_EQUAL( milliseconds(0LL),  microseconds{0LL} )
   CHECK_EQUAL( milliseconds(1LL),  microseconds{1000LL} )
   CHECK_EQUAL( milliseconds(60LL), microseconds{60LL*1000LL} )

   // ------------------------------------
   // inline microseconds seconds(int64_t)
   CHECK_EQUAL( seconds(0LL),  microseconds{0LL} )
   CHECK_EQUAL( seconds(1LL),  microseconds{1000000LL} )
   CHECK_EQUAL( seconds(60LL), microseconds{60LL*1000000LL} )

   // ------------------------------------
   // inline microseconds minutes(int64_t)
   CHECK_EQUAL( minutes(0LL),  microseconds{0LL} )
   CHECK_EQUAL( minutes(1LL),  microseconds{60LL*1000000LL} )
   CHECK_EQUAL( minutes(60LL), microseconds{60LL*60LL*1000000LL} )

   // ----------------------------------
   // inline microseconds hours(int64_t)
   CHECK_EQUAL( hours(0LL),  microseconds{0LL} )
   CHECK_EQUAL( hours(1LL),  microseconds{60LL*60LL*1000000LL} )
   CHECK_EQUAL( hours(60LL), microseconds{60LL*60LL*60LL*1000000LL} )

   // ---------------------------------
   // inline microseconds days(int64_t)
   CHECK_EQUAL( days(0LL),  microseconds{0LL} )
   CHECK_EQUAL( days(1LL),  microseconds{24LL*60LL*60LL*1000000LL} )
   CHECK_EQUAL( days(60LL), microseconds{24LL*60LL*60LL*60LL*1000000LL} )
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosio/time.hpp`
EOSIO_TEST_BEGIN(time_point_type_test)
   static const microseconds ms0 { 0LL};
   static const microseconds ms1 { 1LL};
   static const microseconds msn1{-1LL};
   static const microseconds ms_min{i64min};
   static const microseconds ms_max{i64max};

   //// explicit time_point(microseconds)
   // microseconds& time_since_epoch()
   CHECK_EQUAL( time_point{ms0}.time_since_epoch(), ms0 )
   CHECK_EQUAL( time_point{ms1}.time_since_epoch(), ms1 )
   CHECK_EQUAL( time_point{ms_min}.time_since_epoch(), ms_min )
   CHECK_EQUAL( time_point{ms_max}.time_since_epoch(), ms_max )

   // --------------------------
   // uint32_t sec_since_epoch()
   CHECK_EQUAL( time_point{ms0}.sec_since_epoch(), 0 )
   CHECK_EQUAL( time_point{ms1}.sec_since_epoch(), 0 )
   CHECK_EQUAL( time_point{microseconds{1000000}}.sec_since_epoch(),   1 )
   CHECK_EQUAL( time_point{microseconds{10000000}}.sec_since_epoch(), 10 )

   // -----------------------------------------
   // time_point operator+(const microseconds&)
   CHECK_EQUAL( (time_point{ms0}  + ms1), time_point{ms1} )
   CHECK_EQUAL( (time_point{msn1} + ms1), time_point{ms0} )

   // ---------------------------------------
   // time_point operator+(const time_point&)
   CHECK_EQUAL( (time_point{ms0}  + time_point{ms1}), time_point{ms1} )
   CHECK_EQUAL( (time_point{msn1} + time_point{ms1}), time_point{ms0} )

   // -----------------------------------------
   // time_point operator-(const microseconds&)
   CHECK_EQUAL( (time_point{ms0} - ms1), time_point{msn1} )
   CHECK_EQUAL( (time_point{ms0} - msn1), time_point{ms1} )

   // -----------------------------------------
   // microseconds operator-(const time_point&)
   CHECK_EQUAL( (time_point{ms0} - time_point{ms1}), msn1 )
   CHECK_EQUAL( (time_point{ms0} - time_point{msn1}), ms1 )

   // -------------------------------------------
   // time_point& operator+=(const microseconds&)
   CHECK_EQUAL( (time_point{ms0} += ms1), time_point{ms1}  )
   CHECK_EQUAL( (time_point{msn1} += ms1), time_point{ms0} )

   // -------------------------------------------
   // time_point& operator-=(const microseconds&)
   CHECK_EQUAL( (time_point{ms0} -= ms1), time_point{msn1} )
   CHECK_EQUAL( (time_point{ms0} -= msn1), time_point{ms1} )

   // ----------------------------------
   // bool operator==(const time_point&)
   CHECK_EQUAL( (time_point{ms0} == time_point{ms0}), true  )
   CHECK_EQUAL( (time_point{ms0} == time_point{ms1}), false )

   // ----------------------------------
   // bool operator!=(const time_point&)
   CHECK_EQUAL( (time_point{ms0} != time_point{ms0}), false )
   CHECK_EQUAL( (time_point{ms0} != time_point{ms1}), true  )

   // ---------------------------------
   // bool operator<(const time_point&)
   CHECK_EQUAL( (time_point{ms0} <  time_point{ms1}), true  )
   CHECK_EQUAL( (time_point{ms1} <  time_point{ms1}), false )

   // ----------------------------------
   // bool operator<=(const time_point&)
   CHECK_EQUAL( (time_point{ms1} <= time_point{ms1}), true  )
   CHECK_EQUAL( (time_point{ms1} <= time_point{ms0}), false )

   // ---------------------------------
   // bool operator>(const time_point&)
   CHECK_EQUAL( (time_point{ms1} > time_point{ms0}), true  )
   CHECK_EQUAL( (time_point{ms1} > time_point{ms1}), false )

   // ----------------------------------
   // bool operator>=(const time_point&)
   CHECK_EQUAL( (time_point{ms1} >= time_point{ms1}), true  )
   CHECK_EQUAL( (time_point{ms0} >= time_point{ms1}), false )
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosio/time.hpp`
EOSIO_TEST_BEGIN(time_point_sec_type_test)
   static const microseconds ms0 { 0LL};
   static const microseconds ms1 { 1LL};
   static const microseconds msn1{-1LL};
   static const microseconds ms_min{i64min};
   static const microseconds ms_max{i64max};

   static const time_point tp0{ms0};
   static const time_point tp1{ms1};
   static const time_point tpn1{msn1};
   static const time_point tp_min{ms_min};
   static const time_point tp_max{ms_max};

   //// time_point_sec()
   //uint32_t sec_since_epoch()const
   CHECK_EQUAL( time_point_sec{}.utc_seconds, 0 )

   //// explicit time_point_sec(uint32_t)
   CHECK_EQUAL( time_point_sec{u32min}.utc_seconds, 0 )
   CHECK_EQUAL( time_point_sec{u32max}.utc_seconds, u32max )
   CHECK_EQUAL( time_point_sec{u32max + 1}.utc_seconds, 0 )

   //// time_point_sec(const time_point&)
   CHECK_EQUAL( time_point_sec{tp0}.utc_seconds, ms0._count / 1000000 )
   CHECK_EQUAL( time_point_sec{tp1}.utc_seconds, ms1._count / 1000000 )
   CHECK_EQUAL( time_point_sec{tpn1}.utc_seconds, msn1._count / 1000000 )

   // ---------------------------
   // static time_point_sec min()
   CHECK_EQUAL( time_point_sec{}.min().utc_seconds == 0, true )
   CHECK_EQUAL( time_point_sec{}.min().utc_seconds != 1, true )

   // -------------------------------
   // static time_point_sec maximum()
   CHECK_EQUAL( time_point_sec{}.maximum().utc_seconds == 0xFFFFFFFF, true )
   CHECK_EQUAL( time_point_sec{}.maximum().utc_seconds != 1, true )

   // --------------------------
   // operator time_point()const
   CHECK_EQUAL( time_point_sec{u32min}.operator time_point(), time_point{microseconds{static_cast<int64_t>(u32min)*1000000}} )
   CHECK_EQUAL( time_point_sec{u32max}.operator time_point(), time_point{microseconds{static_cast<int64_t>(u32max)*1000000}} )

   // -------------------------------------------
   // time_point_sec operator=(const time_point&)
   CHECK_EQUAL( (time_point_sec{} = tp0), time_point_sec{} )
   CHECK_EQUAL( (time_point_sec{} = tp1), time_point_sec{} )
   CHECK_EQUAL( (time_point_sec{} = tp_max), time_point_sec{tp_max} )

   // ---------------------------------------
   // time_point_sec operator+(uint32_t)const
   CHECK_EQUAL( (time_point_sec{} + u32min), time_point_sec{u32min} )
   CHECK_EQUAL( (time_point_sec{} + u32max), time_point_sec{u32max} )

   // -----------------------------------------------------------------------
   // friend time_point operator+(const time_point_sec&, const microseconds&)
   CHECK_EQUAL( (time_point_sec{0} + microseconds{ 1000000LL}), time_point{microseconds{ 1000000LL}} )
   CHECK_EQUAL( (time_point_sec{0} + microseconds{-1000000LL}), time_point{microseconds{-1000000LL}} )
   CHECK_EQUAL( (time_point_sec{1} + microseconds{ 1000000LL}), time_point{microseconds{ 2000000LL}} )
   CHECK_EQUAL( (time_point_sec{1} + microseconds{-1000000LL}), time_point{microseconds{       0LL}} )

   // -----------------------------------------------------------------------
   // friend time_point operator-(const time_point_sec&, const microseconds&)
   CHECK_EQUAL( (time_point_sec{0} - microseconds{ 1000000LL}), time_point{microseconds{-1000000LL}} )
   CHECK_EQUAL( (time_point_sec{0} - microseconds{-1000000LL}), time_point{microseconds{ 1000000LL}} )
   CHECK_EQUAL( (time_point_sec{1} - microseconds{ 1000000LL}), time_point{microseconds{       0LL}} )
   CHECK_EQUAL( (time_point_sec{1} - microseconds{-1000000LL}), time_point{microseconds{ 2000000LL}} )

   // ---------------------------------------------------------------------------
   // friend microseconds operator-(const time_point_sec&, const time_point_sec&)
   CHECK_EQUAL( (time_point_sec{0} - time_point_sec{0}), microseconds{       0LL} )
   CHECK_EQUAL( (time_point_sec{0} - time_point_sec{1}), microseconds{-1000000LL} )
   CHECK_EQUAL( (time_point_sec{1} - time_point_sec{0}), microseconds{ 1000000LL} )
   CHECK_EQUAL( (time_point_sec{1} - time_point_sec{1}), microseconds{       0LL} )

   // -----------------------------------------------------------------------
   // friend microseconds operator-(const time_point&, const time_point_sec&)
   CHECK_EQUAL( (time_point{microseconds{0}} - time_point_sec{0}), microseconds{       0LL} )
   CHECK_EQUAL( (time_point{microseconds{0}} - time_point_sec{1}), microseconds{-1000000LL} )
   CHECK_EQUAL( (time_point{microseconds{1}} - time_point_sec{0}), microseconds{       1LL} )
   CHECK_EQUAL( (time_point{microseconds{1}} - time_point_sec{1}), microseconds{ -999999LL} )

   // ------------------------------------
   // time_point_sec& operator+=(uint32_t)
   CHECK_EQUAL( (time_point_sec{0} += u32min), time_point_sec{u32min} )
   CHECK_EQUAL( (time_point_sec{0} += u32max), time_point_sec{u32max} )

   // ----------------------------------------
   // time_point_sec& operator+=(microseconds)
   CHECK_EQUAL( (time_point_sec{0} += microseconds{      1LL}), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{0} += microseconds{1000000LL}), time_point_sec{1} )
   CHECK_EQUAL( (time_point_sec{1} += microseconds{      1LL}), time_point_sec{1} )
   CHECK_EQUAL( (time_point_sec{1} += microseconds{1000000LL}), time_point_sec{2} )

   // ------------------------------------------
   // time_point_sec& operator+=(time_point_sec)
   CHECK_EQUAL( (time_point_sec{0} += time_point_sec{0}), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{0} += time_point_sec{1}), time_point_sec{1} )
   CHECK_EQUAL( (time_point_sec{1} += time_point_sec{0}), time_point_sec{1} )
   CHECK_EQUAL( (time_point_sec{1} += time_point_sec{1}), time_point_sec{2} )

   // ------------------------------------
   // time_point_sec& operator-=(uint32_t)
   CHECK_EQUAL( (time_point_sec{u32min} -= u32min), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{u32max} -= u32max), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{u32max} -= u32min), time_point_sec{u32max} )

   // ----------------------------------------
   // time_point_sec& operator-=(microseconds)
   CHECK_EQUAL( (time_point_sec{0} -= microseconds{      1LL}), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{1} -= microseconds{1000000LL}), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{2} -= microseconds{1000000LL}), time_point_sec{1} )
   CHECK_EQUAL( (time_point_sec{3} -= microseconds{1000000LL}), time_point_sec{2} )

   // ------------------------------------------
   // time_point_sec& operator-=(time_point_sec)
   CHECK_EQUAL( (time_point_sec{0} -= time_point_sec{0}), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{1} -= time_point_sec{1}), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{2} -= time_point_sec{1}), time_point_sec{1} )
   CHECK_EQUAL( (time_point_sec{3} -= time_point_sec{1}), time_point_sec{2} )

   // ---------------------------------------
   // time_point_sec operator+(uint32_t)const
   CHECK_EQUAL( (time_point_sec{0} + u32min), time_point_sec{u32min} )
   CHECK_EQUAL( (time_point_sec{0} + u32max), time_point_sec{u32max} )

   // ---------------------------------------
   // time_point_sec operator-(uint32_t)const
   CHECK_EQUAL( (time_point_sec{u32min} - u32min), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{u32max} - u32max), time_point_sec{0} )
   CHECK_EQUAL( (time_point_sec{u32max} - u32min), time_point_sec{u32max} )

   // --------------------------------------------------------------------
   // friend bool operator==(const time_point_sec&, const time_point_sec&)
   CHECK_EQUAL( (time_point_sec{1} == time_point_sec{1}), true  )
   CHECK_EQUAL( (time_point_sec{0} == time_point_sec{1}), false )

   // --------------------------------------------------------------------
   // friend bool operator!=(const time_point_sec&, const time_point_sec&)
   CHECK_EQUAL( (time_point_sec{1} != time_point_sec{1}), false )
   CHECK_EQUAL( (time_point_sec{0} != time_point_sec{1}), true  )

   // -------------------------------------------------------------------
   // friend bool operator<(const time_point_sec&, const time_point_sec&)
   CHECK_EQUAL( (time_point_sec{0} <  time_point_sec{1}), true  )
   CHECK_EQUAL( (time_point_sec{1} <  time_point_sec{1}), false )

   // --------------------------------------------------------------------
   // friend bool operator<=(const time_point_sec&, const time_point_sec&)
   CHECK_EQUAL( (time_point_sec{1} <= time_point_sec{1}), true  )
   CHECK_EQUAL( (time_point_sec{2} <= time_point_sec{1}), false )

   // -------------------------------------------------------------------
   // friend bool operator>(const time_point_sec&, const time_point_sec&)
   CHECK_EQUAL( (time_point_sec{1} >  time_point_sec{0}), true  )
   CHECK_EQUAL( (time_point_sec{1} >  time_point_sec{1}), false )

   // --------------------------------------------------------------------
   // friend bool operator>=(const time_point_sec&, const time_point_sec&)
   CHECK_EQUAL( (time_point_sec{1} >= time_point_sec{1}), true  )
   CHECK_EQUAL( (time_point_sec{1} >= time_point_sec{2}), false )
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosio/time.hpp`
EOSIO_TEST_BEGIN(block_timestamp_type_test)
   static const int64_t bt_epoch{946684800000LL};

   static const microseconds ms0{bt_epoch*1000};
   static const microseconds ms1{bt_epoch*1000+500000};
   static const microseconds ms2{bt_epoch*1000+1000000};
   static const microseconds ms_max{i64max};

   static const time_point tp0{ms0};
   static const time_point tp1{ms1};
   static const time_point tp2{ms2};
   static const time_point tp_max{ms_max};

   static const time_point_sec tps0{tp0};
   static const time_point_sec tps1{tp1};
   static const time_point_sec tps2{tp2};

   //// explicit block_timestamp(uint32_t)
   CHECK_EQUAL( block_timestamp{}.slot, 0 )
   CHECK_EQUAL( block_timestamp{u32min}.slot, u32min )
   CHECK_EQUAL( block_timestamp{u32max}.slot, u32max )

   //// block_timestamp(const time_point&)
   // void set_time_point(const time_point&)
   CHECK_EQUAL( block_timestamp{tp0}.slot, 0 )
   CHECK_EQUAL( block_timestamp{tp1}.slot, 1 )
   CHECK_EQUAL( block_timestamp{tp2}.slot, 2 )

   //// block_timestamp(const time_point_sec&)
   // void set_time_point(const time_point_sec&)
   // Infeasible constructor?

   // --------------------------------
   // static block_timestamp maximum()
   CHECK_EQUAL( block_timestamp{}.maximum() == block_timestamp{0xFFFF}, true )
   CHECK_EQUAL( block_timestamp{}.maximum() != block_timestamp{0}, true )

   // ----------------------------
   // static block_timestamp min()
   CHECK_EQUAL( block_timestamp{}.min() == block_timestamp{0}, true )
   CHECK_EQUAL( block_timestamp{}.min() != block_timestamp{0xFFFF}, true )

   // ----------------------
   // block_timestamp next()
   CHECK_EQUAL( (block_timestamp{0}.next()), block_timestamp{1} )
   CHECK_EQUAL( (block_timestamp{1}.next()), block_timestamp{2} )
   CHECK_EQUAL( (block_timestamp{2}.next()), block_timestamp{3} )

   CHECK_ASSERT( "block timestamp overflow", [](){block_timestamp{u32max}.next();} )

   // -------------------------------
   // time_point to_time_point()const
   CHECK_EQUAL( block_timestamp{1}.to_time_point(), time_point{microseconds{(1*500+bt_epoch)*1000}} )
   CHECK_EQUAL( block_timestamp{2}.to_time_point(), time_point{microseconds{(2*500+bt_epoch)*1000}} )
   CHECK_EQUAL( block_timestamp{3}.to_time_point(), time_point{microseconds{(3*500+bt_epoch)*1000}} )

   // --------------------------
   // operator time_point()const
   CHECK_EQUAL( block_timestamp{1}.operator time_point(), time_point{microseconds{(1*500+bt_epoch)*1000}} )
   CHECK_EQUAL( block_timestamp{2}.operator time_point(), time_point{microseconds{(2*500+bt_epoch)*1000}} )
   CHECK_EQUAL( block_timestamp{3}.operator time_point(), time_point{microseconds{(3*500+bt_epoch)*1000}} )

   // ---------------------------------
   // void operator=(const time_point&)
   block_timestamp bt_equal_op{};
   bt_equal_op = tp0;
   CHECK_EQUAL( bt_equal_op, block_timestamp{tp0} )
   bt_equal_op = tp1;
   CHECK_EQUAL( bt_equal_op, block_timestamp{tp1} )
   bt_equal_op = tp2;
   CHECK_EQUAL( bt_equal_op, block_timestamp{tp2} )

   // ---------------------------------------
   // bool operator==(const block_timestamp&)
   CHECK_EQUAL( block_timestamp{1} == block_timestamp{1}, true  )
   CHECK_EQUAL( block_timestamp{0} == block_timestamp{1}, false )

   // ---------------------------------------
   // bool operator!=(const block_timestamp&)
   CHECK_EQUAL( block_timestamp{1} != block_timestamp{1}, false )
   CHECK_EQUAL( block_timestamp{0} != block_timestamp{1}, true  )

   // --------------------------------------
   // bool operator<(const block_timestamp&)
   CHECK_EQUAL( block_timestamp{0} < block_timestamp{1}, true  )
   CHECK_EQUAL( block_timestamp{1} < block_timestamp{1}, false )

   // ---------------------------------------
   // bool operator<=(const block_timestamp&)
   CHECK_EQUAL( block_timestamp{1} <= block_timestamp{1}, true  )
   CHECK_EQUAL( block_timestamp{2} <= block_timestamp{1}, false )

   // ---------------------------------------
   // bool operator>(const block_timestamp&)
   CHECK_EQUAL( block_timestamp{1} > block_timestamp{0}, true  )
   CHECK_EQUAL( block_timestamp{1} > block_timestamp{1}, false )

   // ---------------------------------------
   // bool operator>=(const block_timestamp&)
   CHECK_EQUAL( block_timestamp{1} >= block_timestamp{1}, true  )
   CHECK_EQUAL( block_timestamp{1} >= block_timestamp{2}, false )
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   bool verbose = false;
   if( argc >= 2 && std::strcmp( argv[1], "-v" ) == 0 ) {
      verbose = true;
   }
   silence_output(!verbose);

   EOSIO_TEST(microseconds_type_test);
   EOSIO_TEST(time_point_type_test);
   EOSIO_TEST(time_point_sec_type_test);
   EOSIO_TEST(block_timestamp_type_test);
   return has_failed();
}
