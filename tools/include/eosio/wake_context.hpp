#pragma once

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <string_view>

namespace eosio { namespace cdt {
   struct target {
      target() = default;
      target(const std::string& name) : name(name) {}

      void add_include_path(const std::string& ip) { include_paths.insert(ip); }

      template <typename... Strings>
      void add_include_paths(const std::string& ip, Strings...strings) {
         add_include_path(ip);
         if constexpr (sizeof...(Strings) > 0)
            add_include_paths(strings...);
      }

      void add_ricardian_path(const std::string& rp) { ricardian_paths.insert(rp); }

      template <typename... Strings>
      void add_ricardian_paths(const std::string& rp, Strings...strings) {
         add_ricardian_path(rp);
         if constexpr (sizeof...(Strings) > 0)
            add_ricaridan_paths(strings...);
      }

      bool operator==(const target& other) { return name == other.name; }
      bool operator<(const target& other)  { return name < other.name; }
      struct hasher {
         size_t operator()(const target& t) {
            return std::hash<std::string>()(t.name);
         }
      };
      std::string name = "";
      std::unordered_set<std::string> include_paths   = {};
      std::unordered_set<std::string> ricardian_paths = {};
      std::unordered_set<std::string> link_paths      = {};
      std::unordered_set<std::string> libraries       = {};
      std::unordered_set<std::string> options         = {};
   };

   struct project {
      project() = default;
      project(const std::string& name) : name(name) {}
      bool operator==(const project& other) { return name == other.name; }
      bool operator<(const project& other)  { return name == other.name; }
      struct hasher {
         size_t operator()(const target& t) {
            return std::hash<std::string>()(t.name);
         }
      };
      std::string name = "";
      std::unordered_map<std::string, target, target::hasher> targets = {};
   };

   struct wake_context {
      void add_project(const std::string& pname) {
         projects.emplace(pname, pname);
      }

      std::unordered_map<std::string, project> projects = {};
   };
}} // ns eosio::wake
