/*
 * Regression test for https://github.com/EOSIO/eosio.cdt/issues/582
 *
 * Verifies that separate cpp and hpp files can be compile without `-o`
 */

#include "separate_cpp_hpp.hpp"

void separate_cpp_hpp::act() {
   eosio::print("ok\n");
}
