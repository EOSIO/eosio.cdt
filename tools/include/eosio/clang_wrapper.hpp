#pragma once

#include <algorithm>
#include <clang/AST/Attr.h>
#include <clang/AST/DeclBase.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;

namespace eosio { namespace cdt { namespace clang_wrapper {

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
               auto annotation = annotate->getAnnotation().str();
               boost::replace_all(annotation, "\"", "");
               tokenizer tokens{annotation};
               for (const auto& token: tokens) {
                  std::string str = token;
                  std::string arg;
                  auto l = str.find('(');
                  if (l != std::string::npos) {
                     auto r = str.find(')');
                     arg = str.substr(l+1, r-(l+1));
                     boost::trim(arg);
                     str = str.substr(0, l);
                  }
                  boost::trim(str);
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
            return attrs.find("action") != attrs.end();
         }

         bool isEosioContract() const {
            return attrs.find("contract") != attrs.end();
         }

         bool isEosioTable() const {
            return attrs.find("table") != attrs.end();
         }

         bool isEosioIgnore() const {
            return attrs.find("ignore") != attrs.end();
         }

         bool isEosioNotify() const {
            return attrs.find("on_notify") != attrs.end();
         }

         bool hasEosioRicardian() const {
            return false;
         }

         bool isEosioWasmEntry() const {
            auto* wasm_entry = decl->template getAttr<clang::WebAssemblyExportNameAttr>();
            return !!wasm_entry;
         }

         const Attr* getEosioActionAttr() const {
            return isEosioAction() ? &attrs.at("action") : nullptr;
         }

         const Attr* getEosioContractAttr() const {
            return isEosioContract() ? &attrs.at("contract") : nullptr;
         }

         const Attr* getEosioTableAttr() const {
            return isEosioTable() ? &attrs.at("table") : nullptr;
         }

         const Attr* getEosioNotifyAttr() const {
            return isEosioNotify() ? &attrs.at("on_notify") : nullptr;
         }

         const Attr* getEosioRicardianAttr() const {
            static const Attr empty{""};
            return &empty;
         }

      private:
         T decl;
         std::map<std::string,Attr> attrs;
   };

   template<typename T>
   Decl<T> make_decl(T decl) {
      return {decl};
   }

} } }
