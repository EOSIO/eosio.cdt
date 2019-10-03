#include <eosio/name.hpp>
#include <eosio/print.hpp>
#include <eosio/types.h>

#include <cstring>
#include <map>
#include <string>
#include <vector>

#pragma once

struct intrinsic_row {
   intrinsic_row() = default;

   intrinsic_row(std::string table_key, uint64_t primary_key, const void* buffer, uint32_t buffer_size, uint64_t secondary_key = 0):
            table_key{ table_key }, primary_key{ primary_key }, buffer_size{ buffer_size}, secondary_key{ secondary_key } {
      data = std::string((char*)buffer, buffer_size);
   }

   bool operator ==(const intrinsic_row r2) const {
      return table_key == r2.table_key && primary_key == r2.primary_key;
   }

   bool operator <(const intrinsic_row r2) const {
      return primary_key < r2.primary_key;
   }

   std::string table_key;

   std::string data;
   uint32_t buffer_size;

   uint64_t primary_key;

   uint64_t secondary_key;
};


static const eosio::name TESTING_CODE = eosio::name{ "test" };
static const intrinsic_row NULLROW{ "", -1, "", 0 };

// Primary Key
extern std::map<std::string, std::vector<intrinsic_row>>* key_to_table;
extern std::map<int32_t, std::vector<intrinsic_row>>* iterator_to_table;

// Secondary Key
extern std::map<int32_t, std::vector<intrinsic_row>>* secondary_iterator_to_table;

