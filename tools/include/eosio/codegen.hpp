#pragma once
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
   extern codegen_exception codegen_ex;
   extern Rewriter          codegen_rewriter;
   extern CompilerInstance* codegen_ci;
   extern std::set<FileID>  codegen_rewritten;
   class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
   private:
      ASTContext *ast_context;
      std::set<CXXRecordDecl*> cxx_records;
      void insert_fid(Decl* decl) {
         codegen_rewritten.insert(std::get<0>(codegen_ci->getSourceManager().getDecomposedLoc(decl->getLocation())));
      }

   public:
      explicit eosio_codegen_visitor(CompilerInstance *CI) 
            : generation_utils([&](){throw codegen_ex;}), ast_context(&(CI->getASTContext())) {
         codegen_rewriter.setSourceMgr(ast_context->getSourceManager(), ast_context->getLangOpts());
         codegen_ci = CI;
      }

      virtual bool VisitCXXRecordDecl(CXXRecordDecl* decl) {
         cxx_records.insert(decl);
         if (decl->getNameAsString() == "hello") {
            insert_fid(decl);
            codegen_rewriter.InsertText(decl->getLocation(), "// this is a pp\n", true, true); 
         }
         return true;
      }
   
      virtual bool VisitFunctionDecl(FunctionDecl *func) {
         std::string funcName = func->getNameInfo().getName().getAsString();
         if (funcName == "do_math") {
               codegen_rewriter.ReplaceText(func->getLocation(), funcName.length(), "add5");
               errs() << "** Rewrote function def: " << funcName << "\n";
         }    
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
