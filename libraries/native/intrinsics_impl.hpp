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
   // uint128_t idx256;

   double idxdouble;
   long double idxlongdouble;

};

enum secondary_index_type { empty, idx64, idx128, /*idx256,*/ idxdouble, idxlongdouble };

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
            /*
         case idx256:
            return val.idx256 == r2.val.idx256;
            */
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
            /*
         case idx256:
            return val.idx256 < r2.val.idx256;
            */
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


/*
template<typename ObjectType,
            typename SecondaryKeyProxy = typename std::add_lvalue_reference<ObjectType>::type,
            typename SecondaryKeyProxyConst = typename std::add_lvalue_reference<typename std::add_const<ObjectType>::type>::type
         >
class generic_index {
   std::vector<ObjectType> rows;
   std::string table_key;
};

extern generic_index<uint64_t> gi_64;
typedef std::array<uint128_t, 2> key256_t;
extern generic_index<key256_t, uint128_t*, const uint128_t*> gi_256;
*/

template <typename T>
T& selectMember(secondary_index_union& u)
{
    return *((T*)&u);
}

struct secondary_index_store {
   secondary_index indexes[16];

   std::map<std::string, secondary_index[16]> key_to_secondary_indexes;
   std::map<int32_t, secondary_index[16]> iterator_to_secondary_indexes;

   std::map<std::string, int32_t> key_to_iterator_secondary;
   std::map<int32_t, std::string> iterator_to_key_secondary;

   static const int SHIFT_FOR_KEY = 24;
   static const int SHIFT_FOR_INDEX = 20;
   static const int ITERATOR_MASK = 0x000FFFFF;
   static const int TABLE_MASK = 0x0000000F;
   static const int INDEX_MASK = 0x00F;

   int32_t iterator_to_table_key(int32_t itr) {
      return itr >> SHIFT_FOR_KEY;
   }
   int32_t table_key_to_iterator(int32_t table_key) {
      return table_key << SHIFT_FOR_KEY;
   }
   int32_t iterator_to_index(int32_t itr) {
      return (itr >> SHIFT_FOR_INDEX) & INDEX_MASK;
   }
   int32_t index_to_iterator(int32_t index) {
      return index << SHIFT_FOR_INDEX;
   }
   int32_t get_iterator(int32_t itr) {
      return itr & ITERATOR_MASK;
   }
   uint64_t table_name_to_index(uint64_t table_name) {
      return table_name & TABLE_MASK;
   }
   std::string normalize_table_name(capi_name table) {
      return eosio::name{ table & 0xFFFFFFFFFFFFFFF0 }.to_string();
   }

   std::tuple<int32_t, int32_t, int32_t> unpack_iterator(int32_t iterator) {
      int32_t table_key = iterator_to_table_key(iterator);
      int32_t index = iterator_to_index(iterator);
      int32_t itr = get_iterator(iterator);

      return std::make_tuple(table_key, index, itr);
   }

   template <typename T, secondary_index_type Idx>
   int32_t store(uint64_t scope, uint64_t table, uint64_t payer, const uint64_t id, const T* secondary) {
      std::string key = TESTING_CODE.to_string() + eosio::name{ scope }.to_string() + normalize_table_name(table);
      uint64_t index = table_name_to_index(table);

      int32_t table_key;
      auto t = key_to_secondary_indexes.find(key);
      if (t == key_to_secondary_indexes.end()) {
         table_key = iterator_to_secondary_indexes.size()+1;
         auto s = secondary_index{Idx, std::vector<secondary_index_row>(), key};
         key_to_secondary_indexes[key][index] = s;
         iterator_to_secondary_indexes[table_key][index] = s;

         key_to_iterator_secondary[key] = table_key;
         iterator_to_key_secondary[table_key] = key;
      } else {
         table_key = key_to_iterator_secondary[key];
      }

      auto& idxs = key_to_secondary_indexes.at(key);
      auto& idx = idxs[index];

      idx.rows.push_back(secondary_index_row{Idx, *secondary, id});

      iterator_to_secondary_indexes[table_key][index].rows.push_back(secondary_index_row{Idx, *secondary, id});

      return table_key_to_iterator(table_key) + index_to_iterator(index) + idx.rows.size() - 1;
   }


   template <typename T, secondary_index_type Idx>
   void update(int32_t iterator, uint64_t payer, const T* secondary) {
      auto [table_key, index, itr] = unpack_iterator(iterator);

      auto& tbl = iterator_to_secondary_indexes[table_key];
      auto& idx = tbl[index];
      // TODO
      // idx.rows[itr].val.Idx = *secondary;
      selectMember<T>(idx.rows[itr].val) = *secondary;

      auto key = iterator_to_key_secondary[table_key];
      key_to_secondary_indexes[key][index] = idx;
      return;
   }

   template <typename T, secondary_index_type Idx>
   int32_t find_primary(uint64_t code, uint64_t scope, uint64_t table, T* secondary, uint64_t primary) {
      std::string key = TESTING_CODE.to_string() + eosio::name{ scope }.to_string() + normalize_table_name(table);
      uint64_t index = table_name_to_index(table);

      auto t = key_to_secondary_indexes.find(key);
      if (t == key_to_secondary_indexes.end()) {
         return -1;
      }

      auto& idxs = key_to_secondary_indexes.at(key);
      auto& idx = idxs[index];

      for (int i = 0; i < idx.rows.size(); ++i) {
         auto& row = idx.rows[i];
         if (row.primary_key == primary) {
            // TODO:
            // *secondary = row.val.Idx;
            *secondary = selectMember<T>(row.val);
            int32_t table_key = table_key_to_iterator(key_to_iterator_secondary[key]);
            return table_key + index_to_iterator(index) + i;
         }
      }

      return -1;
   }

   template <typename T, secondary_index_type Idx>
   int32_t find_secondary(capi_name code, uint64_t scope, capi_name table, const T* secondary, uint64_t* primary) {
      std::string key = TESTING_CODE.to_string() + eosio::name{ scope }.to_string() + normalize_table_name(table);
      uint64_t index = table_name_to_index(table);

      auto t = key_to_secondary_indexes.find(key);
      if (t == key_to_secondary_indexes.end()) {
         return -1;
      }

      auto& idxs = key_to_secondary_indexes.at(key);
      auto& idx = idxs[index];

      for (int i = 0; i < idx.rows.size(); ++i) {
         auto& row = idx.rows[i];
         if (selectMember<T>(row.val)== *secondary) {
            *primary = row.primary_key;
            // TODO: Not confident this works
            int32_t table_key = table_key_to_iterator(key_to_iterator_secondary[key]);
            return table_key + index_to_iterator(index) + i;
         }
      }

      return -1;
   }

   template <typename T, secondary_index_type Idx>
   int32_t lowerbound(capi_name code, uint64_t scope, capi_name table, T* secondary, uint64_t* primary) {
      std::string key = TESTING_CODE.to_string() + eosio::name{ scope }.to_string() + normalize_table_name(table);

      auto t = key_to_secondary_indexes.find(key);
      if (t == key_to_secondary_indexes.end()) {
         return -1;
      }

      uint64_t index = table_name_to_index(table);
      auto& idx = key_to_secondary_indexes[key][index];

      std::vector<secondary_index_row> to_sort;

      std::copy(idx.rows.begin(), idx.rows.end(), std::back_inserter(to_sort));

      auto cmp = [](secondary_index_row a, secondary_index_row b) { return a.val.idx64 < b.val.idx64; };
      std::sort(to_sort.begin(), to_sort.end(), cmp);

      auto row_itr = to_sort.begin();

      secondary_index_row match;
      while (row_itr != to_sort.end()) {
         if (selectMember<T>(row_itr->val) >= *secondary) {
            *primary = row_itr->primary_key;
            match = *row_itr;
            break;
         }

         ++row_itr;
      }

      auto& idxs = key_to_secondary_indexes.at(key);
      idx = idxs[index];
      for (int i = 0; i < idx.rows.size(); ++i) {
         auto& row = idx.rows[i];
         if (row == match) {
            int32_t table_key = table_key_to_iterator(key_to_iterator_secondary[key]);
            return table_key + index_to_iterator(index) + i;
         }
      }

      return -1;
   }

   template <typename T, secondary_index_type Idx>
   int32_t upperbound(capi_name code, uint64_t scope, capi_name table, T* secondary, uint64_t* primary) {
      std::string key = TESTING_CODE.to_string() + eosio::name{ scope }.to_string() + normalize_table_name(table);

      auto t = key_to_secondary_indexes.find(key);
      if (t == key_to_secondary_indexes.end()) {
         return -1;
      }

      uint64_t index = table_name_to_index(table);
      auto& idx = key_to_secondary_indexes[key][index];

      std::vector<secondary_index_row> to_sort;

      std::copy(idx.rows.begin(), idx.rows.end(), std::back_inserter(to_sort));
      std::sort(to_sort.begin(), to_sort.end());

      auto row_itr = to_sort.begin();

      secondary_index_row match;
      while (row_itr != to_sort.end()) {
         if (selectMember<T>(row_itr->val) <= *secondary) {
            *primary = row_itr->primary_key;
            match = *row_itr;
            break;
         }

         ++row_itr;
      }

      auto& idxs = key_to_secondary_indexes.at(key);
      idx = idxs[index];
      for (int i = 0; i < idx.rows.size(); ++i) {
         auto& row = idx.rows[i];
         if (row == match) {
            // TODO: Not confident this works
            int32_t table_key = table_key_to_iterator(key_to_iterator_secondary[key]);
            return table_key + index_to_iterator(index) + i;
         }
      }

      return -1;
   }

   void remove(int32_t iterator) {
      auto [table_key, index, itr] = unpack_iterator(iterator);

      auto& tbl = iterator_to_secondary_indexes[table_key];
      auto& idx = tbl[index];
      idx.rows[itr] = SNULLROW; 

      auto key = iterator_to_key_secondary[table_key];
      key_to_secondary_indexes[key][index] = idx;
      return;
   }

   int32_t end(capi_name code, uint64_t scope, capi_name table) {
      std::string key = TESTING_CODE.to_string() + eosio::name{ scope }.to_string() + normalize_table_name(table);

      auto t = key_to_secondary_indexes.find(key);
      if (t == key_to_secondary_indexes.end()) {
         return -1;
      }

      uint64_t index = table_name_to_index(table);

      return key_to_secondary_indexes[key][index].rows.size();
   }

   int32_t next(int32_t iterator, uint64_t* primary) {
      int32_t table_key = iterator_to_table_key(iterator);
      int32_t index = iterator_to_index(iterator);
      int32_t itr = get_iterator(iterator);

      int32_t new_it = itr+1;

      auto& idx = iterator_to_secondary_indexes[table_key][index];
      if (new_it == idx.rows.size()) return -1;

      *primary = idx.rows[new_it].primary_key;
      return iterator+1;
   }

   int32_t previous(int32_t iterator, uint64_t* primary) {
      int32_t table_key = iterator_to_table_key(iterator);
      int32_t index = iterator_to_index(iterator);
      int32_t itr = get_iterator(iterator);

      int32_t new_it = itr-1;
      if (new_it < 0) return -1;

      auto& idx = iterator_to_secondary_indexes[table_key][index];

      *primary = idx.rows[new_it].primary_key;
      return iterator-1;
   }


};

extern secondary_index_store* secondary_indexes;