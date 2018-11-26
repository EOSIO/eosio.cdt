#pragma once
#include "llvm/Support/raw_ostream.h"
#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
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
   extern llvm::raw_string_ostream codegen_buff; 
   extern codegen_exception codegen_ex;
   extern Rewriter          codegen_rewriter;
   extern CompilerInstance* codegen_ci;
   extern std::set<FileID>  codegen_rewritten;
   class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
   private:
      std::set<std::string> defined_datastreams;
      std::set<std::string> datastream_uses;
      std::set<std::string> actions;
      ASTContext *ast_context;
      std::map<std::string, CXXMethodDecl*> cxx_methods;
      std::map<std::string, CXXRecordDecl*> cxx_records;
      std::map<std::string, RecordDecl*>    records;
      void insert_fid(Decl* decl) {
         codegen_rewritten.insert(std::get<0>(codegen_ci->getSourceManager().getDecomposedLoc(decl->getLocation())));
      }

   public:
      explicit eosio_codegen_visitor(CompilerInstance *CI) 
            : generation_utils([&](){throw codegen_ex;}), ast_context(&(CI->getASTContext())) {
         codegen_rewriter.setSourceMgr(ast_context->getSourceManager(), ast_context->getLangOpts());
         codegen_ci = CI;
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
                     defined_datastreams.insert(tp->getQualifiedNameAsString());
                  }
               }
            }
         }
         return true;
      }
      virtual bool VisitCXXMethodDecl(CXXMethodDecl* decl) {
         std::string method_name = decl->getNameAsString();
         if (decl->isEosioAction()) {
            method_name = generation_utils::get_action_name(decl);
            actions.insert(method_name);
            for (auto param : decl->parameters()) {
               if (auto tp = dyn_cast<NamedDecl>(param->getOriginalType().getTypePtr()->getAsCXXRecordDecl())) {
                  datastream_uses.insert(tp->getQualifiedNameAsString());
               }
            }
         }
         cxx_methods.emplace(method_name, decl);
         return true;
      }
      virtual bool VisitRecordDecl(RecordDecl* decl) {
         std::string rec_name = decl->getQualifiedNameAsString();
         records.emplace(rec_name, decl);
         return true;
      }
      virtual bool VisitCallExpr(CallExpr* expr) {
         if (auto callee = expr->getDirectCallee()) {
            if (callee->getNumParams() == 2) {
               if (is_datastream(callee->getParamDecl(0)->getOriginalType())) {
                  datastream_uses.insert(get_base_type(callee->getParamDecl(1)->getOriginalType()));
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
            actions.insert(rec_name);
            //codegen_buff << "void __eosio_action_" << decl->getNameAsString() << "(uint64_t a, uint64_t b, 
            //codegen_rewriter.InsertText(decl->getTranslationUnitDecl()->getEndLoc(), "// this is a pp\n", true, true); 
         }
         cxx_records.emplace(rec_name, decl);
         return true;
      }
   
      virtual bool VisitFunctionDecl(FunctionDecl *func) {
         std::string funcName = func->getNameInfo().getName().getAsString();
         //outs() << func->getNameAsString() << "\n";
         /*
         if (funcName == "do_math") {
               codegen_rewriter.ReplaceText(func->getLocation(), funcName.length(), "add5");
               errs() << "** Rewrote function def: " << funcName << "\n";
         }    
         */
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
         visitor->TraverseDecl(Context.getTranslationUnitDecl());
      }
   };

   class eosio_codegen_frontend_action : public ASTFrontendAction {
   public:
      virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
         return std::make_unique<eosio_codegen_consumer>(&CI);
      }
   };

   class codegen : public generation_utils {
      public:

      codegen() : generation_utils([&](){throw codegen_ex;}) {
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
}} // ns eosio::cdt
