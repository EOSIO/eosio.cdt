#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

struct abi_typedef {
   std::string new_type_name;
   std::string type;
   bool operator<(const abi_typedef& t) const { return new_type_name < t.new_type_name; }
};

struct abi_field {
   std::string name;
   std::string type;
};

struct abi_struct {
   std::string name;
   std::string base;
   std::vector<abi_field> fields;
   bool operator<(const abi_struct& s) const { return name < s.name; }
};

struct abi_action {
   std::string name;
   std::string type;
   std::string ricardian_contract;
   bool operator<(const abi_action& s) const { return name < s.name; }
};

struct abi_table {
   std::string name;
   std::string type;
   std::string index_type;
   std::vector<std::string> key_names;
   std::vector<std::string> key_types;
   bool operator<(const abi_table& t) const { return name < t.name; }
};

struct abi_ricardian_clause_pair {
   std::string id;
   std::string body;
};

struct abi_variant {
   std::string name;
   std::vector<std::string> types;
   bool operator<(const abi_variant& t) const { return name < t.name; }
};

struct abi_error_message {
   uint64_t    error_code;
   std::string error_msg;
};

struct wasm_action {
   std::string name;
   std::string handler;
};

struct wasm_notify {
   std::string name;
   std::string contract;
   std::string handler;
};

namespace std {
   template<>
   struct less<wasm_action> {
      bool operator()(const wasm_action& lhs, const wasm_action& rhs) const {
         return lhs.name < rhs.name;
      }
   };

   template<>
   struct less<wasm_notify> {
      bool operator()(const wasm_notify& lhs, const wasm_notify& rhs) const {
         if (lhs.name == rhs.name) {
            if (lhs.contract == "*" && rhs.contract != "*") {
               return false;
            } else if (lhs.contract != "*" && rhs.contract == "*") {
               return true;
            }
         }
         return std::tie(lhs.name, lhs.contract) < std::tie(rhs.name, rhs.contract);
      }
   };
}

/// From eosio libraries/chain/include/eosio/chain/abi_def.hpp
struct abi {
   std::string version = "eosio::abi/1.1";
   std::set<abi_struct>  structs;
   std::set<abi_typedef> typedefs;
   std::set<abi_action>  actions;
   std::set<abi_table>   tables;
   std::set<abi_variant> variants;
   std::vector<abi_ricardian_clause_pair>   ricardian_clauses;
   std::vector<abi_error_message> error_messages;
   std::set<wasm_action> wasm_actions;
   std::set<wasm_notify> wasm_notifies;
   std::set<std::string> wasm_entries;
};

inline void dump( const abi& abi ) {
   std::cout << "ABI : "; 
   std::cout << "\n\tversion : " << abi.version;
   std::cout << "\n\tstructs : ";
   for (auto s : abi.structs) {
      std::cout << "\n\t\tstruct : ";
      std::cout << "\n\t\t\tname : " << s.name;
      std::cout << "\n\t\t\tbase : " << s.base;
      std::cout << "\n\t\t\tfields : ";
      for (auto f : s.fields) {
         std::cout << "\n\t\t\t\tfield : ";
         std::cout << "\n\t\t\t\t\tname : " << f.name;
         std::cout << "\n\t\t\t\t\ttype : " << f.type << '\n';
      }
   }
}
