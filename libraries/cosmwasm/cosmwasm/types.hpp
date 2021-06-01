#pragma once
#include "serialize.hpp"
#include "json.hpp"

namespace cosmwasm {

   typedef std::string addr;

   struct block_info {
      uint64_t height;
      uint64_t time;
      std::string chain_id;

      COSMWASM_SERIALIZE(block_info, (height)(time)(chain_id))
   };

   struct contract_info {
      addr address;

      COSMWASM_SERIALIZE(contract_info, (address))
   };

   struct coin {
      std::string denom;
      uint128_t amount;

      COSMWASM_SERIALIZE(coin, (denom)(amount))
   };

   struct message_info {
      addr sender;
      std::vector<coin> funds;

      COSMWASM_SERIALIZE(message_info, (sender)(funds))
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

   template<typename T>
   struct contract_result {
      T ok;
      std::string error;

      COSMWASM_SERIALIZE(contract_result<T>, (ok)(error))
   };
}
