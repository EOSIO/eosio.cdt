#include <eosiolib/eosio.hpp>
#include <eosiolib/string.hpp>
#include <string>

using namespace eosio;

CONTRACT rope_tests : public contract{
   public:
      using contract::contract;
#if 1
      ACTION test1() {
         rope r("test string 0");
         r += ", test string 1";
         r += ", test string 2";
         r += ", test string 3";
         r += ", test string 4";
         r += ", test string 5";
         r += ", test string 6";
         r += ", test string 7";
         r.insert(8, "Something else");
         r.insert(4, "(Inserted 1)");
         r.insert(20, "(Inserted 2)");
         
         std::string s("test string 0");
         s += ", test string 1";
         s += ", test string 2";
         s += ", test string 3";
         s += ", test string 4";
         s += ", test string 5";
         s += ", test string 6";
         s += ", test string 7";
         s.insert(8, "Something else");
         s.insert(4, "(Inserted 1)");
         s.insert(20, "(Inserted 2)");

         r = r + r;
         s = s + s;

         eosio::print_f("%\n", r.c_str());
         eosio_assert(false, "something");

         //rope r3 = r + r;
         /*
         rope r2 = r;
         r2 += "hoot";
         r2 += " again";
         r2 += " again";
         */
         /*
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         */
         //auto pp = r.c_str();
         //eosio::print("PP %\n", pp.get());
         //prints_l(pp.get(), r.length());
         //eosio::print_f("%\n", r2.c_str());
      }
#endif
#if 0
      ACTION test2() {
         std::string r("hello");
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r += ", world";
         r += " please!";
         r.insert(40, "Something else");
         r.insert(60, "Something else");
         r.insert(40, "Stupid2");
         /*
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");
         r.insert(40, "Something else");

         r.insert(40, "Something else");
        //r.c_str(buff);
        */
        //eosio::print(r);
        //eosio_assert(false, "somethingelse");
      }
#endif
};

EOSIO_DISPATCH(rope_tests, (test1))//(test2))
