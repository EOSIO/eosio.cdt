#pragma once
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/QualTypeNames.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"

#include <eosio/gen.hpp>

#include <eosio/utils.hpp>
#include <eosio/whereami/whereami.hpp>
#include <eosio/abi.hpp>

#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <set>
#include <map>
#include <chrono>
#include <ctime>
#include <utility>

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

namespace eosio { namespace cdt {
   struct codegen_exception : public std::exception {
      virtual const char* what() const throw() {
         return "eosio.codegen fatal error";
      }
   };
   
   struct include_double {
      include_double(std::string fn, SourceRange sr) : file_name(fn), range(sr) {}
      std::string    file_name;
      SourceRange    range;
   };

   class codegen : public generation_utils {
      public:
         codegen_exception codegen_ex;
         Rewriter          codegen_rewriter;
         CompilerInstance* codegen_ci;
         std::string       contract_name;
         std::string       abi;
         std::set<std::string> defined_datastreams;
         std::set<std::string> datastream_uses;
         std::set<std::string> actions;
         ASTContext *ast_context;
         std::map<std::string, CXXMethodDecl*> cxx_methods;
         std::map<std::string, CXXRecordDecl*> cxx_records;
         std::map<std::string, RecordDecl*>    records;
         llvm::sys::fs::TempFile*              tmp_file;
         llvm::ArrayRef<std::string>           sources;
         size_t                                source_index = 0;
         std::map<std::string, std::string>    tmp_files;

         codegen() : generation_utils([&](){throw codegen_ex;}) {
         }

         static codegen& get() {
            static codegen inst;
            return inst;
         }
         
         void set_contract_name(std::string cn) {
            contract_name = cn;
         }
         
         void set_abi(std::string s) {
            abi = s;
         }

         void add_action( const clang::CXXRecordDecl* decl ) {
            std::string action_name;
            auto _action_name = decl->getEosioActionAttr()->getName();
            if (_action_name.empty()) {
               try {
                  validate_name( decl->getName().str(), error_handler );
               } catch (...) {
                  std::cout << "Error, name <" <<decl->getName().str() << "> is an invalid EOSIO name.\n";
                  throw;
               }
               action_name = decl->getName().str();
            }
            else {
               try {
                  validate_name( _action_name.str(), error_handler );
               } catch (...) {
                  std::cout << "Error, name <" << _action_name.str() << "> is an invalid EOSIO name.\n";
                  throw;
               }
               action_name = _action_name.str();
            }
         }

         void add_action( const clang::CXXMethodDecl* decl ) {
            std::string action_name;
            auto _action_name = decl->getEosioActionAttr()->getName();

            if (_action_name.empty()) {
               try {
                  validate_name( decl->getNameAsString(), error_handler );
               } catch (...) {
                  std::cout << "Error, name <" <<decl->getNameAsString() << "> is an invalid EOSIO name.\n";
               }
               action_name = decl->getNameAsString();
            }
            else {
               try {
                  validate_name( _action_name.str(), error_handler );
               } catch (...) {
                  std::cout << "Error, name <" << _action_name.str() << "> is an invalid EOSIO name.\n";
               }
               action_name = _action_name.str();
            }
         }
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

   class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
      private:
         codegen& cg = codegen::get();
         FileID    main_fid;
         StringRef main_name;
         Rewriter  rewriter;
         CompilerInstance* ci;

      public:
         explicit eosio_codegen_visitor(CompilerInstance *CI) 
               : generation_utils([&](){throw cg.codegen_ex;}), ci(CI) {
            cg.ast_context = &(CI->getASTContext());
            cg.codegen_ci = CI;
            rewriter.setSourceMgr(CI->getASTContext().getSourceManager(), CI->getASTContext().getLangOpts());
         }

         void set_main_fid(FileID fid) {
            main_fid = fid;
         }
         
         void set_main_name(StringRef mn) {
            main_name = mn;
         } 
         
         Rewriter& get_rewriter() {
            return rewriter;
         }

         bool is_datastream(const QualType& qt) {
            auto str_name = qt.getAsString();
            auto ds_re    = std::regex("(((class eosio::)?datastream<[a-zA-Z]+[a-zA-Z0-9]*.*>)|(DataStream)) &"); 
            if (std::regex_match(str_name, ds_re))
               return true;
            return false;
         }
         bool is_type_of(const QualType& qt, const std::string& t, const std::string& ns="") {
            return true;
         }

         template <size_t N>
         void emitError(CompilerInstance& inst, SourceLocation loc, const char (&err)[N]) {
            FullSourceLoc full(loc, inst.getSourceManager());
            unsigned id = inst.getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Error, err);
            inst.getDiagnostics().Report(full, id);
         }

         std::string get_base_type(const QualType& qt) {
            std::istringstream ss(qt.getAsString());
            std::vector<std::string> results((std::istream_iterator<std::string>(ss)),
                                             std::istream_iterator<std::string>());
            for (auto s : results) {
               if (s == "const"  || s == "volatile" ||
                  s == "struct" || s == "class" ||
                  s == "&"      || s == "*")
                  continue;
               return s;
            }
            return "";
         }

         virtual bool VisitFunctionTemplateDecl(FunctionTemplateDecl* decl) {
            if (decl->getNameAsString() == "operator<<") {
               if (decl->getTemplatedDecl()->getNumParams() == 2) {
                  auto param0 = decl->getTemplatedDecl()->getParamDecl(0)->getOriginalType();
                  if (is_datastream(param0)) {
                     if (auto tp = dyn_cast<NamedDecl>(decl->getTemplatedDecl()->getParamDecl(1)->getOriginalType().getTypePtr()->getPointeeCXXRecordDecl())) {
                        cg.defined_datastreams.insert(tp->getQualifiedNameAsString());
                     }
                  }
               }
            }
            return true;
         }

         void create_action_dispatch(CXXMethodDecl* decl) {
            constexpr static uint32_t max_stack_size = 512;
            std::stringstream ss;
            codegen& cg = codegen::get();
            std::string nm = decl->getNameAsString()+"_"+decl->getParent()->getNameAsString();
            if (cg.is_eosio_contract(decl, cg.contract_name)) {
               ss << "extern \"C\" __attribute__((eosio_wasm_action(\"";
               ss << generation_utils::get_action_name(decl);
               ss << ":";
               ss << "__eosio_action_" << nm;
               ss << "\"))) void __eosio_action_" << nm << "(unsigned long long r, unsigned long long c, unsigned long long a) {\n";
               ss << "size_t as = action_data_size();\n";
               ss << "if (as <= 0) return;\n";
               ss << "void* buff = as >= " << max_stack_size << " ? malloc(as) : alloca(as);\n";
               ss << "read_action_data(buff, as);\n";
               ss << "eosio::datastream<const char*> ds{(char*)buff, as};\n";
               int i=0;
               for (auto param : decl->parameters()) {
                  clang::LangOptions lang_opts;
                  lang_opts.CPlusPlus = true;
                  clang::PrintingPolicy policy(lang_opts);
                  auto qt = param->getOriginalType().getNonReferenceType();
                  qt.removeLocalConst();
                  qt.removeLocalVolatile();
                  qt.removeLocalRestrict();
                  std::string tn = clang::TypeName::getFullyQualifiedName(qt, *(cg.ast_context), policy);
                  tn = tn == "_Bool" ? "bool" : tn; // TODO look out for more of these oddities
                  ss << tn << " arg" << i << "; ds >> arg" << i++ << ";\n";
               }
               ss << decl->getParent()->getQualifiedNameAsString() << "{eosio::name{r},eosio::name{c},ds}." << decl->getNameAsString() << "(";
               for (int i=0; i < decl->parameters().size(); i++) {
                  ss << "arg" << i;
                  if (i < decl->parameters().size()-1)
                     ss << ", ";
               }
               ss << ");";
               ss << "}\n";

               rewriter.InsertTextAfter(ci->getSourceManager().getLocForEndOfFile(main_fid), ss.str());
            }
         }


         virtual bool VisitCXXMethodDecl(CXXMethodDecl* decl) {
            std::string method_name = decl->getNameAsString();
            if (decl->isEosioAction()) {
               method_name = generation_utils::get_action_name(decl);
               if (cg.actions.count(decl->getNameAsString()) == 0)
                  if (cg.actions.count(method_name) == 0)
                     create_action_dispatch(decl);
                  else
                     emitError(*ci, decl->getLocation(), "action already defined elsewhere");
               cg.actions.insert(decl->getNameAsString()); // insert the method action, so we don't create the dispatcher twice
               cg.actions.insert(method_name);
               for (auto param : decl->parameters()) {
                  if (auto tp = dyn_cast<NamedDecl>(param->getOriginalType().getTypePtr()->getAsCXXRecordDecl())) {
                     cg.datastream_uses.insert(tp->getQualifiedNameAsString());
                  }
               }
            }
            cg.cxx_methods.emplace(method_name, decl);
            return true;
         }
         virtual bool VisitRecordDecl(RecordDecl* decl) {
            std::string rec_name = decl->getQualifiedNameAsString();
            cg.records.emplace(rec_name, decl);
            return true;
         }
         virtual bool VisitCallExpr(CallExpr* expr) {
            if (auto callee = expr->getDirectCallee()) {
               if (callee->getNumParams() == 2) {
                  if (is_datastream(callee->getParamDecl(0)->getOriginalType())) {
                     cg.datastream_uses.insert(get_base_type(callee->getParamDecl(1)->getOriginalType()));
                  }
               }
            }
            return true;
         }
         virtual bool VisitCXXRecordDecl(CXXRecordDecl* decl) {
            std::string rec_name = decl->getQualifiedNameAsString();
            if (decl->isEosioAction()) {
               rec_name = generation_utils::get_action_name(decl);
               cg.actions.insert(rec_name);
            }
            cg.cxx_records.emplace(rec_name, decl);
            return true;
         }
      };

      class eosio_codegen_consumer : public ASTConsumer {
      private:
         eosio_codegen_visitor *visitor;
         std::string main_file;
         CompilerInstance* ci;

      public:
         explicit eosio_codegen_consumer(CompilerInstance *CI, std::string file)
            : visitor(new eosio_codegen_visitor(CI)), main_file(file), ci(CI) { }
         
         virtual void HandleTranslationUnit(ASTContext &Context) {
            codegen& cg = codegen::get();
            auto& src_mgr = Context.getSourceManager();
            auto& f_mgr = src_mgr.getFileManager();
            auto main_fe = f_mgr.getFile(main_file);
            if (main_fe) {
               auto fid = src_mgr.getOrCreateFileID(f_mgr.getFile(main_file), SrcMgr::CharacteristicKind::C_User);
               visitor->set_main_fid(fid);
               visitor->set_main_name(main_fe->getName());
               visitor->TraverseDecl(Context.getTranslationUnitDecl());
               int fd;
               llvm::SmallString<128> fn;
               try {
                  SmallString<64> res;
                  llvm::sys::path::system_temp_directory(true, res);

                  std::ofstream out(std::string(res.c_str())+"/"+llvm::sys::path::filename(main_fe->getName()).str()+"-tmp.cpp");
                  for (auto inc : global_includes[main_file]) {
                     visitor->get_rewriter().ReplaceText(inc.range, 
                           std::string("\"")+inc.file_name+"\"\n");
                  }
                  // generate apply stub with abi
                  std::stringstream ss;
                  ss << "extern \"C\" {\n";
                  ss << "\t__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi(";
                  std::string abi = cg.abi;
                  ss << std::quoted(abi);
                  ss << ")))\n";
                  ss << "\tvoid __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){}\n";
                  ss << "}";
                  visitor->get_rewriter().InsertTextAfter(ci->getSourceManager().getLocForEndOfFile(fid), ss.str());
                  auto& RewriteBuf = visitor->get_rewriter().getEditBuffer(fid);
                  out << std::string(RewriteBuf.begin(), RewriteBuf.end());
                  cg.tmp_files.emplace(main_file, fn.str());
                  out.close();
               } catch (...) {
                  llvm::outs() << "Failed to create temporary file\n";
               }
            }
         }

      };

      class eosio_codegen_frontend_action : public ASTFrontendAction {
      public:
         virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
            CI.getPreprocessor().addPPCallbacks(std::make_unique<eosio_ppcallbacks>(CI.getSourceManager(), file.str()));
            return std::make_unique<eosio_codegen_consumer>(&CI, file);
         }
   };

}} // ns eosio::cdt
