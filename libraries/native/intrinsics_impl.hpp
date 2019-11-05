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

   intrinsic_row(std::string table_key, uint64_t primary_key, const void* buffer, uint32_t buffer_size):
            table_key{ table_key }, primary_key{ primary_key }, buffer_size{ buffer_size} {
      data = std::string((char*)buffer, buffer_size);
   }

   bool operator ==(const intrinsic_row r2) const {
      return table_key == r2.table_key && primary_key == r2.primary_key;
   }

   bool operator <(const intrinsic_row r2) const {
      return primary_key < r2.primary_key;
   }

   std::string table_key;
   uint64_t primary_key;

   std::string data;
   uint32_t buffer_size;
};

union secondary_index_union {
   uint64_t idx64;
   uint128_t idx128;
   // TODO:??
   uint128_t idx256;

   double idxdouble;
   long double idxlongdouble;

};

enum secondary_index_type { empty, idx64, idx128, idx256, idxdouble, idxlongdouble };

struct secondary_index_row {
   secondary_index_type tag;
   secondary_index_union val;
   uint64_t primary_key;

   bool operator ==(const secondary_index_row r2) const {
      if (tag != r2.tag) return false;
      if (primary_key != r2.primary_key) return false;

      switch(tag) {
         case idx64:
            return val.idx64 == r2.val.idx64;
         case idx128:
            return val.idx128 == r2.val.idx128;
         case idx256:
            return val.idx256 == r2.val.idx256;
         case idxdouble:
            return val.idxdouble == r2.val.idxdouble;
         case idxlongdouble:
            return val.idxlongdouble == r2.val.idxlongdouble;
         default:
            return false;
      }

   }

   bool operator <(const secondary_index_row r2) const {
      switch(tag) {
         case idx64:
            return val.idx64 < r2.val.idx64;
         case idx128:
            return val.idx128 < r2.val.idx128;
         case idx256:
            return val.idx256 < r2.val.idx256;
         case idxdouble:
            return val.idxdouble < r2.val.idxdouble;
         case idxlongdouble:
            return val.idxlongdouble < r2.val.idxlongdouble;
         default:
            return false;
      }
   }
};

struct secondary_index {
   secondary_index_type tag;
   std::vector<secondary_index_row> rows;
   std::string table_key;
};


static const eosio::name TESTING_CODE = eosio::name{ "test" };
static const intrinsic_row NULLROW{ "", -1, "", 0 };

static const secondary_index_row SNULLROW{ empty, 0, -1 };



// Primary Index
extern std::map<std::string, std::vector<intrinsic_row>>* key_to_table;
extern std::map<int32_t, std::vector<intrinsic_row>>* iterator_to_table;



// Secondary Indexes
extern std::map<std::string, secondary_index[16]>* key_to_secondary_indexes;
extern std::map<int32_t, secondary_index[16]>* iterator_to_secondary_indexes;

extern std::map<std::string, int32_t>* key_to_iterator_secondary;
extern std::map<int32_t, std::string>* iterator_to_key_secondary;
