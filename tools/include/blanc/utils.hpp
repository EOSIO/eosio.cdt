#pragma once
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
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

   std::string llvm_path() {
      const char *llvm_root = std::getenv("LLVM_ROOT");
      if (llvm_root)
         return llvm_root;

      std::string conf_file =
            eosio::cdt::whereami::where() + "/../etc/eosio.cdt.conf";
      if (llvm::sys::fs::exists(conf_file)) {
         std::ifstream t(conf_file);
         std::string s((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
         const std::regex base_regex("\\s*LLVM_ROOT\\s*=\\s*(.+)\\s*$");
         std::smatch base_match;
         if (std::regex_match(s, base_match, base_regex))
            return base_match[1];
      }

      return "";
   }

   llvm::ArrayRef<llvm::StringRef> get_llvm_bin_paths() {
      static std::string p = llvm_path();
      static std::string llvm_bin_path = p + "/bin";
      static llvm::StringRef r[] = { llvm_bin_path };
      return p.size() ? llvm::ArrayRef<llvm::StringRef>{r} : llvm::ArrayRef<llvm::StringRef>{};
   }

   template <typename Container>
   int exec_subprogram(const std::string& prog, const Container& options, bool show_commands) {
      
      auto run_prog = [&options, show_commands](const std::string& abs_path) {
         std::stringstream cmd;
         cmd << abs_path ;
         for (const auto& s : options) {
            cmd << " " << s;
         }
         auto cmd_str = cmd.str();
         if (show_commands) llvm::outs() << "\n" << cmd_str << "\n";
         return std::system(cmd_str.c_str());
      };

      if (prog.size() && prog[0] == '/') return run_prog(prog);

      static auto search_paths = get_llvm_bin_paths();
      auto path = llvm::sys::findProgramByName(prog.c_str(), search_paths );
      if (search_paths.size() && !path) {
         // use the system PATH environment to find the program instead
         path = llvm::sys::findProgramByName(prog.c_str(), {} );
      }

      if (path) return run_prog(*path);

      std::cerr << prog << " not found, please set the environment variables LLVM_ROOT or PATH to locate the program\n";
      return ENOENT;
      
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
