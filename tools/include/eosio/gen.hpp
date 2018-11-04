#pragma once

#include "clang/AST/DeclCXX.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/Builtins.h"
#include "llvm/Support/raw_ostream.h"
#include <functional>
#include <vector>
#include <string>
#include "eosio/utils.hpp"

namespace eosio { namespace cdt {

struct generation_utils {
   std::function<void()> error_handler;
  
   generation_utils( std::function<void()> err ) : error_handler(err) {}
   
   static inline bool is_tuple( const clang::QualType& type ) {
   }

   static inline bool is_ignorable( const clang::QualType& type ) {
      auto check = [&](const clang::Type* pt) {
        if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt))
         if (auto rt = llvm::dyn_cast<clang::RecordType>(tst->desugar()))
            return rt->getDecl()->isEosioIgnore();

         return false;
      };

      bool is_ignore = false;
      if ( auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr()) )
         is_ignore = check(pt->desugar().getTypePtr());
      else
         is_ignore = check(type.getTypePtr());
      return is_ignore;
   }
   
   static inline clang::QualType get_ignored_type( const clang::QualType& type ) {
      auto get = [&](const clang::Type* pt) {
         if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt))
            if (auto decl = llvm::dyn_cast<clang::RecordType>(tst->desugar()))
               return decl->getDecl()->isEosioIgnore() ? tst->getArg(0).getAsType() : type;
         return type;
      };
      
      const clang::Type* t;
      if ( auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr()) ) {
         t = pt->desugar().getTypePtr();
      }
      else
         t = type.getTypePtr();
      return get(t);
   }

   static inline bool is_eosio_contract( const clang::CXXMethodDecl* decl, const std::string& cn ) {
      std::string name = "";
      if (decl->isEosioContract())
         name = decl->getEosioContractAttr()->getName();
      else if (decl->getParent()->isEosioContract())
         name = decl->getParent()->getEosioContractAttr()->getName();
      if (name.empty()) {
         name = decl->getParent()->getName().str();
      }
      return name == cn; 
   }

   static inline bool is_eosio_contract( const clang::CXXRecordDecl* decl, const std::string& cn ) {
      std::string name = "";
      auto pd = llvm::dyn_cast<clang::CXXRecordDecl>(decl->getParent());
      if (decl->isEosioContract()) {
         auto nm = decl->getEosioContractAttr()->getName().str();
         name = nm.empty() ? decl->getName().str() : nm;
      }
      else if (pd && pd->isEosioContract()) {
         auto nm = pd->getEosioContractAttr()->getName().str();
         name = nm.empty() ? pd->getName().str() : nm;
      }
      return cn == name;
   }


   inline bool is_template_specialization( const clang::QualType& type, const std::vector<std::string>& names ) {
      auto check = [&](const clang::Type* pt) {
         if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt)) {
            if (auto rt = llvm::dyn_cast<clang::RecordType>(tst->desugar())) {
               if ( names.empty() ) {
                  return true;
               } else {
                  for ( auto name : names )
                     if ( rt->getDecl()->getName().str() == name )
                        return true;
               }
            }
         }
        return false;
      };
      bool is_specialization = false;
      if (auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr()))
         is_specialization = check(pt->desugar().getTypePtr());
      else
         is_specialization = check(type.getTypePtr());

      return is_specialization;
   }
   inline clang::QualType get_template_argument( const clang::QualType& type ) {
      auto ret = [&](const clang::Type* t) {
         if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(t))
            return tst->getArg(0).getAsType();
         std::cout << "Internal error, wrong type of template specialization\n";
         error_handler();
      };
      if (auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr()))
         return ret(pt->desugar().getTypePtr());
      return ret(type.getTypePtr());
   }

   std::string get_base_type_name( const clang::QualType& type ) {
      clang::QualType newType = type;
      std::string type_str = newType.getNonReferenceType().getAsString();
      int i = type_str.length()-1;
      for (; i > 0; i--) {
         if (type_str[i] == ':') {
            return type_str.substr(i+1); 
         }
         if (type_str[i] == ' ') {
            static const std::string valid_prefixs[] = {"long", "signed", "unsigned" };
            bool valid = false;
            for (auto &s : valid_prefixs) {
               if (i >= s.length() && type_str.substr(i - s.length(), s.length()) == s &&
                   (i - s.length() == 0 || type_str[i - s.length() - 1] == ' ')) {
                  valid = true; 
                  i -= s.length();
                  break;
               }
            }
            if (valid) continue;
            return type_str.substr(i+1); 
         }
      }
      return type_str;
   }

   std::string _translate_type( const clang::QualType& t ) {
      static std::map<std::string, std::string> translation_table =
      {
         {"unsigned __int128", "uint128"},
         {"__int128", "int128"},
         {"uint128_t", "uint128"},
         {"int128_t", "int128"},

         {"unsigned long long", "uint64"},
         {"long long", "int64"},
         {"uint64_t", "uint64"},
         {"int64_t", "int64"},

         {"unsigned long", "uint32"},
         {"long", "int32"},
         {"unsigned int", "uint32"},
         {"int", "int32"},
         {"uint32_t", "uint32"},
         {"int32_t", "int32"},

         {"unsigned short", "uint16"},
         {"short", "int16"},
         {"uint16_t", "uint16"},
         {"int16_t", "int16"},

         {"unsigned char", "uint8"},
         {"signed char", "int8"},
         {"char", "int8"},
         {"uint8_t", "uint8"},
         {"int8_t", "int8"},
         {"_Bool", "bool"},

         {"float",  "float32"},
         {"double", "float64"},
         {"long double", "float128"},
         
         {"unsigned_int", "varuint32"},
         {"signed_int",   "varint32"}, 
         
         {"block_timestamp", "block_timestamp_type"},
         {"capi_name",    "name"},
         {"capi_public_key", "public_key"},
         {"capi_signature", "signature"},
         {"capi_checksum160", "checksum160"},
         {"capi_checksum256", "checksum256"},
         {"capi_checksum512", "checksum512"}
      }; 

      std::string base_name = get_base_type_name(t);
      auto ret = translation_table[get_base_type_name(t)];
      if (ret == "")
         return base_name;
      return ret;
   }

   inline std::string translate_type( const clang::QualType& type ) {
      if ( is_template_specialization( type, {"vector"} ) ) {
         auto t =_translate_type(get_template_argument( type ));
         return t=="int8" ? "bytes" : t+"[]";
      }
      else if ( is_template_specialization( type, {"optional"} ) )
         return _translate_type(get_template_argument( type ))+"?";
      return _translate_type( type );
   }


   inline bool is_name_type( const std::string& t ) {
      static const std::set<std::string> name_types = { "name", 
                                                        "account_name",
                                                        "permission_name",
                                                        "table_name",
                                                        "scope_name",
                                                        "action_name" };
      return name_types.count(t) >= 1;
   }
   
   inline bool is_builtin_type( const std::string& t ) {
      static const std::set<std::string> builtins =
      {
         "bool",
         "int8",
         "uint8",
         "int16",
         "uint16",
         "int32",
         "uint32",
         "int64",
         "uint64",
         "int128",
         "uint128",
         "varint32",
         "varuint32",
         "float32",
         "float64",
         "float128",
         "time_point",
         "time_point_sec",
         "name",
         "bytes",
         "string",
         "block_timestamp_type",
         "capi_name",
         "capi_checksum160",
         "capi_checksum256",
         "capi_checksum512",
         "capi_public_key",
         "capi_signature",
         "checksum160",
         "checksum256",
         "checksum512",
         "public_key",
         "signature",
         "symbol",
         "symbol_code",
         "asset",
         "extended_asset"
      };
      return builtins.count(t) >= 1;
   }
   
   inline bool is_builtin_type( const clang::QualType& t ) {
      std::string nt = translate_type(t);
      return is_builtin_type(nt) || is_name_type(nt) || is_template_specialization(t, {"vector", "optional"});
   } 

   inline bool is_cxx_record( const clang::QualType& t ) {
      return t.getTypePtr()->isRecordType();
   }


   inline std::string get_type( const clang::QualType& t ) {
      return translate_type(t); 
   }

   inline std::string get_type_alias( const clang::QualType& t ) {
      if (is_name_type(get_base_type_name(t)))
         return "name";
      return get_type(clang::QualType(t).getCanonicalType());
   }

   inline bool is_aliasing( const clang::QualType& t ) {
      if (is_name_type(get_base_type_name(t)))
         return true;
      return get_base_type_name(t).compare(get_type_alias(t)) != 0;
   }
};
}} // ns eosio::cdt
