#include <cosmwasm/cosmwasm.hpp>
#include "token.hpp"

using namespace cosmwasm;

class token_contract : public contract {
public:
   using contract::contract;

   void instantiate(
      const std::string& name,
      const std::string& symbol,
      uint8_t decimals,
      std::vector<token> initial_balances
   ) {
      auto total_supply = create_accounts(initial_balances);
      _storage.set(
         namespaces_with_key({}, to_bytes("token_info")),
         token_info_response {
            name, symbol, decimals, total_supply
         }.as_bytes()
      );
   }

   void transfer(const std::string& recipient, uint128_t amount) {
      auto address = addr_validate(recipient);
      sub_balance(_info.sender, amount);
      add_balance(address, amount);
   }

   bytes get_balance(const std::string& address) {
      balance_response bres = { 0 };

      auto s = _storage.get(namespaces_with_key({to_bytes("balance")}, to_bytes(address)));
      if (s) {
         bres.balance = from_bytes<uint128_t>(*s);
      }

      return to_bytes(bres);
   }

   bytes get_token_info() {
      return *_storage.get(namespaces_with_key({}, to_bytes("token_info")));
   }


private:
   void add_balance(const std::string& address, const uint128_t& amount) {
      uint128_t balance = 0;

      auto key = namespaces_with_key({to_bytes("balance")}, to_bytes(address));
      auto s = _storage.get(key);
      if (s) {
         balance = from_bytes<uint128_t>(*s);
      }

      uint128_t new_balance = balance + amount;
      check(new_balance > balance);
      _storage.set(key, to_bytes(new_balance));
   }

   void sub_balance(const std::string& address, const uint128_t& amount) {
      uint128_t balance = 0;

      auto key = namespaces_with_key({to_bytes("balance")}, to_bytes(address));
      auto s = _storage.get(key);
      if (s) {
         balance = from_bytes<uint128_t>(*s);
      }

      uint128_t new_balance = balance - amount;
      check(new_balance < balance);
      _storage.set(key, to_bytes(new_balance));
   }

   uint128_t create_accounts(const std::vector<token>& accounts) {
      uint128_t total_supply = 0;
      for (const auto& acc : accounts) {
         total_supply += acc.amount;
         auto address = addr_validate(acc.address);
         add_balance(address, acc.amount);
      }
      return total_supply;
   }
};


extern "C" {
   [[clang::export_name("instantiate")]]
   region* instantiate(region* env_ptr, region* info_ptr, region* msg_ptr) {
      contract_result<response> res;

      auto e = json::from_region<cosmwasm::env>(env_ptr);
      auto i = json::from_region<cosmwasm::message_info>(info_ptr);
      auto m = json::from_region<instantiate_msg>(msg_ptr);
      auto c = token_contract(e, i);

      c.instantiate(m.name, m.symbol, m.decimals, m.initial_balances);

      return json::to_region(res).release();
   }

   [[clang::export_name("execute")]]
   region* execute(region* env_ptr, region* info_ptr, region* msg_ptr) {
      contract_result<response> res;

      auto e = json::from_region<cosmwasm::env>(env_ptr);
      auto i = json::from_region<cosmwasm::message_info>(info_ptr);
      auto m = json::from_region<execute_msg>(msg_ptr);
      auto c = token_contract(e, i);

      std::visit(overloaded {
         [&](const transfer_msg& msg) {
            c.transfer(msg.recipient, msg.amount);
            res.ok.attributes.push_back({"action", "transfer"});
            res.ok.attributes.push_back({"from", i.sender});
            res.ok.attributes.push_back({"to", msg.recipient});
            res.ok.attributes.push_back({"amount", std::to_string(msg.amount)});
         },
      }, m);

      return json::to_region(res).release();
   }

   [[clang::export_name("query")]]
   region* query(region* env_ptr, region* msg_ptr) {
      contract_result<bytes> res;

      auto e = json::from_region<cosmwasm::env>(env_ptr);
      auto m = json::from_region<query_msg>(msg_ptr);
      auto c = token_contract(e, {});

      std::visit(overloaded {
         [&](balance_msg& msg) { res.ok = c.get_balance(msg.address); },
         [&](token_info_msg& msg) { res.ok = c.get_token_info(); },
      }, m);

      return json::to_region(res).release();
   }
}
