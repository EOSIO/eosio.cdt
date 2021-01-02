//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"

#include "clang_wrapper.hpp"

using namespace clang;
using namespace eosio::cdt;

namespace {

template <size_t N>
void emitError(CompilerInstance& inst, SourceLocation loc, const char (&err)[N]) {
   FullSourceLoc full(loc, inst.getSourceManager());
   unsigned id = inst.getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Error, err);
   inst.getDiagnostics().Report(full, id);
}

class ValidateVisitor : public RecursiveASTVisitor<ValidateVisitor> {
   static inline bool is_ignorable( const clang::QualType& type ) {
      auto check = [&](const clang::Type* pt) {
        if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt)) {
         if (auto rt = llvm::dyn_cast<clang::RecordType>(tst->desugar())) {
            auto decl = rt->getDecl();
            return clang_wrapper::Decl<decltype(decl)>(decl).isEosioIgnore();
         }
        }

         return false;
      };

      bool is_ignore = false;
      if ( auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr()) )
         is_ignore = check(pt->desugar().getTypePtr());
      else
         is_ignore = check(type.getTypePtr());
      return is_ignore;
   }

   public:
      ValidateVisitor(CompilerInstance& inst) : instance(inst) {}
      bool VisitCXXMethodDecl(CXXMethodDecl* Decl) {
         bool invalid_params = false;
         for (auto param : Decl->parameters()) {
            bool ignore = is_ignorable(param->getType().getNonReferenceType().getUnqualifiedType());
            if (invalid_params && !ignore)
               emitError(instance, param->getLocation(), "ignorable types cannot be preceded by non-ignorable types, this restriction will be relaxed in future versions");
            invalid_params |= ignore;
         }
         return true;
      }
   private:
      CompilerInstance& instance;
};

class ValidateConsumer : public ASTConsumer {
  CompilerInstance &Instance;
  std::set<std::string> ParsedTemplates;

public:
  ValidateConsumer(CompilerInstance &Instance)
      : Instance(Instance), visitor(Instance) {}
  virtual void HandleTranslationUnit(ASTContext& ctx) {
     visitor.TraverseDecl(ctx.getTranslationUnitDecl());
  }

private:
  ValidateVisitor visitor;
};

class ValidateAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<ValidateConsumer>(CI);
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {

    return true;
  }
  PluginASTAction::ActionType getActionType()override { return AddAfterMainAction; }
  void PrintHelp(llvm::raw_ostream& ros) {
    ros << "Help for validate plugin goes here\n";
  }

};

}

static FrontendPluginRegistry::Add<ValidateAction>
X("-validate", "only allow ignored arguments at the end of action method parameter list");
