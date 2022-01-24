#include <cxxabi.h>

extern "C" {
[[eosio::wasm_import]] void eosio_assert(unsigned int, const char*);
}

namespace __cxxabiv1 {
extern "C" {
_LIBCXXABI_FUNC_VIS _LIBCXXABI_NORETURN
void __cxa_pure_virtual(void) {
  eosio_assert(false, "Pure virtual function called!");
}

_LIBCXXABI_FUNC_VIS _LIBCXXABI_NORETURN
void __cxa_deleted_virtual(void) {
  eosio_assert(false, "Deleted virtual function called!");
}
} // extern "C"
} // abi
