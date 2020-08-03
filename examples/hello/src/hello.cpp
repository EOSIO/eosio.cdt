#include <hello.hpp>

[[eosio::action]]
void hello::hi( name nm ) {
   print_f("Name : %\n", nm);
}

[[eosio::action]]
void hello::check( name nm ) {
   print_f("Name : %\n", nm);
   eosio::check(nm == "hello"_n, "check name not equal to `hello`");
}

[[eosio::action]]
std::vector<std::string> hello::checkwithrv( name nm ) {
   print_f("Name : %\n", nm);

   std::vector<std::string> results;
   if (nm == "hello"_n) {
      results.push_back("0");
      results.push_back("validation has passed");
   }
   else {
      results.push_back("1");
      results.push_back("name not equal to `hello`");
   }
   return results;
}
