#include <eosio/eosio.hpp>
#include <eosio/table.hpp>
#include <eosio/contract.hpp>
#include <eosio/action.hpp>
#include <eosio/crypto.hpp>
#include <eosio/fixed_bytes.hpp>
#include <eosio/privileged.hpp>
#include <eosio/producer_schedule.hpp>

#include <eosio/asset.hpp>
#include <eosio/binary_extension.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>
/* 
 tests which create aliases and indirect calls for host functions.
 at least one host function called within a C++ lambda.  It can be a trivial lambda which simply calls the host function and nothing else.
 at least one host function "renamed" with a using directive and called via the alternative name.
 at least one host function "renamed" with a #define and called using that alternative name.
 Possibly in two different versions, one which #defines just the function name and one which #defines both the function name and the parameters passed to it.
 Yes, both versions I think.  It may not be functionally different to the compiler but it looks different to a human, so let's try it.
 And finally, combine all four in a chain of aliases
*/

extern "C" __attribute__((eosio_wasm_import)) void set_resource_limit(int64_t, int64_t, int64_t);
extern "C" __attribute__((eosio_wasm_import)) void foo_bar(int64_t, int64_t, int64_t);

#define ACTION_TYPE  [[eosio::action, eosio::read_only]]

using func = void (*)(int64_t, int64_t, int64_t);

func srl_g1 = set_resource_limit;
func srl_g2 = set_resource_limit;
func srl_g3;
func fb_g = foo_bar;

class [[eosio::contract]] hf_indirect_call_tests : public eosio::contract {
public:
   using contract::contract;
   func srl_m1 = set_resource_limit;
   func srl_m2 = set_resource_limit;
   func srl_m3;
   func fb_m = foo_bar;

   ACTION_TYPE
   bool tlambda() {
      auto set_rsc = [](){
         set_resource_limit(0, 0, 0);
      };
      set_rsc();
      return true;
   }

   ACTION_TYPE
   bool tlambdap() {
      auto set_rsc = [](int64_t a, int64_t b, int64_t c){
         set_resource_limit(a, b, c);
      };
      set_rsc(0, 0, 0);
      return true;
   }

   ACTION_TYPE
   bool tlambdap2(int64_t a, int64_t b, int64_t c) {
      auto set_rsc = [=](int64_t x, int64_t y, int64_t z){
         set_resource_limit(x, y, z);
      };
      set_rsc(a, b, c);
      return true;
   }

   ACTION_TYPE
   bool talias () {
      func srl = set_resource_limit;
      srl(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasg1() {
      srl_g1(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasg2 () {
      srl_g2 = foo_bar;
      srl_g2(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasg3 () {
      srl_g3 = set_resource_limit;
      srl_g3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasg3re1 () {
      srl_g3 = foo_bar;
      srl_g3(0,0,0);
      srl_g3 = set_resource_limit;
      srl_g3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasg3re2 () {
      srl_g3 = set_resource_limit;
      srl_g3 = foo_bar;
      srl_g3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasg3re3 () {
      srl_g3 = foo_bar;
      srl_g3 = set_resource_limit;
      srl_g3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasgr () {
      fb_g(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasm1 () {
      srl_m1(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasm2 () {
      srl_m2 = foo_bar;
      srl_m2(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasm3 () {
      srl_m3 = set_resource_limit;
      srl_m3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasm3re1 () {
      srl_m3 = foo_bar;
      srl_m3(0,0,0);
      srl_m3 = set_resource_limit;
      srl_m3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasm3re2 () {
      srl_m3 = set_resource_limit;
      srl_m3 = foo_bar;
      srl_m3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasm3re3 () {
      srl_m3 = foo_bar;
      srl_m3 = set_resource_limit;
      srl_m3(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasmr () {
      fb_m(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool taliasma () {
      func srl_a1, srl_a2 = set_resource_limit;
      srl_a2(0,0,0);
   }

   #define setfun set_resource_limit
   ACTION_TYPE
   bool tdefine(){
      setfun(0,0,0);
      return true;
   }

   #define setfunp() set_resource_limit(0,0,0)
   ACTION_TYPE
   bool tdefinep(){
      setfunp();
      return true;
   }

   #define setfunpi(a, b, c) set_resource_limit(a,b,c)
   ACTION_TYPE
   bool tdefinepi(){
      setfunpi(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool combine(){
      func srl = set_resource_limit;
      srl(0,0,0);
      setfun(0,0,0);
      setfunp();
      setfunpi(0,0,0);
      return true;
   }

   ACTION_TYPE
   bool combinea(){
      tlambdap();
      tlambdap2(0,0,0);
      talias();
      tdefine();
      tdefinep();
      tdefinepi();
      combine();
      return true;
   }

   ACTION_TYPE
   bool combinec(){
      auto set_rsc = [](){
         func _srl = set_resource_limit;
         #define mysrl  _srl
         mysrl(0, 0, 0);
      };
      set_rsc();
      return true; 
   }

   ACTION_TYPE
   bool combinecp(){
      auto set_rsc = [](int64_t a, int64_t b, int64_t c){
         func _srl = set_resource_limit;
         #define mysrl  _srl
         mysrl(a, b, c);
      };
      set_rsc(0,0,0);
      return true; 
   }
};
