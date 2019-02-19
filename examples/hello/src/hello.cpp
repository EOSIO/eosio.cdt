#include <hello.hpp>
ACTION hello::hi( name nm ) {
   print_f("Name : %\n", nm);
}

ACTION hello::check( name nm ) {
   print_f("Name : %\n", nm);
   eosio::check(nm == "hello"_n, "check name not equal to `hello`");
}
