#include <eosio/eosio.hpp>
#include <eosio/tester.hpp>

using namespace eosio::native;
using namespace eosio;

EOSIO_TEST_BEGIN(signed_int_test)   //test datastream insert/extract signed_int
   //silence_output(true);

   char buffer[32];
   datastream ds(buffer,32);
   signed_int a(44), b((1<<30)+2), c(-35), d(-(1<<30)-2);   //small +, small -, large +, large -
   ds << a << b << c << d;
   ds.seekp(0);
   signed_int aa, bb, cc, dd;
   ds >> aa >> bb >> cc >> dd;
   check(a.value==aa.value, "signed_int datastream extract fails");
   check(b.value==bb.value, "signed_int datastream extract fails");
   check(c.value==cc.value, "signed_int datastream extract fails");
   check(d.value==dd.value, "signed_int datastream extract fails");
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(signed_int_test);
   return has_failed();
}
