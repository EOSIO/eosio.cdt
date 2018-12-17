#include <hello.hpp>
ACTION hello::hi( name nm ) {
   print_f("Name : %\n", nm);
}

EOSIO_DISPATCH( hello, (hi) )
