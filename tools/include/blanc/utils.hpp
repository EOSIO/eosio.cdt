#pragma once
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <optional>
#include <utility>
#include <eosio/whereami/whereami.hpp>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>
namespace blanc {

   template<typename T>
   void print(T&& container) {
      std::for_each(container.begin(), container.end(), [](const auto& e){ llvm::outs() << e << " "; });
      llvm::outs() << "\n";
   }

   template<>
   void print(std::string& s) {
      llvm::outs() << "\"" << s << "\" ";
   }

   void print(const char* s) {
      llvm::outs() << s ;
   }

   template<typename T, typename... Ts>
   void print(T&& a, Ts&&... b) {
      print(std::forward<T>(a));
      print(std::forward<Ts>(b)...);
   }

   void print(const char** begin, size_t length) {
      std::for_each(begin, begin+length, [](auto e){ llvm::outs() << e << " "; });
      llvm::outs() << "\n";
   }

   int exec_subprogram(const std::string& prog, std::vector<std::string> options, bool show_commands) {
      int ret = 0;
      std::string find_path = eosio::cdt::whereami::where();
      std::stringstream args;
      for (const auto& s : options) {
         args << s << " ";
      }
      
      auto path = llvm::sys::findProgramByName(prog.c_str(), {find_path});
      if (!path) path = llvm::sys::findProgramByName(prog.c_str(), {"/usr/bin"});

      if (path) {
         auto cmd = *path+" "+args.str();
         if (show_commands) llvm::outs() << "\n" << cmd << "\n";
         return std::system(cmd.c_str());
      } else {
         return ENOENT;
      }
   }

   std::optional<std::string> get_option_value(const std::vector<std::string>& options, std::string name, std::string prefix = "-{1,2}", std::string suffix = "(=.+)?$") {
      std::regex re{prefix+name+suffix};
      bool ret_next = false;
      for (const auto& opt : options) {
         if (ret_next) {
            return opt;
         }
         if (regex_match(opt, re)) {
            auto pos = opt.find('=');
            if (pos != std::string::npos) {
               return opt.substr(pos+1);
            }
            ret_next = true;
         }
      }
      return std::nullopt;
   }

   bool has_option(const std::vector<std::string>& options, std::string name, std::string prefix = "-{1,2}", std::string suffix = "(=.+)?$") {
      std::regex re{prefix+name+suffix};
      for (const auto& opt : options) {
         if (regex_match(opt, re)) {
            return true;
         }
      }
      return false;
   }

   std::optional<std::string> extract_option(std::vector<std::string>& options, bool has_value, std::string name, std::string prefix = "-{1,2}", std::string suffix = "(=.+)?$") {
      std::regex re{prefix+name+suffix};
      std::string ret;
      std::vector<std::string> new_opts;
      for (auto it = options.begin(); it != options.end(); ++it) {
         if (regex_match(*it, re)) {
            new_opts.insert(new_opts.begin(), options.begin(), it);
            if (has_value) {
               auto pos = it->find('=');
               if (pos != std::string::npos) {
                  ret = it->substr(pos+1);
               } else if ((++it) != options.end()) {
                  ret = *it;
               }
            }
            if (it != options.end()) {
               new_opts.insert(new_opts.end(), it+1, options.end());
            }
            options = new_opts;
            return ret;
         }
      }
      return std::nullopt;
   }

   template<typename T>
   llvm::SmallString<PATH_MAX> to_absolute_path(T&& path) {
      llvm::SmallString<PATH_MAX> absolute_path = llvm::StringRef(path);
      llvm::sys::fs::make_absolute(absolute_path);
      return absolute_path;
   }

   template<typename T>
   std::string get_temporary_path(T&& path) {
      static llvm::SmallString<PATH_MAX> system_temp_dir;
      if (system_temp_dir.empty()) {
         llvm::sys::path::system_temp_directory(true, system_temp_dir);
      }
      auto tmp_dir = system_temp_dir;
      llvm::sys::path::append(tmp_dir, path);
      return tmp_dir.str().str();
   }

   struct scope_exit {
      std::function<void()> fun;
      template <typename Fn>
      scope_exit(Fn&& fn) : fun(fn) {}
      ~scope_exit() { fun(); }
   };
}
