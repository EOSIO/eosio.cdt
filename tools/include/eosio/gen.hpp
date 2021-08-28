#pragma once

#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/Builtins.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>
#include <eosio/utils.hpp>
#include <eosio/error_emitter.hpp>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include <utility>
#include <variant>
#include <blanc/clang_wrapper.hpp>

using namespace blanc;

namespace eosio { namespace cdt {

struct simple_ricardian_tokenizer {
   simple_ricardian_tokenizer( const std::string& src ) : source(src), index(0) {}
   int eat_ws(int i) {
      if (source[i] != ' ' && source[i] != '\n' && source[i] != '\r' && source[i] != '\t')
         return 0;
      for (; i < source.size(); i++) {
         if (source[i] != ' ' && source[i] != '\n' && source[i] != '\r' && source[i] != '\t')
            break;
      }
      return i - index;
   }

   bool check(const std::string& sub) {
      int i = index;
      int j = 0;
      for (; j < sub.size(); i++, j++) {
         i += eat_ws(i);
         if (sub[j] != source[i])
            return false;
      }
      index = i;
      index += eat_ws(index);
      return true;
   }

   bool is_decl(std::string type) {
      return check("<") && check("h1") && check("class") && check("=")
            && check('\"'+type+'\"') && check(">");
   }

   std::vector<std::string> get_decl(std::string type) {
      if (is_decl(type)) {
         int before, after;
         before = after = index;
         int ws = 0;
         for (; after < source.size(); after++) {
            if (source[after] == '<')
               break;
            if (source[after] != ' ' && source[after] != '\n' && source[after] != '\r' && source[after] != '\t')
               ws = 0;
            else
               ws++;
         }
         index = after;
         if (check("<") && check("/h1") && check(">"))
            return {source.substr(before, after-before-ws)};
      }
      return {};
   }
   std::string get_body(const std::string& type) {
      int i, before;
      i = before = index;
      int ws = 0;
      for (; i < source.size(); i++) {
         index = i;
         if (is_decl(type))
            break;
         if (source[i] != ' ' && source[i] != '\n' && source[i] != '\r' && source[i] != '\t')
            ws = 0;
         else
            ws++;
      }
      index = i;
      return source.substr(before, index-before-ws);
   }

   std::vector<std::pair<std::string, std::string>> parse(const std::string& type) {
      std::vector<std::pair<std::string, std::string>> ret;
      while (index < source.size()) {
         std::vector<std::string> decl = get_decl(type);
         if (!decl.empty()) {
            std::string body = get_body(type);
            ret.push_back(std::make_pair(decl[0], body));
         }
         else
            return {};
      }
      return ret;
   }

   std::string source;
   size_t      index;
};

struct generation_utils {
   std::vector<std::string> resource_dirs;
   std::string contract_name;
   inline static std::string parsed_contract_name = ""; // obtained by parsing methods/records
   bool suppress_ricardian_warnings;

   generation_utils() : resource_dirs({"./"}) {}
   generation_utils( const std::vector<std::string>& paths ) : resource_dirs(paths) {}

   static error_emitter& get_error_emitter() {
      static error_emitter ee;
      return ee;
   }

   static inline bool is_ignorable( const clang::QualType& type ) {
      auto check = [&](const clang::Type* pt) {
        if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt))
         if (auto rt = llvm::dyn_cast<clang::RecordType>(tst->desugar())) {
            auto decl = rt->getDecl();
            return clang_wrapper::wrap_decl(decl).isEosioIgnore();
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

   static inline clang::QualType get_ignored_type( const clang::QualType& type ) {
      if ( !is_ignorable(type) )
         return type;
      auto get = [&](const clang::Type* pt) {
         if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt))
            if (auto decl = llvm::dyn_cast<clang::RecordType>(tst->desugar())) {
               auto _decl = decl->getDecl();
               return clang_wrapper::wrap_decl(_decl).isEosioIgnore() ? tst->getArg(0).getAsType() : type;
            }
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


   inline void set_contract_name( const std::string& cn ) { contract_name = cn; }
   inline std::string get_contract_name()const { return contract_name; }
   static inline std::string get_parsed_contract_name() { return parsed_contract_name; }
   inline void set_resource_dirs( const std::vector<std::string>& rd ) {
      llvm::SmallString<PATH_MAX> cwd;
      auto has_real_path = llvm::sys::fs::real_path("./", cwd, true);
      if (!has_real_path)
         resource_dirs.push_back(cwd.str().str());
      for ( auto res : rd ) {
         llvm::SmallString<PATH_MAX> rp;
         auto has_real_path = llvm::sys::fs::real_path(res, rp, true);
         if (!has_real_path)
            resource_dirs.push_back(rp.str().str());
      }
   }

   template<typename T>
   static inline bool has_eosio_ricardian( const clang_wrapper::Decl<T>& decl ) {
      return decl.hasEosioRicardian();
   }

   template<typename T>
   static inline bool get_eosio_ricardian( const clang_wrapper::Decl<T>& decl ) {
      return decl.getEosioRicardianAttr()->getName();
   }

   template<typename T>
   static inline std::string get_action_name( const clang_wrapper::Decl<T>& decl ) {
      std::string action_name = "";
      auto tmp = decl.getEosioActionAttr()->getNameAsString();
      if (!tmp.empty())
         return tmp;
      return decl->getNameAsString();
   }

   template<typename T>
   static inline std::string get_action_name( T decl ) {
      auto _decl = clang_wrapper::wrap_decl(decl);
      return get_action_name(_decl);
   }

   template<typename T>
   static inline std::string get_notify_pair( const clang_wrapper::Decl<T>& decl ) {
      std::string notify_pair = "";
      auto tmp = decl.getEosioNotifyAttr()->getNameAsString();
      return tmp;
   }

   template<typename T>
   static inline std::string get_notify_pair( T decl ) {
      auto _decl = clang_wrapper::wrap_decl(decl);
      return get_notify_pair(_decl);
   }

   inline std::string get_rc_filename() {
      return contract_name+".contracts.md";
   }
   inline std::string get_clauses_filename() {
      return contract_name+".clauses.md";
   }

   inline std::string read_file( const std::string& fname ) {
      for ( auto res : resource_dirs ) {
         if ( llvm::sys::fs::exists( res + "/" + fname ) ) {
            int fd;
            llvm::sys::fs::file_status stat;
            llvm::sys::fs::openFileForRead(res+"/"+fname, fd);
            llvm::sys::fs::status(fd, stat);
            llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> mb =
               llvm::MemoryBuffer::getOpenFile(fd, fname, stat.getSize());
            if (mb)
               return mb.get()->getBuffer().str();
         }
      }
      return {};
   }

   inline void set_suppress_ricardian_warning(bool suppress_ricardian_warnings) {
      this->suppress_ricardian_warnings = suppress_ricardian_warnings;
   }

   inline std::string get_ricardian_clauses() {
      return read_file(get_clauses_filename());
   }
   inline std::string get_ricardian_contracts() {
      return read_file(get_rc_filename());
   }

   inline std::map<std::string, std::string> parse_contracts() {
      std::string contracts = get_ricardian_contracts();
      std::map<std::string, std::string> rcs;
      simple_ricardian_tokenizer srt(contracts);
      if (contracts.empty()) {
         if (!suppress_ricardian_warnings) {
            std::cout << "Warning, empty ricardian clause file\n";
         }
         return rcs;
      }

      auto parsed = srt.parse("contract");
      for (auto cl : parsed) {
         rcs.emplace(std::get<0>(cl), std::get<1>(cl));
      }
      return rcs;
   }

   inline std::vector<std::pair<std::string, std::string>> parse_clauses() {
      std::string clauses = get_ricardian_clauses();
      std::vector<std::pair<std::string, std::string>> clause_pairs;
      simple_ricardian_tokenizer srt(clauses);
      if (clauses.empty()) {
         if (!suppress_ricardian_warnings) {
            std::cout << "Warning, empty ricardian clause file\n";
         }
         return clause_pairs;
      }

      auto parsed = srt.parse("clause");
      for (auto cl : parsed) {
         clause_pairs.emplace_back(std::get<0>(cl), std::get<1>(cl));
      }
      return clause_pairs;
   }

   template<typename T>
   static inline bool is_eosio_contract( const clang_wrapper::Decl<T>& decl, const std::string& cn ) {
      std::string name = "";
      if (auto* r = llvm::dyn_cast<clang::CXXRecordDecl>(*decl)) {
         auto pd = decl.getParent();
         if (decl.isEosioContract()) {
            auto nm = decl.getEosioContractAttr()->getNameAsString();
            name = nm.empty() ? r->getName().str() : nm;
         } else if (pd && pd.isEosioContract()) {
            auto nm = pd.getEosioContractAttr()->getNameAsString();
            name = nm.empty() ? pd->getName().str() : nm;
         }
         parsed_contract_name = name;
         return cn == parsed_contract_name;
      } else if (auto* m = llvm::dyn_cast<clang::CXXMethodDecl>(*decl)) {
         if (decl.isEosioContract()) {
            name = decl.getEosioContractAttr()->getNameAsString();
         } else if (decl.getParent().isEosioContract()) {
            name = decl.getParent().getEosioContractAttr()->getNameAsString();
         }
         if (name.empty()) {
            name = decl.getParent()->getName().str();
         }
         parsed_contract_name = name;
         return cn == parsed_contract_name;
      }
      return false;
   }

   template<typename T>
   static inline bool is_eosio_contract( T decl, const std::string& cn ) {
      auto _decl = clang_wrapper::wrap_decl(decl);
      return is_eosio_contract(_decl, cn);
   }

   inline bool is_template_specialization( const clang::QualType& type, const std::vector<std::string>& names ) {
      auto check = [&](const clang::Type* pt) {
         if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt)) {
            if (auto rt = llvm::dyn_cast<clang::RecordType>(tst->desugar())) {
               if ( names.empty() ) {
                  return true;
               } else {
                  for ( auto name : names ) {
                     if ( const auto* decl = rt->getDecl() ) {
                        if (decl->getName().str() == name) {
                           return true;
                        }
                     }
                  }
               }
            }
         }
        return false;
      };
      bool is_specialization = false;
      if (auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr())) {
         is_specialization = check(pt->desugar().getTypePtr());
      }
      else
         is_specialization = check(type.getTypePtr());

      return is_specialization;
   }

   using template_arg_t = std::variant<clang::QualType, clang::Expr*, llvm::APSInt>;

   inline template_arg_t get_template_argument( const clang::QualType& type, int index = 0 ) {
      template_arg_t ret_val;

      auto resolve = [&](const clang::Type* t) {
         auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(t);
         if (tst) {
            auto arg = tst->getArg(index);
            if ( arg.getKind() == clang::TemplateArgument::ArgKind::Type ) {
               ret_val = arg.getAsType();
               return;
            } else if ( arg.getKind() == clang::TemplateArgument::ArgKind::Integral ) {
               ret_val = arg.getAsIntegral();
               return;
            } else if ( arg.getKind() == clang::TemplateArgument::ArgKind::Expression ) {
               ret_val = arg.getAsExpr();
               return;
            }
         }
         CDT_INTERNAL_ERROR("Wrong type of template specialization");
      };

      if (auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr()))
         resolve(pt->desugar().getTypePtr());
      else
         resolve(type.getTypePtr());

      return ret_val;
   }

   inline std::string get_template_argument_as_string( const clang::QualType& type, int index = 0 ) {
      auto arg = get_template_argument( type, index );
      if (std::holds_alternative<clang::QualType>(arg)) {
         return translate_type(std::get<clang::QualType>(arg));
      }
      else if (std::holds_alternative<clang::Expr*>(arg)) {
         if (auto ce = llvm::dyn_cast<clang::CastExpr>(std::get<clang::Expr*>(arg))) {
            auto il = llvm::dyn_cast<clang::IntegerLiteral>(ce->getSubExpr());
            return std::to_string(il->getValue().getLimitedValue());
         }
      } else {
         return std::get<llvm::APSInt>(arg).toString(10);
      }
      CDT_INTERNAL_ERROR("Tried to get a non-existent template argument");
      __builtin_unreachable();
   }

   std::string get_base_type_name( const clang::QualType& type ) {
      clang::QualType newType = type;
      std::string type_str = newType.getNonReferenceType().getAsString();
      return get_base_type_name(type_str);
   }

   std::string get_base_type_name( const std::string& type_str ) {
      int i = type_str.length()-1;
      int template_nested = 0;
      for (; i > 0; i--) {
         if (type_str[i] == '>') ++template_nested;
         if (type_str[i] == '<') --template_nested;
         if (type_str[i] == ':' && template_nested == 0) {
            return type_str.substr(i+1);
         }
         if (type_str[i] == ' ' && template_nested == 0) {
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

   std::string _translate_type( const clang::QualType& type ) {
      clang::QualType newType = type;
      std::string type_str = newType.getNonReferenceType().getAsString();
      return _translate_type(get_base_type_name(type_str));
   }

   std::string _translate_type( const std::string& t ) {
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

         {"basic_string<char>", "string"},

         {"block_timestamp", "block_timestamp_type"},
         {"capi_name",    "name"},
         {"capi_public_key", "public_key"},
         {"capi_signature", "signature"},
         {"capi_checksum160", "checksum160"},
         {"capi_checksum256", "checksum256"},
         {"capi_checksum512", "checksum512"},
         {"fixed_bytes_20", "checksum160"},
         {"fixed_bytes_32", "checksum256"},
         {"fixed_bytes_64", "checksum512"}
      };

      auto ret = translation_table[t];

      if (ret == "")
         return t;
      return ret;
   }

   inline std::string replace_in_name( std::string name ) {
      std::string ret = name;
      std::replace(ret.begin(), ret.end(), '<', '_');
      std::replace(ret.begin(), ret.end(), '>', '_');
      std::replace(ret.begin(), ret.end(), ',', '_');
      std::replace(ret.begin(), ret.end(), ' ', '_');
      std::replace(ret.begin(), ret.end(), ':', '_');
      return ret;
   }

   inline std::string get_template_name( const clang::QualType& type ) {
      auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr());
      auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt ? pt->desugar().getTypePtr() : type.getTypePtr());
      std::string ret = tst->getTemplateName().getAsTemplateDecl()->getName().str()+"_";
      for (int i=0; i < tst->getNumArgs(); ++i) {
         ret += get_template_argument_as_string(type, i);
         if (i < tst->getNumArgs() - 1) {
            ret += "_";
         }
      }
      return _translate_type(replace_in_name(ret));
   }

   inline std::string translate_type( const clang::QualType& type ) {
      if ( is_template_specialization( type, {"ignore"} ) )
         return get_template_argument_as_string( type );
      else if ( is_template_specialization( type, {"binary_extension"} ) ) {
         auto t = get_template_argument_as_string( type );
         return t+"$";
      }
      else if ( is_template_specialization( type, {"vector", "set", "deque", "list"} ) ) {
         auto t = get_template_argument_as_string( type );
         if ( t=="int8" || t=="uint8" ) {
            return "bytes";
         } else {
            return t+"[]";
         }
      }
      else if (is_tuple(type)) {
         return translate_type(get_nested_type(type));
      }
      else if ( is_template_specialization( type, {"optional"} ) )
         return get_template_argument_as_string( type )+"?";
      else if ( is_template_specialization( type, {"map"} )) {
         auto t0 = get_template_argument_as_string( type );
         auto t1 = get_template_argument_as_string( type, 1);
         return replace_in_name("pair_" + t0 + "_" + t1 + "[]");
      }
      else if ( is_template_specialization( type, {"pair"} )) {
         auto t0 = get_template_argument_as_string( type );
         auto t1 = get_template_argument_as_string( type, 1);
         return replace_in_name("pair_" + t0 + "_" + t1);
      }
      else if ( is_template_specialization( type, {"tuple"} )) {
         auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr());
         auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>( pt ? pt->desugar().getTypePtr() : type.getTypePtr() );
         std::string ret = "tuple_";
         for (int i=0; i < tst->getNumArgs(); ++i) {
            ret += get_template_argument_as_string( type, i );
            if ( i < tst->getNumArgs()-1 )
               ret += "_";
         }
         return replace_in_name(ret);
      }
      else if ( is_template_specialization( type, {} )) {
         auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr());
         auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt ? pt->desugar().getTypePtr() : type.getTypePtr() );
         std::string ret = tst->getTemplateName().getAsTemplateDecl()->getName().str()+"_";
         for (int i=0; i < tst->getNumArgs(); ++i) {
            ret += get_template_argument_as_string(type,i);
            if ( i < tst->getNumArgs()-1 )
               ret += "_";
         }
         return _translate_type(replace_in_name(ret));
      }
      return _translate_type( type );
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
         "name",
         "checksum160",
         "checksum256",
         "checksum512",
         "public_key",
         "signature",
         "public_key",
         "signature",
         "symbol",
         "symbol_code",
         "asset",
         "extended_asset"
      };
      return builtins.count(_translate_type(t)) >= 1;
   }

   inline bool is_reserved( const std::string& t ) {
      return t.find("__") != std::string::npos;
   }

   inline bool is_builtin_type( const clang::QualType& t ) {
      std::string nt = translate_type(t);
      return is_builtin_type(nt);
   }

   inline bool is_cxx_record( const clang::QualType& t ) {
      return t.getTypePtr()->isRecordType();
   }

   inline std::string get_type( const clang::QualType& t ) {
      return translate_type(get_ignored_type(t));
   }

   inline std::string get_type_alias_string( const clang::QualType& t ) {
      if (auto dt = llvm::dyn_cast<clang::TypedefType>(t.getTypePtr()))
         return get_type(dt->desugar());
      else if (auto dt = llvm::dyn_cast<clang::ElaboratedType>(t.getTypePtr()))
         return get_type_alias_string(dt->desugar());
      return get_type(t);
   }

   inline std::vector<clang::QualType> get_type_alias( const clang::QualType& t ) {
      if (auto dt = llvm::dyn_cast<clang::TypedefType>(t.getTypePtr()))
         return {dt->desugar()};
      else if (auto dt = llvm::dyn_cast<clang::ElaboratedType>(t.getTypePtr()))
         return get_type_alias(dt->desugar());
      return {};
   }

   inline bool is_aliasing( const clang::QualType& t ) {
      if (is_builtin_type(t))
         return false;
      if (get_base_type_name(t).find("<") != std::string::npos) return false;
      return get_base_type_name(t).compare(get_type_alias_string(t)) != 0;
   }

   inline bool is_tuple(const clang::QualType& t) {
      constexpr std::string_view test_str = "tuple<";
      return t.getAsString().substr(0, test_str.size()) == test_str;
   }

   inline clang::QualType get_nested_type(const clang::QualType& t) {
      if (auto pt = llvm::dyn_cast<clang::ElaboratedType>(t.getTypePtr())) {
         if (auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt->desugar().getTypePtr())) {
            if (auto rt = llvm::dyn_cast<clang::ElaboratedType>(tst->desugar())) {
               return tst->desugar();
            }
         }
      }
      CDT_INTERNAL_ERROR("Tried to get a nested template type of a template not containing one");
      __builtin_unreachable();
   }

   inline bool is_kv_map(const clang::CXXRecordDecl* decl) {
      return decl->getQualifiedNameAsString().find("eosio::kv::map<") != std::string::npos;
   }

   // TODO replace this body after this release to reflect the new table type
   inline bool is_kv_table(const clang::CXXRecordDecl* decl) {
      for (const auto& base : decl->bases()) {
         auto type = base.getType();
         if (type.getAsString().find("eosio::kv::table<") != std::string::npos) {
            return true;
         }
      }

      return false;
   }

   inline bool is_kv_internal(const clang::CXXRecordDecl* decl) {
      const std::set<std::string> internal_types {
         "table",
         "table_base",
         "index",
         "index_base"
      };

      const auto fqn = decl->getQualifiedNameAsString();

      const auto in_kv_namespace = fqn.find("eosio::kv") != std::string::npos;
      const bool is_internal = internal_types.count(decl->getNameAsString());

      return in_kv_namespace && is_internal;
   }

   inline bool is_write_host_func( const clang::FunctionDecl *func_decl ) {
      static const std::set<std::string> write_host_funcs =
      {
         "set_resource_limits",
         "set_wasm_parameters_packed",
         "set_resource_limit",
         "set_proposed_producers",
         "set_proposed_producers_ex",
         "set_blockchain_parameters_packed",
         "set_parameters_packed",
         "set_kv_parameters_packed",
         "set_privileged",
         "db_store_i64",
         "db_update_i64",
         "db_remove_i64",
         "db_idx64_store",
         "db_idx64_update",
         "db_idx64_remove",
         "db_idx128_store",
         "db_idx128_update",
         "db_idx128_remove",
         "db_idx256_store",
         "db_idx256_update",
         "db_idx256_remove",
         "db_idx_double_store",
         "db_idx_double_update",
         "db_idx_double_remove",
         "db_idx_long_double_store",
         "db_idx_long_double_update",
         "db_idx_long_double_remove",
         "kv_erase",
         "kv_set",
         "send_deferred",
         "send_inline",
         "send_context_free_inline"
      };

      return write_host_funcs.count(func_decl->getNameInfo().getAsString()) >= 1;
   }

   inline bool is_deferred_transaction_func( const std::string& t ) {
      static const std::set<std::string> deferred_transaction_funcs =
      {
         "send_deferred",
      };
      return deferred_transaction_funcs.count(t) >= 1;
   }

   inline bool is_inline_action_func( const std::string& t ) {
      static const std::set<std::string> inline_action_funcs =
      {
         "send_inline",
         "send_context_free_inline"
      };
      return inline_action_funcs.count(t) >= 1;
   }
};
}} // ns eosio::cdt
