#pragma once
#include <string>
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

namespace eosio { namespace cdt {
   struct include_double {
      include_double(std::string fn, SourceRange sr) : file_name(fn), range(sr) {}
      std::string    file_name;
      SourceRange    range;
   };
   std::map<std::string, std::vector<include_double>>  global_includes;
   class eosio_ppcallbacks : public PPCallbacks {
      public:
         eosio_ppcallbacks(SourceManager& sm, std::string file) : sources(sm), fn(file) {}
      protected:
         virtual void InclusionDirective(
            SourceLocation hash_loc,
            const Token &include_token,
            StringRef file_name,
            bool is_angled,
            CharSourceRange filename_range,
            const FileEntry *file,
            StringRef search_path,
            StringRef relative_path,
            const clang::Module *imported,
            clang::SrcMgr::CharacteristicKind file_type) {
            auto fid = sources.getFileID(hash_loc);
            auto fe  = sources.getFileEntryForID(fid);

            if (!is_angled && llvm::sys::path::filename(fe->getName()) == llvm::sys::path::filename(fn)) {
               global_includes[fe->getName().str()].emplace_back(
                     (search_path + llvm::sys::path::get_separator() + file_name).str(),
                     filename_range.getAsRange());
            }
         }

         std::string fn;
         SourceManager& sources;
   };
}}
