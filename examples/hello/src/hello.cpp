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

// Checks the input param `nm` and returns serialized std::pair<int, std::string> instance.
[[eosio::action]]
std::pair<int, std::string> hello::checkwithrv( name nm ) {
   print_f("Name : %\n", nm);

   std::pair<int, std::string> results = {0, "NOP"};
   if (nm == "hello"_n) {
      results = {0, "Validation has passed."};
   }
   else {
      results = {1, "Input param `name` not equal to `hello`."};
   }
   return results; // the `set_action_return_value` intrinsic is invoked automatically here
}
