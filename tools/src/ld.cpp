#include <llvm/Option/ArgList.h>
#include <llvm/Option/OptTable.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>

#include <blanc/profile.hpp>
#include <blanc/utils.hpp>
#include <eosio/abigen.hpp>
#include <eosio/abimerge.hpp>
#include <eosio/utils.hpp>

#include <unistd.h>
#include <fstream>

#define ONLY_LD

using namespace llvm;
using namespace llvm::opt;
using namespace blanc;
using namespace eosio::cdt;

enum {
   OPT_INVALID = 0,
#define OPTION(_1, _2, ID, _4, _5, _6, _7, _8, _9, _10, _11, _12) OPT_##ID,
#include "ld_options.inc"
#undef OPTION
};

#define PREFIX(NAME, VALUE) static const char* const NAME[] = VALUE;
#include "ld_options.inc"
#undef PREFIX

static const OptTable::Info InfoTable[] = {
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, ALIASARGS, FLAGS, PARAM,    \
               HELPTEXT, METAVAR, VALUES)                                        \
  {PREFIX, NAME,  HELPTEXT,    METAVAR,     OPT_##ID,  Option::KIND##Class, \
   PARAM,  FLAGS, OPT_##GROUP, OPT_##ALIAS, ALIASARGS, VALUES},
#include "ld_options.inc"
#undef OPTION
};

class BlancLdOptTable : public OptTable {
public:
   BlancLdOptTable(): OptTable(InfoTable) {}
};

bool is_wasm_target;
profile _profile = profile::eosio;
std::vector<std::string> inputs;
std::string output;

int abi_version_major = 1;
int abi_version_minor = 1;
bool no_abigen;

void generate_eosio_dispatch(const std::string& output, const std::set<wasm_action>& wasm_actions, const std::set<wasm_notify>& wasm_notifies) {
   try {

      std::ofstream ofs(output);
      if (!ofs) throw;
      ofs << "#include <cstdint>\n";
      ofs << "extern \"C\" {\n";
      ofs << "  __attribute__((eosio_wasm_import)) void eosio_assert_code(uint32_t, uint64_t);";
      ofs << "  void eosio_set_contract_name(uint64_t n);\n";
      for (auto& wa : wasm_actions) {
         ofs << "  void " << wa.handler << "(uint64_t r, uint64_t c);\n";
      }
      for (auto& wn : wasm_notifies) {
         ofs << "  void " << wn.handler << "(uint64_t r, uint64_t c);\n";
      }
      ofs << "  __attribute__((weak, eosio_wasm_entry))\n";
      ofs << "  void apply(uint64_t r, uint64_t c, uint64_t a) {\n";
      ofs << "    eosio_set_contract_name(r);\n";
      ofs << "    if (c == r) {\n";
      if (wasm_actions.size()) {
         ofs << "      switch (a) {\n";
         for (auto& wa : wasm_actions) {
            ofs << "      case " << string_to_name(wa.name.c_str()) << "ull:\n";
            ofs << "        " << wa.handler << "(r, c);\n";
            ofs << "        break;\n";
            
         }
         // assert that no action was found when the receiver is not "eosio"
         ofs << "      default:\n"
             << "        if ( r != " << string_to_name("eosio") << "ull) eosio_assert_code(false, 1);\n"
             << "      }\n";
      }
      ofs << "    } else {\n";
      if (wasm_notifies.size()) {
         std::string action;
         for (auto& wn : wasm_notifies) {
            if (wn.name != action) {
               if (action.empty()) {
                  ofs << "      if (a == " << string_to_name(wn.name.c_str()) << "ull) {\n";
               } else {
                  ofs << "        }\n";
                  ofs << "      }\n";
                  ofs << "      else if (a == " << string_to_name(wn.name.c_str()) << "ull) {\n";
               }
               ofs << "        switch (c) {\n";
               action = wn.name;
            }
            if (wn.contract != "*")
               ofs << "        case " << string_to_name(wn.contract.c_str()) << "ull:\n";
            else
               ofs << "        default:\n";
            ofs << "          " << wn.handler << "(r, c);\n";
            ofs << "          break;\n";
         }
         ofs << "        }\n";
         ofs << "      }\n";
      }
      ofs << "    }\n";
      ofs << "  }\n";
      ofs << "}\n";
      ofs.close();
   } catch (...) {
      llvm::outs() << "Failed to generate eosio dispatcher\n";
   }
}

std::string generate_cosmwasm_dispatch(const std::set<wasm_action>& wasm_actions, const std::set<wasm_notify>& wasm_notifies) {
   std::string output;
   try {
   } catch (...) {
      llvm::outs() << "Failed to generate cosmwasm dispatcher\n";
   }
   return output;
}

std::vector<std::string> override_link_options(InputArgList& Args) {
   std::vector<std::string> new_opts;

   if (auto target = Args.getLastArgNoClaim(OPT_m)) {
      if (std::string(target->getValue()).starts_with("wasm32")) {
         is_wasm_target = true;
      }
   } else {
      is_wasm_target = true;
   }

   if (is_wasm_target) {

      auto input_args = Args.filtered(OPT_INPUT);
      for (const auto& arg : input_args) {
         inputs.emplace_back(arg->getValue());
      }

      if (!Args.hasArgNoClaim(OPT_gc_sections, OPT_no_gc_sections)) {
         new_opts.emplace_back("--gc-sections");
      }
      if (!Args.hasArgNoClaim(OPT_entry)) {
         new_opts.emplace_back("--no-entry");
      }
      if (!Args.hasArgNoClaim(OPT_strip_all, OPT_strip_debug)) {
         new_opts.emplace_back("--strip-all");
      }
      if (!Args.hasArgNoClaim(OPT_merge_data_segments, OPT_no_merge_data_segments)) {
         new_opts.emplace_back("--no-merge-data-segments");
      }
      if (!Args.hasFlag(OPT_stack_first, OPT_INVALID)) {
         new_opts.emplace_back("--stack-first");
      }
      if (!Args.hasArgNoClaim(OPT_lto_O)) {
         new_opts.emplace_back("--lto-O3");
      }
      if (Args.hasArgNoClaim(OPT_no_abigen)) {
         no_abigen = true;
      }
      if (auto arg = Args.getLastArgNoClaim(OPT_abi_version)) {
         std::string abi_version = arg->getValue();
         float tmp;
         abi_version_major = std::stoi(abi_version);
         abi_version_minor = (int)(std::modf(std::stof(abi_version), &tmp) * 10);
      }
      new_opts.emplace_back("-export=__heap_base");
      new_opts.emplace_back("-export=__data_end");
      new_opts.emplace_back("-lc++");
      new_opts.emplace_back("-lc");

      if (_profile == profile::eosio) {
         auto opt_z= Args.getLastArgNoClaim(OPT_z);
         if (!opt_z || strncmp("stack-size", opt_z->getValue(), sizeof("stack-size")-1) != 0 ) {
            new_opts.emplace_back("-zstack-size=8192");
         }
         new_opts.emplace_back("-lcstdio");
         new_opts.emplace_back("--allow-undefined-file="+eosio::cdt::whereami::where()+"/../eosio.imports");
      } 

      if (auto arg = Args.getLastArgNoClaim(OPT_o)) {
         output  = arg->getValue();
      } else if (inputs.size() == 1) {
         llvm::SmallString<PATH_MAX> fn = llvm::sys::path::filename(inputs[0]);
         llvm::sys::path::replace_extension(fn, ".wasm");
         output = fn.str().str();
      } else {
         output = "a.out";
      }

      // fix for macos
      Args.eraseArg(OPT_headerpad_max_install_names);
      Args.eraseArg(OPT_search_paths_first);
   }

   Args.eraseArg(OPT_abi_version);
   Args.eraseArg(OPT_no_abigen);

   for (const auto arg : Args) {
      new_opts.emplace_back(arg->getAsString(Args));
   }
   if (is_wasm_target) {
      new_opts.emplace_back("-L"+eosio::cdt::whereami::where()+"/../lib");
   }
   return new_opts;
}

int main(int argc, const char** argv) {

   std::set<wasm_action> wasm_actions;
   std::set<wasm_notify> wasm_notifies;
   std::vector<std::string> desc_files;
   bool dispatcher_was_found = false;

   ArrayRef<const char*> Argv{argv+1, argv+argc};
   unsigned missingIndex;
   unsigned missingCount;

   BlancLdOptTable opts;
   auto Args = opts.ParseArgs(Argv, missingIndex, missingCount);

   bool keep_generated = Args.hasArgNoClaim(OPT_keep_generated);
   if (keep_generated) Args.eraseArg(OPT_keep_generated);
   bool show_commands = Args.hasArgNoClaim(OPT_show_commands);
   if (show_commands) Args.eraseArg(OPT_show_commands);

   auto args = override_link_options(Args);

   std::vector<std::string> tmp_inputs;
   blanc::scope_exit on_exit([&tmp_inputs, keep_generated]() {
      if (!keep_generated) {
         for (const auto& tmp_file : tmp_inputs) {
            llvm::sys::fs::remove(tmp_file);
         }
      }
   });

   if (is_wasm_target) {
      try {
         ojson abi;
         for (const auto& input : inputs) {
            auto desc_name = input.substr(0, input.rfind("-"))+".desc";
            if (!llvm::sys::fs::exists(desc_name)) {
               desc_name = to_absolute_path(input).str().str() +".desc";
            }
            if (llvm::sys::fs::exists(desc_name)) {
               std::ifstream ifs(desc_name);
               auto desc = ojson::parse(ifs);
               ifs.close();

               if (abi.empty()) {
                  abigen ag;
                  ag.set_abi_version(abi_version_major, abi_version_minor);
                  abi = ag.to_json();
               }
               abi = ABIMerger(abi).merge(desc);

               for (auto wa : desc["wasm_actions"].array_range()) {
                  wasm_action act;
                  act.name = wa["name"].as_string();
                  act.handler = wa["handler"].as_string();
                  wasm_actions.insert(act);
               }
               for (auto wn : desc["wasm_notifies"].array_range()) {
                  wasm_notify noti;
                  noti.contract = wn["contract"].as_string();
                  noti.name = wn["name"].as_string();
                  noti.handler = wn["handler"].as_string();
                  wasm_notifies.insert(noti);
               }
               if (_profile == profile::eosio && !dispatcher_was_found) {
                  for (auto we : desc["wasm_entries"].array_range()) {
                     auto name = we.as_string();
                     if (name == "apply") {
                        dispatcher_was_found = true;
                        break;
                     }
                  }
               }
               desc_files.push_back(desc_name);
               tmp_inputs.emplace_back(desc_name);
            }
         }
         if (!no_abigen && !abi.empty()) {
            llvm::SmallString<PATH_MAX> output_ref = llvm::StringRef(output);
            llvm::sys::path::replace_extension(output_ref, ".abi");

            std::ofstream ofs(output_ref.str().str());
            if (!ofs) throw;
            ofs << pretty_print(abi);
            ofs.close();
         }
         if (_profile == profile::eosio && !dispatcher_was_found && (!wasm_actions.empty() || !wasm_notifies.empty())) {
            std::string main_file = output + ".dispatch.cpp";
            generate_eosio_dispatch(main_file, wasm_actions, wasm_notifies);
            tmp_inputs.emplace_back(main_file);
            if (!main_file.empty()) {
               std::vector<std::string> new_opts { "-c", "-o", main_file+".o" , main_file , "--no-missing-ricardian-clause"};
                  if (auto ret = blanc::exec_subprogram(COMPILER_NAME, new_opts, show_commands)) {
                  return ret;
               }
               args.push_back(main_file+".o");
               tmp_inputs.emplace_back(main_file+".o");
            }
         }
      } catch (std::runtime_error& err) {
         llvm::errs() << err.what() << '\n';
         return -1;
      }

      const char* backend = std::getenv("EOSIO_LINKER_BACKEND");
      if (!backend) {
         backend = TOOL_BACKEND;
      }

      if (auto ret = blanc::exec_subprogram(backend, args, show_commands)) {
         return ret;
      }

      if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/"+POSTPASS_NAME)) {
         if (auto ret = blanc::exec_subprogram(POSTPASS_NAME, {"--profile="+std::to_string(_profile), output}, show_commands)) {
            return ret;
         }
      }
   } else {
      if (auto ret = blanc::exec_subprogram(eosio::cdt::whereami::where() + "/ld", args, show_commands)) {
         return ret;
      }
   }

   return 0;
}
