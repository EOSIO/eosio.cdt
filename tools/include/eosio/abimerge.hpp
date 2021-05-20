#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wcovered-switch-default"
#include <jsoncons/json.hpp>
#include "abi.hpp"

#include <string>
#include <vector>

using jsoncons::json;
using jsoncons::ojson;

class ABIMerger {
   public:
      ABIMerger(ojson a) : abi(a) {}
      void set_abi(ojson a) {
         abi = a;
      }
      std::string get_abi_string()const {
         std::stringstream ss;
         ss << pretty_print(abi);
         return ss.str();
      }
      ojson merge(ojson other) {
         ojson ret;
         ret["____comment"] = abi["____comment"];
         ret["version"]  = merge_version(other);
         ret["types"]    = merge_types(other);
         ret["structs"]  = merge_structs(other);
         ret["actions"]  = merge_actions(other);
         ret["tables"]   = merge_tables(other);
         ret["kv_tables"] = merge_kv_tables(other);
         ret["ricardian_clauses"]  = merge_clauses(other);
         ret["variants"] = merge_variants(other);
         std::string vers = abi["version"].as<std::string>();
         if (std::stod(vers.substr(vers.size()-3))*10 >= 12) {
            ret["action_results"] = merge_action_results(other);
         }
         return ret;
      }
   private:
      std::string merge_version(ojson b) {
         std::string ver_a = abi["version"].as<std::string>();
         std::string ver_b = b["version"].as<std::string>();
         return std::stod(ver_a.substr(ver_a.size()-3))*10 < std::stod(ver_b.substr(ver_b.size()-3))*10 ?
            ver_b : ver_a;
      }

      static bool struct_is_same(ojson a, ojson b) {
         bool same_fields = a["fields"].size() == b["fields"].size();
         for (auto a_field : a["fields"].array_range()) {
            bool found_field = false;
            for (auto b_field : b["fields"].array_range()) {
               if (a_field["name"] == b_field["name"] &&
                   a_field["type"] == b_field["type"])
                  found_field = true;
            }
            if (!found_field)
               return false;
         }
         return a["name"] == b["name"] &&
                a["base"] == b["base"] && same_fields;
      }

      static bool type_is_same(ojson a, ojson b) {
         return a["new_type_name"] == b["new_type_name"] &&
                a["type"] == b["type"];
      }

      static bool action_is_same(ojson a, ojson b) {
         return a["name"] == b["name"] &&
                a["type"] == b["type"] &&
                a["ricardian_contract"] == b["ricardian_contract"];
      }

      template <typename T>
      static bool action_is_almost_same(ojson a, ojson b, T& rc) {
         if (a["ricardian_contract"].empty())
            rc = b["ricardian_contract"];
         return a["name"] == b["name"] &&
                a["type"] == b["type"];
      }


      static bool variant_is_same(ojson a, ojson b) {
         for (auto tya : a["types"].array_range()) {
            bool found_ty = false;
            for (auto tyb : b["types"].array_range()) {
               if (tyb == tya)
                  found_ty = true;
            }
            if (!found_ty)
               return false;
         }
         return a["name"] == b["name"];
      }

      static bool table_is_same(ojson a, ojson b) {
         return a["name"] == b["name"] &&
                a["type"] == b["type"] &&
                a["index_type"] == b["index_type"] &&
                a["key_names"] == b["key_names"] &&
                a["key_types"] == b["key_types"];
      }

      static bool clause_is_same(ojson a, ojson b) {
         return a["id"] == b["id"] &&
                a["body"] == b["body"];
      }

      static bool action_result_is_same(ojson a, ojson b) {
         return a["name"] == b["name"] &&
                a["result_type"] == b["result_type"];
      }

      template <typename F>
      void add_object_to_array(ojson& ret, ojson a, ojson b, std::string type, std::string id, F&& is_same_func) {
         for (auto obj_a : a[type].array_range()) {
            ret.push_back(obj_a);
         }
         for (auto obj_b : b[type].array_range()) {
            bool should_skip = false;
            for (auto obj_a : a[type].array_range()) {
               if (obj_a[id] == obj_b[id]) {
                  if (!is_same_func(obj_a, obj_b)) {
                     throw std::runtime_error(std::string("Error, ABI structs malformed : ")+obj_a[id].as<std::string>()+" already defined");
                  }
                  else
                     should_skip = true;
               }
            }
            if (!should_skip)
               ret.push_back(obj_b);
         }
      }

      void add_object_to_object(ojson& ret, ojson a, ojson b, std::string type, std::string id) {
         for (auto obj_a : a[type].object_range()) {
            ret.insert_or_assign(obj_a.key(), obj_a.value());
         }
         for (auto obj_b : b[type].object_range()) {
            bool should_skip = false;
            for (auto obj_a : a[type].object_range()) {
               if (obj_a.key() == obj_b.key()) {
                  if (obj_a.value() != obj_b.value()) {
                     throw std::runtime_error(std::string("Error, ABI structs malformed : ")+std::string(obj_a.key().data(), obj_a.key().size())+" already defined");
                  }
                  else {
                     should_skip = true;
                  }
               }
            }
            if (!should_skip) {
               ret.insert_or_assign(obj_b.key(), obj_b.value());
            }
         }
      }

      ojson merge_structs(ojson b) {
         ojson structs = ojson::array();
         add_object_to_array(structs, abi, b, "structs", "name", struct_is_same);
         return structs;
      }

      ojson merge_types(ojson b) {
         ojson types = ojson::array();
         add_object_to_array(types, abi, b, "types", "new_type_name", type_is_same);
         return types;
      }

      ojson merge_variants(ojson b) {
         ojson vars = ojson::array();
         add_object_to_array(vars, abi, b, "variants", "name", variant_is_same);
         return vars;
      }

      ojson merge_actions(ojson b) {
         ojson acts = ojson::array();
         add_object_to_array(acts, abi, b, "actions", "name", action_is_same);
         return acts;
      }

      ojson merge_tables(ojson b) {
         ojson tabs = ojson::array();
         add_object_to_array(tabs, abi, b, "tables", "name", table_is_same);
         return tabs;
      }

      ojson merge_kv_tables(ojson b) {
         ojson kv_tabs = ojson::object();
         add_object_to_object(kv_tabs, abi, b, "kv_tables", "name");
         return kv_tabs;
      }

      ojson merge_clauses(ojson b) {
         ojson cls = ojson::array();
         add_object_to_array(cls, abi, b, "ricardian_clauses", "id", clause_is_same);
         return cls;
      }

      ojson merge_action_results(ojson b) {
         ojson res = ojson::array();
         add_object_to_array(res, abi, b, "action_results", "name", action_result_is_same);
         return res;
      }

      ojson abi;
};
#pragma GCC diagnostic pop
