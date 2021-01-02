#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/Sema/ParsedAttr.h>
#include <clang/Sema/Sema.h>
#include <clang/Sema/SemaDiagnostic.h>
#include <llvm/IR/Attributes.h>

#include "macro.hpp"

using namespace clang;

EOSIO_ATTR(EosioIgnore, ignore, eosio_ignore, eosio::ignore, 0, 1, (!isa<CXXRecordDecl>(D) && !isa<RecordDecl>(D)))
EOSIO_ATTR(EosioNotify, on_notify, eosio_on_notify, eosio::on_notify, 1, 0, (!isa<CXXMethodDecl>(D)))
EOSIO_ATTR(EosioRicardian, ricardian, eosio_ricardian, eosio::ricardian, 1, 0, (!isa<CXXRecordDecl>(D) && !isa<CXXMethodDecl>(D)))
EOSIO_ATTR(EosioContract, contract, eosio_contract, eosio::contract, 0, 1, (!isa<CXXRecordDecl>(D) && !isa<CXXMethodDecl>(D)))
EOSIO_ATTR(EosioAction, action, eosio_action, eosio::action, 0, 1, (!isa<CXXRecordDecl>(D) && !isa<CXXMethodDecl>(D)))
EOSIO_ATTR(EosioTable, table, eosio_table, eosio::table, 0, 1, (!isa<CXXRecordDecl>(D)))
EOSIO_ATTR(EosioWasmAction, wasm_action, eosio_wasm_action, eosio::wasm_action, 0, 1, (!isa<FunctionDecl>(D)))
EOSIO_ATTR(EosioWasmNotify, wasm_notify, eosio_wasm_notify, eosio::wasm_notify, 0, 1, (!isa<FunctionDecl>(D)))
EOSIO_ATTR(EosioWasmAbi, wasm_abi, eosio_wasm_abi, eosio::wasm_abi, 0, 1, (!isa<FunctionDecl>(D)))

namespace eosio { namespace cdt {

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

} }

static ParsedAttrInfoRegistry::Add<eosio::cdt::EosioWasmEntryAttrInfo> EosioWasmEntry("eosio_wasm_entry", "");
static ParsedAttrInfoRegistry::Add<eosio::cdt::EosioWasmImportAttrInfo> EosioWasmImport("eosio_wasm_import", "");
