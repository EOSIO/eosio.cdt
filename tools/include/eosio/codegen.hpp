#pragma once
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
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
         std::stringstream codegen_buff; 
         codegen_exception codegen_ex;
         Rewriter          codegen_rewriter;
         CompilerInstance* codegen_ci;
         std::set<FileID>  codegen_rewritten;
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

         codegen() : generation_utils([&](){throw codegen_ex;}) {
         }

         static codegen& get() {
            static codegen inst;
            return inst;
         }

         void set_sources(const llvm::ArrayRef<std::string>& srcs) {
            sources = srcs;
         }
         std::string get_source() {
            return sources[source_index];
         }
         void reset() {
            static bool has_reset = false;
            if (!has_reset) {
               has_reset = true;
               return;
            }
            source_index++;
            codegen_buff.str("");
         }
         void set_temp_file_name(const std::string& nm) {
            auto tf = llvm::sys::fs::TempFile::create(nm, llvm::sys::fs::perms::all_write);
            if (tf)
               tmp_file = &(*tf);
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
         void insert_fid(Decl* decl) {
            cg.codegen_rewritten.insert(std::get<0>(cg.codegen_ci->getSourceManager().getDecomposedLoc(decl->getLocation())));
         }

      public:
         explicit eosio_codegen_visitor(CompilerInstance *CI) 
               : generation_utils([&](){throw cg.codegen_ex;}) {
            cg.ast_context = &(CI->getASTContext());
            cg.codegen_rewriter.setSourceMgr(cg.ast_context->getSourceManager(), cg.ast_context->getLangOpts());
            cg.codegen_ci = CI;
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
            codegen& cg = codegen::get();
            cg.codegen_buff << "extern \"C\" void __eosio_action_" << decl->getNameAsString() << "(uint64_t r, uint64_t c, uint64_t a) {";
            cg.codegen_buff << "if (a == eosio::name{\"" << generation_utils::get_action_name(decl) << "\"}.value){";
            cg.codegen_buff << "size_t as = action_data_size();";
            cg.codegen_buff << "if (as <= 0) return;";
            cg.codegen_buff << "void* buff = as >= " << max_stack_size << " ? malloc(as) : alloca(as);";
            cg.codegen_buff << "read_action_data(buff, as);";
            cg.codegen_buff << "eosio::datastream<const char*> ds{(char*)buff, as};";
            int i=0;
            for (auto param : decl->parameters()) {
               clang::LangOptions lang_opts;
               lang_opts.CPlusPlus = true;
               clang::PrintingPolicy policy(lang_opts);
               std::string tn = clang::TypeName::getFullyQualifiedName(param->getOriginalType().getNonReferenceType(), *(cg.ast_context), policy);
               cg.codegen_buff << tn << " arg" << i++ << " << ds;";
            }
            cg.codegen_buff << decl->getParent()->getQualifiedNameAsString() << "{r,c,ds}." << decl->getNameAsString() << "(";
            for (int i=0; i < decl->parameters().size(); i++) {
               cg.codegen_buff << "arg" << i;
               if (i < decl->parameters().size()-1)
                  cg.codegen_buff << ", ";
            }
            cg.codegen_buff << ");";
            cg.codegen_buff << "}\n";
         }
         virtual bool VisitCXXMethodDecl(CXXMethodDecl* decl) {
            std::string method_name = decl->getNameAsString();
            if (decl->isEosioAction()) {
               //insert_fid(decl);
               method_name = generation_utils::get_action_name(decl);
               if (cg.actions.count(method_name) == 0)
                  create_action_dispatch(decl);
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
            //insert_fid(decl);
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
            //insert_fid(decl);
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

      public:
         explicit eosio_codegen_consumer(CompilerInstance *CI)
            : visitor(new eosio_codegen_visitor(CI)) // initialize the visitor
         { }

         virtual void HandleTranslationUnit(ASTContext &Context) {
            codegen& cg = codegen::get();
            cg.reset();
            auto& src_mgr = Context.getSourceManager();
            auto& f_mgr = src_mgr.getFileManager();
            auto fid = src_mgr.getOrCreateFileID(f_mgr.getFile(cg.get_source()), SrcMgr::CharacteristicKind::C_User);
            visitor->TraverseDecl(Context.getTranslationUnitDecl());
            for (auto fi : codegen_rewritten) {
               auto& RewriteBuf =
                  codegen_rewriter.getEditBuffer(fi);
               llvm::outs() << std::string(RewriteBuf.begin(), RewriteBuf.end());
            }

            llvm::outs() << codegen::get().codegen_buff.str() << "\n";
            int fd;
            llvm::SmallString<128> fn;
            if (auto fe = Context.getSourceManager().getFileEntryForID(fid)) {
               auto ec = llvm::sys::fs::createTemporaryFile(fe->getName(),"tmp", fd, fn);
               if (!ec) {
                  llvm::raw_fd_ostream fdo(fn, ec);
                  if (ec) {
                     llvm::outs() << "error: (couldn't open " << fn << ")\n";
                     throw cg.codegen_ex;
                  } else {
                     fdo << cg.codegen_buff.str();
                  }
               } else {
                  llvm::outs() << "Oops " << fe->getName() << "\n";
               }
            } else {
               llvm::outs() << "Why?\n";
            }
         }
      };

      class eosio_codegen_frontend_action : public ASTFrontendAction {
      public:
         virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
            return std::make_unique<eosio_codegen_consumer>(&CI);
         }
   };

}} // ns eosio::cdt
