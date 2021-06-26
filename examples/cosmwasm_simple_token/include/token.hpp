#pragma once
#include <cosmwasm/serialize.hpp>

struct token {
   std::string address;
   uint128_t amount;

   COSMWASM_SERIALIZE(token, (address)(amount))
};

struct instantiate_msg {
   std::string name;
   std::string symbol;
   uint8_t decimals;
   std::vector<token> initial_balances;

   COSMWASM_SERIALIZE(instantiate_msg, (name)(symbol)(decimals)(initial_balances))
};

struct transfer_msg {
   std::string recipient;
   uint128_t amount;

   COSMWASM_SERIALIZE2(transfer_msg, "transfer", (recipient)(amount))
};

using execute_msg = std::variant<transfer_msg>;

struct balance_msg {
   std::string address;

   COSMWASM_SERIALIZE2(balance_msg, "balance", (address))
};

struct token_info_msg {
   COSMWASM_SERIALIZE2(token_info_msg, "token_info")
};

using query_msg = std::variant<balance_msg,token_info_msg>;

struct balance_response {
   uint128_t balance;

   COSMWASM_SERIALIZE(balance_response, (balance))
};

struct token_info_response {
   std::string name;
   std::string symbol;
   uint8_t decimals;
   uint128_t total_supply;

   COSMWASM_SERIALIZE(token_info_response, (name)(symbol)(decimals)(total_supply))
};
