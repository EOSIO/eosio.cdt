#pragma once
#include "serialize.hpp"
#include "json.hpp"

namespace cosmwasm {

   typedef std::string human_addr;

   struct block_info {
      uint64_t height;
      uint64_t time;
      uint64_t time_nanos;
      std::string chain_id;

      COSMWASM_SERIALIZE(block_info, (height)(time)(time_nanos)(chain_id))
   };

   struct contract_info {
      human_addr address;

      COSMWASM_SERIALIZE(contract_info, (address))
   };

   struct coin {
      std::string denom;
      uint128_t amount;

      COSMWASM_SERIALIZE(coin, (denom)(amount))
   };

   struct message_info {
      human_addr sender;
      std::vector<coin> sent_funds;

      COSMWASM_SERIALIZE(message_info, (sender)(sent_funds))
   };

   struct env {
      block_info block;
      contract_info contract;

      COSMWASM_SERIALIZE(env, (block)(contract))
   };

   struct attribute {
      std::string key;
      std::string value;

      COSMWASM_SERIALIZE(attribute, (key)(value))
   };

   struct response {
      std::vector<std::string> sub_messages;
      std::vector<std::string> messages;
      std::vector<attribute> attributes;
      std::optional<bytes> data;

      COSMWASM_SERIALIZE(response, (sub_messages)(messages)(attributes)(data))
   };

   struct contract_result {
      response ok;
      std::string error;

      COSMWASM_SERIALIZE(contract_result, (ok)(error))
   };
}
