#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>

#include "transfer.hpp" 

using namespace eosio;

volatile void func(uint32_t r, uint32_t s) {
   char* buff = (char*)alloca(s);
   std::memset(buff, 0, s);
   if (--r > 0)
      return func(r, s);
}

class [[eosio::contract]] stack_canary_test : public contract {
   public:
      using contract::contract;

      [[eosio::action]] 
      uint64_t test(uint32_t r, uint32_t s) {
         volatile uint32_t _stack_var = 33;
         func(r, s);
         return reinterpret_cast<uint64_t>(&_stack_var);
      }
};