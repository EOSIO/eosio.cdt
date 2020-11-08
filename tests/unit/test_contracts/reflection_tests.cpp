#include <eosio/eosio.hpp>
#include <eosio/reflection.hpp>

using namespace eosio;

struct foo {
   int a;
   float b;
   std::string c;
   EOSIO_CDT_REFLECT(_, a,b,c);
};

struct bar {
   int a;
   float b;
   std::string c;
};

class baz {
   public:
      baz(int a, float b, std::string c)
         : a(a), b(b), c(c) {}
   private:
      int a;
      float b;
      std::string c;
   EOSIO_CDT_REFLECT(_, a,b,c);
};

class fez {
   public:
      fez(int a, float b, std::string c)
         : a(a), b(b), c(c) {}
   private:
      int a;
      float b;
      std::string c;
};

class [[eosio::contract]] reflection_tests : public contract {
   public:
      using contract::contract;

      [[eosio::action]]
      void test1() {
         using mo = meta_object<foo>;
         check(mo::field_count == 3, "should have 3 fields");

         foo f;
         f.a = 13;
         f.b = 32.23;
         f.c = "hello";

         check(mo::get_field<0>(f) == (int)13, "should equal 13");
         check(mo::get_field<1>(f) == (float)32.23, "should equal 32.23");
         check(mo::get_field<2>(f) == std::string("hello"), "should be hello");

         mo::get_field<0>(f) = 42;
         mo::get_field<1>(f) = 42.42;
         mo::get_field<2>(f) = std::string("42");

         check(mo::get_field<0>(f) == (int)42, "should equal 42");
         check(mo::get_field<1>(f) == (float)42.42, "should equal 42.42");
         check(mo::get_field<2>(f) == std::string("42"), "should be 42");

         check(f.a == (int)42, "should equal 42");
         check(f.b == (float)42.42, "should equal 42.42");
         check(f.c == std::string("42"), "should be 42");
      }

      [[eosio::action]]
      void test2() {
         using mo = meta_object<bar>;
         check(mo::field_count == 3, "should have 3 fields");

         bar f;
         f.a = 13;
         f.b = 32.23;
         f.c = "hello";

         check(mo::get_field<0>(f) == (int)13, "should equal 13");
         check(mo::get_field<1>(f) == (float)32.23, "should equal 32.23");
         check(mo::get_field<2>(f) == std::string("hello"), "should be hello");

         mo::get_field<0>(f) = 42;
         mo::get_field<1>(f) = 42.42;
         mo::get_field<2>(f) = std::string("42");

         check(mo::get_field<0>(f) == (int)42, "should equal 42");
         check(mo::get_field<1>(f) == (float)42.42, "should equal 42.42");
         check(mo::get_field<2>(f) == std::string("42"), "should be 42");

         check(f.a == (int)42, "should equal 42");
         check(f.b == (float)42.42, "should equal 42.42");
         check(f.c == std::string("42"), "should be 42");
      }

      [[eosio::action]]
      void test3() {
         using mo = meta_object<baz>;
         check(mo::field_count == 3, "should have 3 fields");

         baz f(13, 32.23, "hello");

         check(mo::get_field<0>(f) == (int)13, "should equal 13");
         check(mo::get_field<1>(f) == (float)32.23, "should equal 32.23");
         check(mo::get_field<2>(f) == std::string("hello"), "should be hello");

         mo::get_field<0>(f) = 42;
         mo::get_field<1>(f) = 42.42;
         mo::get_field<2>(f) = std::string("42");

         check(mo::get_field<0>(f) == (int)42, "should equal 42");
         check(mo::get_field<1>(f) == (float)42.42, "should equal 42.42");
         check(mo::get_field<2>(f) == std::string("42"), "should be 42");
      }

      [[eosio::action]]
      void test4() {
         using mo = meta_object<fez>;
         check(mo::field_count == 3, "should have 3 fields");

         fez f(13, 32.23, "hello");

         check(mo::get_field<0>(f) == (int)13, "should equal 13");
         check(mo::get_field<1>(f) == (float)32.23, "should equal 32.23");
         check(mo::get_field<2>(f) == std::string("hello"), "should be hello");

         mo::get_field<0>(f) = 42;
         mo::get_field<1>(f) = 42.42;
         mo::get_field<2>(f) = std::string("42");

         check(mo::get_field<0>(f) == (int)42, "should equal 42");
         check(mo::get_field<1>(f) == (float)42.42, "should equal 42.42");
         check(mo::get_field<2>(f) == std::string("42"), "should be 42");
      }

};
