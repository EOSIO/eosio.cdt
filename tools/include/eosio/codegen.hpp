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

   class codegen : public generation_utils {
      public:
         codegen_exception codegen_ex;
         Rewriter          codegen_rewriter;
         CompilerInstance* codegen_ci;
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

   class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
      private:
         codegen& cg = codegen::get();
         FileID   main_fid;
         Rewriter rewriter;
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
            ss << "extern \"C\" void __eosio_action_" << decl->getNameAsString() << "(uint64_t r, uint64_t c, uint64_t a) {\n";
            ss << "if (a == eosio::name{\"" << generation_utils::get_action_name(decl) << "\"}.value){\n";
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
               std::string tn = clang::TypeName::getFullyQualifiedName(param->getOriginalType().getNonReferenceType(), *(cg.ast_context), policy);
               ss << tn << " arg" << i << "; ds >> arg" << i++ << ";\n";
            }
            ss << decl->getParent()->getQualifiedNameAsString() << "{eosio::name{r},eosio::name{c},ds}." << decl->getNameAsString() << "(";
            for (int i=0; i < decl->parameters().size(); i++) {
               ss << "arg" << i;
               if (i < decl->parameters().size()-1)
                  ss << ", ";
            }
            ss << ");";
            ss << "}}\n";
            rewriter.InsertTextAfter(ci->getSourceManager().getLocForEndOfFile(main_fid), ss.str());
         }
         virtual bool VisitCXXMethodDecl(CXXMethodDecl* decl) {
            std::string method_name = decl->getNameAsString();
            if (decl->isEosioAction()) {
               method_name = generation_utils::get_action_name(decl);
               if (cg.actions.count(method_name) == 0)
                  create_action_dispatch(decl);
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

      public:
         explicit eosio_codegen_consumer(CompilerInstance *CI, std::string file)
            : visitor(new eosio_codegen_visitor(CI)), main_file(file) { }
         
         virtual void HandleTranslationUnit(ASTContext &Context) {
            codegen& cg = codegen::get();
            auto& src_mgr = Context.getSourceManager();
            auto& f_mgr = src_mgr.getFileManager();
            auto main_fe = f_mgr.getFile(main_file);
            if (main_fe) {
               auto fid = src_mgr.getOrCreateFileID(f_mgr.getFile(main_file), SrcMgr::CharacteristicKind::C_User);
               visitor->set_main_fid(fid);
               visitor->TraverseDecl(Context.getTranslationUnitDecl());
               int fd;
               llvm::SmallString<128> fn;
               try {
                  SmallString<64> res;
                  llvm::sys::path::system_temp_directory(true, res);

                  std::ofstream out(std::string(res.c_str())+"/"+llvm::sys::path::filename(main_fe->getName()).str()+"-tmp.cpp");
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
            return std::make_unique<eosio_codegen_consumer>(&CI, file);
         }
   };

}} // ns eosio::cdt
