#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <clang/Sema/ParsedAttr.h>
#include <clang/Sema/Sema.h>
#include <clang/Sema/SemaDiagnostic.h>
#include <llvm/IR/Attributes.h>

using namespace clang;

#define BLANC_ATTR(NAME, GNU, CXX11, NUM, ONUM, SUBJECTS) \
   namespace blanc { \
      struct NAME ## AttrInfo : public ParsedAttrInfo { \
        unsigned _NumArgs = NUM; \
        NAME ## AttrInfo() { \
          OptArgs = ONUM; \
          static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, #GNU}, \
                                           {ParsedAttr::AS_CXX11, #CXX11}}; \
          Spellings = S; \
        } \
        bool diagAppertainsToDecl(Sema &S, const ParsedAttr &Attr, \
                                  const Decl *D) const override { \
          if (SUBJECTS) { \
            S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type_str) \
              << Attr << "declarations"; \
            return false; \
          } \
          return true; \
        } \
        AttrHandling handleDeclAttribute(Sema &S, Decl *D, \
                                         const ParsedAttr &Attr) const override { \
          StringRef Str = ""; \
          if (Attr.getNumArgs() > 0) { \
            Expr *ArgExpr = Attr.getArgAsExpr(0); \
            clang::StringLiteral *Literal = \
                dyn_cast<clang::StringLiteral>(ArgExpr->IgnoreParenCasts()); \
            if (Literal) { \
              Str = Literal->getString(); \
            } else { \
              S.Diag(ArgExpr->getExprLoc(), diag::err_attribute_argument_type) \
                  << Attr.getAttrName() << AANT_ArgumentString; \
              return AttributeNotApplied; \
            } \
          } \
          else if (spellingIndexToSemanticSpelling(Attr)) { \
             auto& SM = S.getSourceManager(); \
             auto AttrRange = SM.getExpansionRange(Attr.getRange()); \
             auto LangOpts = S.Context.getLangOpts(); \
             auto offset = Lexer::getSourceText(SM.getExpansionRange(AttrRange.getEnd()), SM, LangOpts).size(); \
             auto Begin = AttrRange.getEnd().getLocWithOffset(offset); \
             if (Lexer::getSourceText(CharSourceRange(SourceRange(Begin), true), SM, LangOpts) == "(") { \
                Str = Lexer::getSourceText(CharSourceRange(SourceRange(Begin.getLocWithOffset(1)), true), SM, LangOpts); \
             } else if (_NumArgs) { \
               S.Diag(Attr.getLoc(), diag::err_attribute_argument_type) \
                   << Attr.getAttrName() << "attribute takes one argument"; \
               return AttributeNotApplied; \
             } \
          } \
          auto arg = Str.str(); \
          if (arg[0] == '\"') arg = arg.substr(1, arg.size()-2); \
          auto* anno = D->getAttr<AnnotateAttr>(); \
          if (anno) { \
             if (Str.empty()) { \
                anno->setAnnotation(S.Context, (anno->getAnnotation().str() + ", " + #GNU)); \
             } else { \
                anno->setAnnotation(S.Context, (anno->getAnnotation().str() + ", " + #GNU + "(" + arg + ")")); \
             } \
          } else { \
             if (Str.empty()) { \
                D->addAttr(AnnotateAttr::Create(S.Context, #GNU, \
                                                Attr.getRange())); \
             } else { \
                D->addAttr(AnnotateAttr::Create(S.Context, #GNU + std::string("(") + arg + ")", \
                                                Attr.getRange())); \
             } \
          } \
          return AttributeApplied; \
        } \
      }; \
   } \
   static ParsedAttrInfoRegistry::Add<blanc::NAME ## AttrInfo> NAME(#GNU, "");


// EOSIO attributes
BLANC_ATTR(EosioIgnore, eosio_ignore, eosio::ignore, 0, 1, (!isa<CXXRecordDecl>(D) && !isa<RecordDecl>(D)))
BLANC_ATTR(EosioNotify, eosio_on_notify, eosio::on_notify, 1, 0, (!isa<CXXMethodDecl>(D)))
BLANC_ATTR(EosioRicardian, eosio_ricardian, eosio::ricardian, 1, 0, (!isa<CXXRecordDecl>(D) && !isa<CXXMethodDecl>(D)))
BLANC_ATTR(EosioContract, eosio_contract, eosio::contract, 0, 1, (!isa<CXXRecordDecl>(D) && !isa<CXXMethodDecl>(D)))
BLANC_ATTR(EosioAction, eosio_action, eosio::action, 0, 1, (!isa<CXXRecordDecl>(D) && !isa<CXXMethodDecl>(D)))
BLANC_ATTR(EosioTable, eosio_table, eosio::table, 0, 1, (!isa<CXXRecordDecl>(D)))
BLANC_ATTR(EosioWasmAction, eosio_wasm_action, eosio::wasm_action, 0, 1, (!isa<FunctionDecl>(D)))
BLANC_ATTR(EosioWasmNotify, eosio_wasm_notify, eosio::wasm_notify, 0, 1, (!isa<FunctionDecl>(D)))
BLANC_ATTR(EosioWasmAbi, eosio_wasm_abi, eosio::wasm_abi, 0, 1, (!isa<FunctionDecl>(D)))
BLANC_ATTR(EosioReadOnly, eosio_read_only, eosio::read_only, 0, 0, (!isa<FunctionDecl>(D)))

namespace blanc {
   struct EosioWasmEntryAttrInfo : public ParsedAttrInfo {
      EosioWasmEntryAttrInfo() {
         NumArgs = 0;
         static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, "eosio_wasm_entry"},
                                          {ParsedAttr::AS_CXX11, "eosio::wasm_entry"}};
         Spellings = S;
      }

      bool diagAppertainsToDecl(Sema &S, const ParsedAttr &Attr,
                                const Decl *D) const override {
         if (!isa<FunctionDecl>(D)) {
            S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type_str)
                    << Attr << "functions";
            return false;
         }
         return true;
      }

      AttrHandling handleDeclAttribute(Sema &S, Decl *D,
                                       const ParsedAttr &Attr) const override {
         StringRef Str = "";
         if (auto* named = llvm::dyn_cast<NamedDecl>(D)) {
            Str = named->getName();
         }
         D->addAttr(WebAssemblyExportNameAttr::Create(S.Context, Str,
                                                      Attr.getRange()));
         return AttributeApplied;
      }
   };


   struct EosioWasmImportAttrInfo : public ParsedAttrInfo {
      EosioWasmImportAttrInfo() {
         NumArgs = 0;
         static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, "eosio_wasm_import"},
                                          {ParsedAttr::AS_CXX11, "eosio::wasm_import"}};
         Spellings = S;
      }

      bool diagAppertainsToDecl(Sema &S, const ParsedAttr &Attr,
                                const Decl *D) const override {
         if (!isa<FunctionDecl>(D)) {
            S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type_str)
                    << Attr << "functions";
            return false;
         }
         return true;
      }

      AttrHandling handleDeclAttribute(Sema &S, Decl *D,
                                       const ParsedAttr &Attr) const override {
         StringRef Str = "";
         if (auto* named = llvm::dyn_cast<NamedDecl>(D)) {
            Str = named->getName();
         }
         D->addAttr(WebAssemblyImportNameAttr::Create(S.Context, Str,
                                                      Attr.getRange()));
         return AttributeApplied;
      }
   };
}

static ParsedAttrInfoRegistry::Add<blanc::EosioWasmEntryAttrInfo> EosioWasmEntry("eosio_wasm_entry", "");
static ParsedAttrInfoRegistry::Add<blanc::EosioWasmImportAttrInfo> EosioWasmImport("eosio_wasm_import", "");
