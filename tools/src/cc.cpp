#include <blanc/custom_attrs.hpp>
#include <blanc/profile.hpp>
#include <blanc/tokenize.hpp>
#include <blanc/utils.hpp>
#include <eosio/whereami/whereami.hpp>
#include <llvm/Option/ArgList.h>
#include <llvm/Option/OptTable.h>
#include <llvm/Support/FileSystem.h>

#include "cc.hpp"

using namespace clang;
using namespace clang::driver;
using namespace llvm;
using namespace llvm::opt;
using namespace blanc;

#define PREFIX(NAME, VALUE) static const char* const NAME[] = VALUE;
#include <clang/Driver/Options.inc>
#undef PREFIX

static const OptTable::Info InfoTable[] = {
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, ALIASARGS, FLAGS, PARAM,  \
               HELPTEXT, METAVAR, VALUES)                                      \
  {PREFIX, NAME,  HELPTEXT,    METAVAR,     OPT_##ID,  Option::KIND##Class,    \
   PARAM,  FLAGS, OPT_##GROUP, OPT_##ALIAS, ALIASARGS, VALUES},
#include "cc_options.inc"
#undef OPTION
};

class BlancCcOptTable : public OptTable {
public:
   BlancCcOptTable(): OptTable(InfoTable) {}
};

std::vector<std::string> inputs;
std::vector<std::string> new_inputs;
std::string output;
std::vector<std::string> resource_dirs;
std::set<std::string> include_dirs;
bool link = true;
bool keep_generated = false;

std::string contract_name;
std::string abi_version;
bool no_abigen;
bool suppress_ricardian_warnings;

std::vector<std::string> override_compile_options(InputArgList& Args) {
   std::vector<std::string> new_opts;

   if (Args.hasArgNoClaim(OPT_keep_generated)) {
      keep_generated = true;
      Args.eraseArg(OPT_keep_generated);
   }

   if (Args.hasArgNoClaim(OPT_c, OPT_fsyntax_only, OPT_E)) {
      link = false;
   }

   if (!Args.hasArgNoClaim(OPT_nostdlib)) {
      new_opts.emplace_back("-nostdlib");
   }

   if (!link) {
      if (!Args.hasArgNoClaim(OPT_mthread_model)) {
         new_opts.emplace_back("-mthread-model");
         new_opts.emplace_back("single");
      }
   
      if (!Args.hasArgNoClaim(OPT__sysroot, OPT__sysroot_EQ)) {
         new_opts.emplace_back("--sysroot="+eosio::cdt::whereami::where()+"/..");
      }
      
      if (!Args.hasArgNoClaim(OPT_fno_rtti, OPT_frtti)) {
         new_opts.emplace_back("-fno-rtti");
      }

      if (!Args.hasArgNoClaim(OPT_O_Group)) {
         new_opts.emplace_back("-O3");
      }
      if (!Args.hasArgNoClaim(OPT_ffreestanding)) {
         new_opts.emplace_back("-ffreestanding");
      }
      if (!Args.hasArgNoClaim(OPT_nobuiltininc)) {
         new_opts.emplace_back("-nobuiltininc");
      }
      if (!Args.hasArgNoClaim(OPT_fno_builtin)) {
         new_opts.emplace_back("-fno-builtin");
      }
      if (!Args.hasArgNoClaim(OPT_fno_threadsafe_statics)) {
         new_opts.emplace_back("-fno-threadsafe-statics");
      }
      if (!Args.hasArgNoClaim(OPT_fno_exceptions)) {
         new_opts.emplace_back("-fno-exceptions");
      }
      if (!Args.hasArgNoClaim(OPT_fno_cxx_static_destructors)) {
         // wasm-ld won't genenate code to invoke the static destructors ( i.e. __cxx_global_array_dtor ) by default.
         // Use `-fno-c++-static-destructors` can remove the `__cxx_global_array_dtor` and the functions it invokes from the result wasm.
         new_opts.emplace_back("-fno-c++-static-destructors");
      }

      if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/../lib/libeosio_attrs"  SHARED_LIB_SUFFIX)) {
         new_opts.emplace_back("-fplugin="+eosio::cdt::whereami::where()+"/../lib/libeosio_attrs" SHARED_LIB_SUFFIX);
      }

#ifdef CPP_COMP
      if (!Args.hasArgNoClaim(OPT_std_EQ)) {
         new_opts.emplace_back("-std=c++20");
      }
#endif
   }

#ifdef CPP_COMP
   auto input_args = Args.filtered(OPT_INPUT);
   for (const auto& arg : input_args) {
      inputs.emplace_back(arg->getValue());
   }
   Args.eraseArg(OPT_INPUT);
#endif

   if (auto arg = Args.getLastArgNoClaim(OPT_o)) {
      output = arg->getValue();
   } else if (inputs.size() == 1 && !Args.getLastArgNoClaim(OPT_E)) {
      llvm::SmallString<PATH_MAX> fn = llvm::sys::path::filename(inputs[0]);
      llvm::sys::path::replace_extension(fn, Args.hasArgNoClaim(OPT_c) ? ".o" : ".wasm");
      output = fn.str().str();
   }

   if (auto arg = Args.getLastArgNoClaim(OPT_contract)) {
      contract_name = arg->getValue();
      Args.eraseArg(OPT_contract);
   } else if (output.size()) {
      llvm::SmallString<PATH_MAX> fn = llvm::sys::path::filename(output);
      contract_name = fn.str().str();
      auto pos = contract_name.find('.');
      if (pos != std::string::npos) 
         contract_name.resize(pos);
   }

   if (auto arg = Args.getLastArgNoClaim(OPT_abi_version)) {
      abi_version = arg->getValue();
      if (link) {
         new_opts.emplace_back("-Wl,--abi-version="+abi_version);
      }
   }
   if (Args.hasArgNoClaim(OPT_no_abigen)) {
      no_abigen = true;
      if (link) {
         new_opts.emplace_back("-Wl,--no-abigen");
      }
   }
   if (Args.hasArgNoClaim(OPT_no_missing_ricardian_clause)) {
      suppress_ricardian_warnings = true;
   }
   // XXX: workaround fix for ide
   auto defs = Args.filtered(OPT_D);
   for (const auto& arg : defs) {
      std::string def(arg->getValue());
      if (def.starts_with("EOSIO_CONTRACT=")) {
         contract_name = def.substr(def.find("=")+1);
      } else if (def.starts_with("EOSIO_ABI_VERSION=")) {
         abi_version = def.substr(def.find("=")+1);
         if (link) {
            new_opts.emplace_back("-Wl,--abi-version="+abi_version);
         }
      } else if (def.starts_with("EOSIO_NO_ABIGEN")) {
         no_abigen = true;
         if (link) {
            new_opts.emplace_back("-Wl,--no-abigen");
         }
      } else if (def.starts_with("EOSIO_NO_RICARDIAN")) {
         suppress_ricardian_warnings = true;
      }
   }

   auto resource_args = Args.filtered(OPT_R_Joined);
   for (const auto& arg : resource_args) {
      resource_dirs.emplace_back(arg->getValue());
   }

   Args.eraseArg(OPT_contract);
   Args.eraseArg(OPT_abi_version);
   Args.eraseArg(OPT_no_abigen);
   Args.eraseArg(OPT_no_missing_ricardian_clause);
   Args.eraseArg(OPT_R_Joined);

   for (const auto arg : Args) {
      const auto& opt = arg->getOption();
      if (opt.getID() == OPT_v || !opt.hasFlag(CC1Option) || !link)
         new_opts.emplace_back(arg->getAsString(Args));
   }
   if (!link) {
      new_opts.emplace_back("-I" + eosio::cdt::whereami::where() + "/../include");
      new_opts.emplace_back("-isystem " + eosio::cdt::whereami::where() + "/../include/c++/v1");
   }
   return new_opts;
}

bool is_object_type(const std::string& fn) {
   auto ext = llvm::sys::path::extension(fn);
   return ext == ".o" || ext == ".obj" || ext == ".a";
}

int main(int argc, const char** argv) {

#ifdef CPP_COMP
   const char* backend = std::getenv("EOSIO_CXX_COMPILER_BACKEND");
#else
   const char* backend = std::getenv("EOSIO_C_COMPILER_BACKEND");
#endif
   if (!backend) {
      backend = TOOL_BACKEND;
   }

   ArrayRef<const char*> Argv{argv+1, argv+argc};
   unsigned missingIndex;
   unsigned missingCount;

   BlancCcOptTable opts;
   auto Args = opts.ParseArgs(Argv, missingIndex, missingCount, 0, CLOption);

   auto target = Args.getLastArgNoClaim(OPT_target);
   if (target &&!std::string(target->getValue()).starts_with("wasm32")) {
      return blanc::exec_subprogram(backend, Argv, false);
   }

   bool has_v = Args.hasArgNoClaim(OPT_v);
   auto args = override_compile_options(Args);
   if (!target) {
      args.push_back("-target");
      args.push_back("wasm32");
   }

#ifdef CPP_COMP

   std::vector<std::string> tmp_inputs;
   blanc::scope_exit on_exit([&tmp_inputs, keep_generated = keep_generated](){
      if (!keep_generated) {
         for (const auto& tmp_file : tmp_inputs) {
            llvm::sys::fs::remove(tmp_file);
         }
      }
   });

   bool link_only = true;

   for (auto input : inputs) {
      if (is_object_type(input)) {
         new_inputs.emplace_back(input);
         continue;
      } else {
         link_only = false;
      }

      std::vector<std::string> local_args = args;
      std::string _output;

      auto input_absolute = to_absolute_path(input);
      bool is_preprocess = Args.hasArgNoClaim(OPT_E);

      if (Args.hasArgNoClaim(OPT_c)) {
         if (output.size()) {
            _output = output;
         } else if (!is_preprocess){
            llvm::SmallString<PATH_MAX> fn = llvm::sys::path::filename(input);
            llvm::sys::path::replace_extension(fn, ".o");
            _output = fn.str().str();
         }
      } else if (!is_preprocess) {
         auto ifs = input_absolute.str().str();
         _output = get_temporary_path(std::to_string(std::hash<std::string>{}(ifs))+".o");
      }
      _output = to_absolute_path(_output).str().str();

      auto input_absolute_path = input_absolute;
      llvm::sys::path::remove_filename(input_absolute_path);
      include_dirs.insert(input_absolute_path.str().str());

      local_args.emplace_back("-fsyntax-only");
      if (!link && !is_preprocess) {
         local_args.emplace_back("-fplugin="+eosio::cdt::whereami::where()+"/../lib/libeosio_codegen"+SHARED_LIB_SUFFIX);

         std::string codegen_opts;
         codegen_opts += "output="+_output;
         if (contract_name.size()) {
            codegen_opts += ",contract="+contract_name;
         }
         local_args.emplace_back("-Xclang");
         local_args.emplace_back("-plugin-arg-eosio_codegen");
         local_args.emplace_back("-Xclang");
         local_args.emplace_back(codegen_opts);
      } else {
         local_args.emplace_back("");
      }

      std::string abigen_opts;
      if (contract_name.size()) {
         abigen_opts += "contract="+contract_name;
      }
      if (abi_version.size()) {
         if (abigen_opts.size())
            abigen_opts += ",";
         abigen_opts += "abi_version="+abi_version;
      }
      if (no_abigen) {
         if (abigen_opts.size())
            abigen_opts += ",";
         abigen_opts += "no_abigen";
      }
      if (suppress_ricardian_warnings) {
         if (abigen_opts.size())
            abigen_opts += ",";
         abigen_opts += "suppress_ricardian_warnings";
      }
      if (resource_dirs.size()) {
         for (const auto& r : resource_dirs) {
            if (abigen_opts.size())
               abigen_opts += ",";
            abigen_opts += "R="+r;
         }
      }
      if (abigen_opts.size() && !link) {
         local_args.emplace_back("-Xclang");
         local_args.emplace_back("-plugin-arg-eosio_abigen");
         local_args.emplace_back("-Xclang");
         local_args.emplace_back(abigen_opts);
      }

      local_args.emplace_back(input);

      if (auto ret = blanc::exec_subprogram(backend, local_args, has_v) || is_preprocess) {
         return ret;
      }

      auto tmp_file = _output + ".actions.cpp";
      if (llvm::sys::fs::exists(tmp_file)) {
         new_inputs.emplace_back(tmp_file);
         tmp_inputs.emplace_back(tmp_file);
      }
   }
#endif

   if (link && inputs.size()) {
      args.insert(args.begin(), "-fuse-ld="+eosio::cdt::whereami::where()+"/"+LINKER_NAME);
      if (has_v){
         args.emplace_back("-Wl,--show-commands");
      }
      if (keep_generated){
         args.emplace_back("-Wl,--keep-generated");
      }
      if (output.size()) {
         args.emplace_back("-o "+output);
      }
   }
   else {
      args.emplace_back("-w");
   }
   
   for (const auto& input : new_inputs) {
      args.push_back(input);
   }
   for (const auto& inc : include_dirs) {
      args.push_back("-I"+inc);
   }
   if (auto ret = blanc::exec_subprogram(backend, args, has_v)) {
      return ret;
   }
   return 0;
}
