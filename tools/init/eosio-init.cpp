// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/Builtins.h"
#include "llvm/Support/FileSystem.h"
#include "eosio/utils.hpp"
#include "eosio/gen.hpp"
#include "eosio/whereami/whereami.hpp"
#include "eosio/abi.hpp"

#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <set>
#include <map>
#include <chrono>
#include <ctime>

#include "llvm/Support/CommandLine.h"
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

struct project {
   std::string project_name;
   std::string path;
   bool        bare;

   project( const std::string& pn, const std::string& p, bool b ) : project_name(pn), path(p), bare(b) {
      llvm::sys::fs::create_directory(path);
      if (!bare) {
         llvm::sys::fs::create_directory(path+"/src");
         llvm::sys::fs::create_directory(path+"/include");
         llvm::sys::fs::create_directory(path+"/ricardian");
         llvm::sys::fs::create_directory(path+"/build");
      }
   }

   std::string cdt_root = "/usr/local";
   const std::string cpp = "#include <@.hpp>\n"
                           "ACTION @::hi( name nm ) {\n"
                           "   /* fill in action body */\n"
                           "   print_f(\"Name : %\\n\",nm);\n"
                           "}";

   const std::string hpp = "#include <eosio/eosio.hpp>\n"
                           "using namespace eosio;\n\n"
                           "CONTRACT @ : public contract {\n"
                           "   public:\n"
                           "      using contract::contract;\n\n"
                           "      ACTION hi( name nm );\n\n"
                           "      using hi_action = action_wrapper<\"hi\"_n, &@::hi>;\n"
                           "};";
   
   const std::string ricardian = "<h1 class=\"contract\"> hi </h1>\n\n"
                                 "Stub for hi action's ricardian contract";

   const std::string cmake = "project(@)\n\n"
                             "set(EOSIO_WASM_OLD_BEHAVIOR \"Off\")\n"
                             "find_package(eosio.cdt)\n\n"
                             "add_contract( @ @ @.cpp )\n"
                             "target_include_directories( @ PUBLIC ${CMAKE_SOURCE_DIR}/../include )\n"
                             "target_ricardian_directory( @ ${CMAKE_SOURCE_DIR}/../ricardian )";

   const std::string cmake_extern = "include(ExternalProject)\n"
                                    "# if no cdt root is given use default path\n"
                                    "if(EOSIO_CDT_ROOT STREQUAL \"\" OR NOT EOSIO_CDT_ROOT)\n"
                                    "   find_package(eosio.cdt)\n"
                                    "endif()\n\n"
                                    "ExternalProject_Add(\n"
                                    "   @_project\n"
                                    "   SOURCE_DIR ${CMAKE_SOURCE_DIR}/src\n"
                                    "   BINARY_DIR ${CMAKE_BINARY_DIR}/@\n"
                                    "   CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${EOSIO_CDT_ROOT}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake\n"
                                    "   UPDATE_COMMAND \"\"\n"
                                    "   PATCH_COMMAND \"\"\n"
                                    "   TEST_COMMAND \"\"\n"
                                    "   INSTALL_COMMAND \"\"\n"
                                    "   BUILD_ALWAYS 1\n"
                                    ")";

   const std::string readme_cmake = "--- @ Project ---\n\n"
                                    " - How to Build -\n"
                                    "   - cd to 'build' directory\n"
                                    "   - run the command 'cmake ..'\n"
                                    "   - run the command 'make'\n\n"
                                    " - After build -\n" 
                                    "   - The built smart contract is under the '@' directory in the 'build' directory\n"
                                    "   - You can then do a 'set contract' action with 'cleos' and point in to the './build/@' directory\n\n"
                                    " - Additions to CMake should be done to the CMakeLists.txt in the './src' directory and not in the top level CMakeLists.txt";

   const std::string readme_bare = " --- @ Project ---\n\n"
                                   " - How to Build -\n"
                                   "   - run the command 'eosio-cpp -abigen -o @.wasm @.cpp'\n";

   std::string replace_name( const std::string& in ) {
      std::stringstream ss;
      for ( char c : in ) {
         if (c == '@')
            ss << project_name;
         else
            ss << c;
      }
      return ss.str();
   }
   
   std::string replace_cdt_root( const std::string& in ) {
      std::stringstream ss;
      for ( char c : in ) {
         if (c == '%')
            ss << cdt_root;
         else
            ss << c;
      }
      return ss.str();
   }
   std::string replace_include_type( const std::string& in ) {
      std::stringstream ss;
      for ( char c : in ) {
         if ((c == '<' || c == '>') && bare)
            ss << '"';
         else
            ss << c;
      }
      return ss.str();
   }

   void write_cpp() {
      std::ofstream cpp_out;
      if (bare)
         cpp_out.open(path+"/"+project_name+".cpp");
      else
         cpp_out.open(path+"/src/"+project_name+".cpp");
      cpp_out << replace_include_type(replace_name(cpp));
   }
   void write_hpp() {
      std::ofstream hpp_out;
      if (bare)
         hpp_out.open(path+"/"+project_name+".hpp");
      else
         hpp_out.open(path+"/include/"+project_name+".hpp");
      hpp_out << replace_name(hpp);
   }
   void write_cmake() {
      if (!bare) {
         std::ofstream cmake_out(path+"/src/CMakeLists.txt");
         cmake_out << replace_name(replace_cdt_root(cmake));
      }
   }
   void write_cmake_extern() {
      if (!bare) {
         std::ofstream cmake_out(path+"/CMakeLists.txt");
         cmake_out << replace_name(replace_cdt_root(cmake_extern));
      }
   }
   void write_ricardian() {
      std::ofstream rc_out;
      if (bare)
         rc_out.open(path+"/"+project_name+".contracts.md");
      else
         rc_out.open(path+"/ricardian/"+project_name+".contracts.md");
      rc_out << ricardian;
   }
   void write_readme() {
      std::ofstream readme_out(path+"/README.txt");
      if (bare) {
         readme_out << replace_name(readme_bare);
      } else {
         readme_out << replace_name(readme_cmake);
      }
   }

};

int main(int argc, const char **argv) {

   cl::SetVersionPrinter([](llvm::raw_ostream& os) {
        os << "eosio-init version " << "@VERSION_FULL@" << "\n";
  });
   cl::OptionCategory cat("eosio-init", "generates an eosio smart contract project");
   
   cl::opt<bool> bare_opt(
      "bare",
      cl::desc("produces only a skeleton smart contract without CMake support"), 
      cl::cat(cat));
   cl::opt<std::string> project_name(
      "project",
      cl::desc("output project name"), 
      cl::Required,
      cl::cat(cat));
   cl::opt<std::string> output_dir(
      "path",
      cl::desc("directory to place the project"),
      cl::cat(cat));

   cl::ParseCommandLineOptions(argc, argv, std::string("eosio-proj"));
   try {
      if (!std::regex_match(project_name, std::regex("^[_a-zA-Z][_a-zA-Z0-9]*$"))) {
         throw std::runtime_error("ERROR: invalid identifier: " + project_name + " (ensure that it is a valid C++ identifier)");
      }
      llvm::SmallString<128> rp;
      std::string path = output_dir;
      if (path.empty())
         path = ".";
      llvm::sys::fs::real_path(path, rp, false);
      path = rp.str();
      if (path.empty()) {
         llvm::outs() << output_dir+" path not found\n";
         return -1;
      }
      path += "/"+project_name;

      project proj(project_name, path, bare_opt);
      proj.write_hpp();
      proj.write_cpp();
      proj.write_cmake();
      proj.write_cmake_extern();
      proj.write_ricardian();
      proj.write_readme();
   } catch ( std::exception& e ) {
      std::cout << e.what() << "\n";
      return -1;
   }
   
   return 0;
}
