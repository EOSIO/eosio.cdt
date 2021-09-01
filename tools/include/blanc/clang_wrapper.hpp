#pragma once
#include <clang/AST/Attr.h>
#include <clang/AST/DeclBase.h>
#include "tokenize.hpp"

namespace blanc { namespace clang_wrapper {
   class Attr {
      public:
         Attr() {}
         Attr(std::string arg): arg(arg) {}

         llvm::StringRef getName() const { return arg; }
         std::string getNameAsString() const { return arg; }

      private:
         std::string arg;
   };

   template<typename T>
   class Decl {
      public:
         Decl(T decl): decl(decl) {
            if (!decl || !decl->hasAttrs())
               return;
            if (auto* annotate = decl->template getAttr<clang::AnnotateAttr>()) {
               auto tokens = blanc::tokenize(remove_quotes(annotate->getAnnotation().str()));
               for (const auto& token: tokens) {
                  std::string str = token;
                  std::string arg;
                  auto l = str.find('(');
                  if (l != std::string::npos) {
                     auto r = str.find(')');
                     arg = str.substr(l+1, r-(l+1));
                     str = str.substr(0, l);
                  }
                  attrs.emplace(str, Attr(arg));
               }
            }
         };

         auto operator->() { return decl; }
         const auto operator->() const { return decl; }
         auto operator*() { return decl; }
         const auto operator*() const { return decl; }
         operator bool() { return decl != nullptr; }

         auto getParent() const {
            auto p = llvm::dyn_cast<clang::CXXRecordDecl>(decl->getParent());
            return Decl<decltype(p)>(p);
         }

         bool isEosioAction() const {
            return attrs.find("eosio_action") != attrs.end();
         }

         bool isEosioContract() const {
            return attrs.find("eosio_contract") != attrs.end();
         }

         bool isEosioTable() const {
            return attrs.find("eosio_table") != attrs.end();
         }

         bool isEosioIgnore() const {
            return attrs.find("eosio_ignore") != attrs.end();
         }

         bool isEosioNotify() const {
            return attrs.find("eosio_on_notify") != attrs.end();
         }

         bool hasEosioRicardian() const {
            return false;
         }

         bool isEosioWasmEntry() const {
            auto* wasm_entry = decl->template getAttr<clang::WebAssemblyExportNameAttr>();
            return !!wasm_entry;
         }

         bool isEosioReadOnly() const {
            return attrs.find("eosio_read_only") != attrs.end();
         }

         const Attr* getEosioActionAttr() const {
            return isEosioAction() ? &attrs.at("eosio_action") : nullptr;
         }

         const Attr* getEosioContractAttr() const {
            return isEosioContract() ? &attrs.at("eosio_contract") : nullptr;
         }

         const Attr* getEosioTableAttr() const {
            return isEosioTable() ? &attrs.at("eosio_table") : nullptr;
         }

         const Attr* getEosioNotifyAttr() const {
            return isEosioNotify() ? &attrs.at("eosio_on_notify") : nullptr;
         }

         const Attr* getEosioRicardianAttr() const {
            static const Attr empty{""};
            return &empty;
         }

         const auto* getEosioWasmEntry() const {
            auto* wasm_entry = decl->template getAttr<clang::WebAssemblyExportNameAttr>();
            return wasm_entry;
         }

      private:
         T decl;
         std::map<std::string,Attr> attrs;
   };

   template<typename T>
   Decl<T> wrap_decl(T decl) {
      return {decl};
   }
} }
