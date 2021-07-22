#include <cstdint>

extern "C" {
    volatile uint64_t eosio_contract_name = 0;
    volatile void eosio_set_contract_name(uint64_t n) { eosio_contract_name = n; } // LLVM creates the call to this at the beginning of apply
}
