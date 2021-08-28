#pragma once
#include <eosio/gen.hpp>
#include <eosio/ppcallbacks.hpp>

#include <clang/Driver/Options.h>
#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/QualTypeNames.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Rewrite/Frontend/Rewriters.h>

#include <eosio/utils.hpp>
#include <eosio/whereami/whereami.hpp>
#include <eosio/abi.hpp>
#include <blanc/clang_wrapper.hpp>

#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <set>
#include <map>

using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

#define PICOJSON_OBJECT_ORDERED
#include <picojson/picojson.h>

using ojson = picojson::value;

namespace eosio { namespace cdt {
   class abigen : public generation_utils {
      std::set<std::string> checked_actions;
      public:
      using generation_utils::generation_utils;

      static abigen& get() {
         static abigen ag;
         return ag;
      }

      void set_abi_version(int major, int minor) {
         _abi.version_major = major;
         _abi.version_minor = minor;
      }

      void add_typedef( const clang::QualType& t ) {
         abi_typedef ret;
         ret.new_type_name = get_base_type_name( t );
         auto td = get_type_alias(t);
         if (td.empty())
            return;
         ret.type = translate_type(td[0]);
         if(!is_builtin_type(td[0]))
            add_type(td[0]);
         _abi.typedefs.insert(ret);
      }

      template<typename T>
      void add_wasm_action(const clang_wrapper::Decl<T>& decl, const std::string& handler) {
         wasm_action ret;
         ret.name = get_action_name(decl);
         ret.handler = handler;
         _abi.wasm_actions.insert(ret);
      }

      template<typename T>
      void add_wasm_notify(const clang_wrapper::Decl<T>& decl, const std::string& handler) {
         wasm_notify ret;
         auto str = get_notify_pair(decl);
         auto pos = str.find("::");
         if (pos == std::string::npos) {
            std::cerr << "Error, the argument of eosio::on_notify attribute should have separator '::'" << std::endl;
            throw;
         }
         ret.contract = str.substr(0, pos);
         ret.name = str.substr(pos+2);
         ret.handler = handler;
         _abi.wasm_notifies.insert(ret);
      }

      template<typename T>
      void add_wasm_entries(const clang_wrapper::Decl<T>& decl) {
         if (const auto* Attr = decl->template getAttr<clang::WebAssemblyExportNameAttr>()) {
            _abi.wasm_entries.insert(Attr->getExportName().str());
         }
      }

      void add_action( const clang::CXXRecordDecl* _decl ) {
         auto decl = clang_wrapper::wrap_decl(_decl);
         abi_action ret;
         auto action_name = decl.getEosioActionAttr()->getName();

         if (!checked_actions.insert(get_action_name(decl)).second)
            if (!suppress_ricardian_warnings)
               CDT_CHECK_WARN(!rcs[get_action_name(decl)].empty(), "abigen_warning", decl->getLocation(), "Action <"+get_action_name(decl)+"> does not have a ricardian contract");

         ret.ricardian_contract = rcs[get_action_name(decl)];

         if (action_name.empty()) {
            validate_name(decl->getName().str(), [&](auto s) { CDT_ERROR("abigen_error", decl->getLocation(), s); });
            ret.name = decl->getName().str();
         }
         else {
            validate_name( action_name.str(), [&](auto s) { CDT_ERROR("abigen_error", decl->getLocation(), s); });
            ret.name = action_name.str();
         }
         ret.type = decl->getName().str();
         _abi.actions.insert(ret);
      }

      void add_action( const clang::CXXMethodDecl* _decl ) {
         auto decl = clang_wrapper::wrap_decl(_decl);
         abi_action ret;

         auto action_name = decl.getEosioActionAttr()->getName();

         if (!checked_actions.insert(get_action_name(decl)).second)
            if (!suppress_ricardian_warnings)
               CDT_CHECK_WARN(!rcs[get_action_name(decl)].empty(), "abigen_warning", decl->getLocation(), "Action <"+get_action_name(decl)+"> does not have a ricardian contract");

         ret.ricardian_contract = rcs[get_action_name(decl)];

         if (action_name.empty()) {
            validate_name( decl->getNameAsString(), [&](auto s) { CDT_ERROR("abigen_error", decl->getLocation(), s); } );
            ret.name = decl->getNameAsString();
         }
         else {
            validate_name( action_name.str(), [&](auto s) { CDT_ERROR("abigen_error", decl->getLocation(), s); } );
            ret.name = action_name.str();
         }
         ret.type = decl->getNameAsString();
         _abi.actions.insert(ret);
         // TODO
         if (translate_type(decl->getReturnType()) != "void") {
            /** TODO after LLVM 9 update uncomment this code and use new error handling for pretty clang style errors
            if (decl->getReturnType() == decl->getDeclaredReturnType())
            */
            add_type(decl->getReturnType());
            _abi.action_results.insert({get_action_name(decl), translate_type(decl->getReturnType())});
            /*
            else {
               std::cout << "Error, currently in eosio.cdt v2.0 `auto` is not allowed for actions\n";
               throw abigen_ex;
            }
            */
         }
      }

      void add_tuple(const clang::QualType& type) {
         auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr());
         auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>((pt) ? pt->desugar().getTypePtr() : type.getTypePtr());
         if (!tst) {
            CDT_INTERNAL_ERROR("template specialization failure");
         }
         abi_struct tup;
         tup.name = get_type(type);
         for (int i = 0; i < tst->getNumArgs(); ++i) {
            clang::QualType ftype = std::get<clang::QualType>(get_template_argument(type, i));
            add_type(ftype);
            tup.fields.push_back( {"field_"+std::to_string(i),
                  translate_type(ftype)} );
         }
         _abi.structs.insert(tup);
      }

      void add_pair(const clang::QualType& type) {
         for (int i = 0; i < 2; ++i) {
            clang::QualType ftype = std::get<clang::QualType>(get_template_argument(type, i));
            std::string ty = translate_type(ftype);
            add_type(ftype);
         }
         abi_struct pair;
         pair.name = get_type(type);
         pair.fields.push_back( {"first", get_template_argument_as_string(type)} );
         pair.fields.push_back( {"second", get_template_argument_as_string(type, 1)} );
         add_type(std::get<clang::QualType>(get_template_argument(type)));
         add_type(std::get<clang::QualType>(get_template_argument(type, 1)));
         _abi.structs.insert(pair);
      }

      void add_map(const clang::QualType& type) {
         for (int i = 0; i < 2; ++i) {
            add_type(std::get<clang::QualType>(get_template_argument(type, i)));
         }
         abi_struct kv;
         std::string name = get_type(type);
         kv.name = name.substr(0, name.length() - 2);
         kv.fields.push_back( {"key", get_template_argument_as_string(type)} );
         kv.fields.push_back( {"value", get_template_argument_as_string(type, 1)} );
         add_type(std::get<clang::QualType>(get_template_argument(type)));
         add_type(std::get<clang::QualType>(get_template_argument(type, 1)));
         _abi.structs.insert(kv);
      }

      void add_struct( const clang::CXXRecordDecl* decl, const std::string& rname="" ) {
         if (is_kv_internal(decl) || is_kv_table(decl)) {
            return;
         }

         abi_struct ret;
         if ( decl->getNumBases() == 1 ) {
            ret.base = get_type(decl->bases_begin()->getType());
            add_type(decl->bases_begin()->getType());
         }
         for ( auto field : decl->fields() ) {
            if ( field->getName() == "transaction_extensions") {
               abi_struct ext;
               ext.name = "extension";
               ext.fields.push_back( {"type", "uint16"} );
               ext.fields.push_back( {"data", "bytes"} );
               ret.fields.push_back( {"transaction_extensions", "extension[]"});
               _abi.structs.insert(ext);
            }
            else {
               ret.fields.push_back({field->getName().str(), get_type(field->getType())});
               add_type(field->getType());
            }
         }
         if (!rname.empty())
            ret.name = rname;
         else
            ret.name = decl->getName().str();

         const auto res = _abi.structs.insert(ret);
      }

      void add_struct( const clang::CXXMethodDecl* decl ) {
         abi_struct new_struct;
         new_struct.name = decl->getNameAsString();
         for (auto param : decl->parameters() ) {
            auto param_type = param->getType().getNonReferenceType().getUnqualifiedType();
            new_struct.fields.push_back({param->getNameAsString(), get_type(param_type)});
            add_type(param_type);
         }
         _abi.structs.insert(new_struct);
      }

      std::string to_index_type( std::string t ) {
         return "i64";
      }

      void add_table( const clang::CXXRecordDecl* _decl ) {
         auto decl = clang_wrapper::wrap_decl(_decl);
         // short circuit if we happen across `eosio::kv::map` declaration
         if (is_kv_map(_decl))
            return;
         if (is_kv_table(_decl)) {
            add_kv_table(_decl);
            return;
         }

         tables.insert(_decl);
         abi_table t;
         t.type = decl->getNameAsString();
         auto table_name = decl.getEosioTableAttr()->getName();
         if (!table_name.empty()) {
            validate_name( table_name.str(), [&](auto s) { CDT_ERROR("abigen_error", decl->getLocation(), s); } );
            t.name = table_name.str();
         }
         else {
            t.name = t.type;
         }
         ctables.insert(t);
      }

      void add_table( uint64_t name, const clang::CXXRecordDecl* _decl ) {
         auto decl = clang_wrapper::wrap_decl(_decl);
         if (!(decl.isEosioTable() && abigen::is_eosio_contract(decl, get_contract_name())))
            return;

         abi_table t;
         t.type = decl->getNameAsString();
         t.name = name_to_string(name);
         _abi.tables.insert(t);
      }

      void add_kv_map(const clang::ClassTemplateSpecializationDecl* decl) {
          abi_kv_table akt;
          const auto& first_arg  = decl->getTemplateArgs()[0];
          const auto& second_arg = decl->getTemplateArgs()[1];
          const auto& third_arg  = decl->getTemplateArgs()[2];
          const auto& fourth_arg = decl->getTemplateArgs()[3];

          if (first_arg.getKind() != clang::TemplateArgument::ArgKind::Integral)
             CDT_ERROR("abigen_error", decl->getLocation(), "first template argument to KV map is not an integral const");
          if (second_arg.getKind() != clang::TemplateArgument::ArgKind::Type)
             CDT_ERROR("abigen_error", decl->getLocation(), "second template argument to KV map is not a type");
          if (third_arg.getKind() != clang::TemplateArgument::ArgKind::Type)
             CDT_ERROR("abigen_error", decl->getLocation(), "third template argument to KV map is not a type");

          akt.name = name_to_string(first_arg.getAsIntegral().getExtValue());
          akt.type = translate_type(third_arg.getAsType()); // pick the "value" type
          add_type(third_arg.getAsType());
          akt.indices.push_back({name_to_string(fourth_arg.getAsIntegral().getExtValue()),
                                  translate_type(second_arg.getAsType())}); // set the "key" as the index type
          _abi.kv_tables.insert(akt);
      }

      // TODO remove this after the next release and extend the above for handling the new table type
      void add_kv_table(const clang::CXXRecordDecl* const decl) {
         clang::CXXRecordDecl* table_type;
         std::string templ_name;

         for (const auto& base : decl->bases()) {
            if (const auto templ_base = dyn_cast<clang::ClassTemplateSpecializationDecl>(base.getType()->getAsCXXRecordDecl())) {
               const auto& templ_type = templ_base->getTemplateArgs()[0];
               table_type = templ_type.getAsType().getTypePtr()->getAsCXXRecordDecl();
               add_struct(table_type);

               const auto templ_val = templ_base->getTemplateArgs()[1].getAsIntegral().getExtValue();
               templ_name = name_to_string(templ_val);
            }
         }

         abi_kv_table t;
         t.type = table_type->getNameAsString();
         t.name = templ_name;

         const auto get_string_name_from_kv_index = [&](clang::Expr* expr) {
            std::string index_name;
            if (const auto expr_wc = dyn_cast<clang::ExprWithCleanups>(expr)) {
               if (const auto cc_expr = dyn_cast<clang::CXXConstructExpr>(expr_wc->getSubExpr())) {
                  const auto arg = cc_expr->getArg(0);
                  if (const auto cfc_expr = dyn_cast<clang::CXXFunctionalCastExpr>(arg)) {
                     if (const auto il_expr = dyn_cast<clang::InitListExpr>(cfc_expr->getSubExpr())) {
                        const auto init = il_expr->getInit(0);
                        if (const auto udl = dyn_cast<clang::UserDefinedLiteral>(init)) {
                           const auto child = udl->getRawSubExprs()[0];
                           if (const auto ice = dyn_cast<clang::ImplicitCastExpr>(child)) {
                              if (const auto dre = dyn_cast<clang::DeclRefExpr>(ice->getSubExpr())) {
                                 if (const auto fd = dyn_cast<clang::FunctionDecl>(dre->getDecl())) {
                                    const auto& templ_pack = fd->getTemplateSpecializationArgs()->get(1);
                                    for (const auto& ta : templ_pack.pack_elements()) {
                                       const auto val = (char)ta.getAsIntegral().getExtValue();
                                       index_name.push_back(val);
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
            return index_name;
         };

         for (const auto field : decl->fields()) {
            std::string index_name = get_string_name_from_kv_index(field->getInClassInitializer());
            std::string idx_type;
            const auto qt = field->getType();
            const auto index_qtype = std::get<clang::QualType>(get_template_argument(qt));
            const auto index_type = clang::TemplateArgument(index_qtype);
            if (const auto elab_type = dyn_cast<clang::ElaboratedType>(index_type.getAsType().getTypePtr())) {
               // This is the macro case
               const auto decayed_type = elab_type->getNamedType();
               if (const auto d = dyn_cast<clang::TemplateSpecializationType>(decayed_type)) {
                  const auto& decl_type = d->getArg(0);
                  if (const auto dcl_type = dyn_cast<clang::DecltypeType>(decl_type.getAsType())) {
                     idx_type = get_type_string_from_kv_index_macro_decltype(dcl_type);
                  } else {
                     idx_type = get_type(index_type.getAsType());
                  }
               } else {
                  idx_type = get_type(index_type.getAsType());
               }
            } else {
               // This is the non-macro case
               idx_type = get_type(index_type.getAsType());
            }
            t.indices.push_back({index_name, idx_type});
         }

         _abi.kv_tables.insert(t);
      }

      void add_clauses( const std::vector<std::pair<std::string, std::string>>& clauses ) {
         for ( auto clp : clauses ) {
            _abi.ricardian_clauses.push_back({std::get<0>(clp), std::get<1>(clp)});
         }
      }

      void add_contracts( const std::map<std::string, std::string>& rc ) {
         rcs = rc;
      }

      void add_variant( const clang::QualType& t ) {
         abi_variant var;
         auto pt = llvm::dyn_cast<clang::ElaboratedType>(t.getTypePtr());
         auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>((pt) ? pt->desugar().getTypePtr() : t.getTypePtr());
         var.name = get_type(t);
         for (int i=0; i < tst->getNumArgs(); ++i) {
            var.types.push_back(get_template_argument_as_string( t, i ));
            add_type(std::get<clang::QualType>(get_template_argument( t, i )));
         }
         _abi.variants.insert(var);
      }

      void add_type( const clang::QualType& t ) {
         if (evaluated.count(t.getTypePtr()))
            return;
         evaluated.insert(t.getTypePtr());
         auto type = get_ignored_type(t);
         if (!is_builtin_type(translate_type(type))) {
            if (is_aliasing(type)) {
               add_typedef(type);
            }
            else if (is_template_specialization(type, {"vector", "set", "deque", "list", "optional", "binary_extension", "ignore"})) {
               add_type(std::get<clang::QualType>(get_template_argument(type)));
            }
            else if (is_template_specialization(type, {"map"}))
               add_map(type);
            else if (is_template_specialization(type, {"pair"}))
               add_pair(type);
            else if (is_template_specialization(type, {"tuple"}))
               add_tuple(type);
            else if (is_template_specialization(type, {"variant"}))
               add_variant(type);
            else if (is_template_specialization(type, {})) {
               add_struct(type.getTypePtr()->getAsCXXRecordDecl(), get_template_name(type));
            }
            else if (type.getTypePtr()->isRecordType())
               add_struct(type.getTypePtr()->getAsCXXRecordDecl());
         }
      }

      std::string generate_json_comment() {
         std::stringstream ss;
         ss << "This file was generated with eosio-abigen.";
         ss << " DO NOT EDIT ";
         return ss.str();
      }

      ojson struct_to_json( const abi_struct& s ) {
         ojson o;
         o["name"] = s.name;
         o["base"] = s.base;
         o["fields"] = ojson::array();
         for ( auto field : s.fields ) {
            ojson f;
            f["name"] = field.name;
            f["type"] = field.type;
            o["fields"].push_back(f);
         }
         return o;
      }

      ojson variant_to_json( const abi_variant& v ) {
         ojson o;
         o["name"] = v.name;
         o["types"] = ojson::array();
         for ( auto ty : v.types ) {
            o["types"].push_back( ty );
         }
         return o;
      }

      ojson typedef_to_json( const abi_typedef& t ) {
         ojson o;
         o["new_type_name"] = t.new_type_name;
         o["type"]          = t.type;
         return o;
      }

      ojson action_to_json( const abi_action& a ) {
         ojson o;
         o["name"] = a.name;
         o["type"] = a.type;
         o["ricardian_contract"] = a.ricardian_contract;
         return o;
      }

      ojson clause_to_json( const abi_ricardian_clause_pair& clause ) {
         ojson o;
         o["id"] = clause.id;
         o["body"] = clause.body;
         return o;
      }

      ojson table_to_json( const abi_table& t ) {
         ojson o;
         o["name"] = t.name;
         o["type"] = t.type;
         o["index_type"] = "i64";
         o["key_names"] = ojson::array();
         o["key_types"] = ojson::array();
         return o;
      }

      ojson wasm_action_to_json(const wasm_action& a) {
         ojson o;
         o["name"] = a.name;
         o["handler"] = a.handler;
         return o;
      }

      ojson wasm_notify_to_json(const wasm_notify& n) {
         ojson o;
         o["contract"] = n.contract;
         o["name"] = n.name;
         o["handler"] = n.handler;
         return o;
      }

      std::pair<std::string, ojson> kv_table_to_json( const abi_kv_table& t ) {
         ojson o;
         o["type"] = t.type;
         auto indices = ojson::object();
         for (int i = 0; i < t.indices.size(); ++i) {
            auto idx = t.indices[i];
            if (i == 0) {
               ojson oj;
               oj["name"] = idx.name;
               oj["type"] = idx.type;
               o["primary_index"] = oj;
            } else {
               ojson o;
               o["type"] = idx.type;
               indices.insert_or_assign(idx.name, o);
            }
         }
         o["secondary_indices"] = indices;
         return {t.name, o};
      }

      ojson action_result_to_json( const abi_action_result& result ) {
         ojson o;
         o["name"] = result.name;
         o["result_type"] = result.type;
         return o;
      }

      bool is_empty() {
         std::set<abi_table> set_of_tables;
         for ( auto t : ctables ) {
            bool has_multi_index = false;
            for ( auto u : _abi.tables ) {
               if (t.type == u.type) {
                  has_multi_index = true;
                  break;
               }
               set_of_tables.insert(u);
            }
            if (!has_multi_index)
               set_of_tables.insert(t);
         }
         for ( auto t : _abi.tables ) {
            set_of_tables.insert(t);
         }

         return _abi.structs.empty() && _abi.typedefs.empty() && _abi.actions.empty() && set_of_tables.empty() && _abi.ricardian_clauses.empty() && _abi.variants.empty();
      }

      ojson to_json() {
         ojson o;
         o["____comment"] = generate_json_comment();
         o["version"]     = _abi.version_string();
         o["structs"]     = ojson::array();
         auto remove_suffix = [&]( std::string name ) {
            int i = name.length()-1;
            for (; i >= 0; i--)
               if ( name[i] != '[' && name[i] != ']' && name[i] != '?' && name[i] != '$' )
                  break;
            return name.substr(0,i+1);
         };

         std::set<abi_table> set_of_tables;
         for ( auto t : ctables ) {
            bool has_multi_index = false;
            for ( auto u : _abi.tables ) {
               if (t.type == u.type) {
                  has_multi_index = true;
                  break;
               }
               set_of_tables.insert(u);
            }
            if (!has_multi_index)
               set_of_tables.insert(t);
         }
         for ( auto t : _abi.tables ) {
            set_of_tables.insert(t);
         }

         std::function<std::string(const std::string&)> get_root_name;
         get_root_name = [&] (const std::string& name) {
            for (auto td : _abi.typedefs)
               if (remove_suffix(name) == td.new_type_name)
                  return get_root_name(td.type);
            return name;
         };

         auto validate_struct = [&]( abi_struct as ) {
            if ( is_builtin_type(_translate_type(as.name)) )
               return false;
            if ( is_reserved(_translate_type(as.name)) ) {
               return false;
            }
            for ( auto s : _abi.structs ) {
               for ( auto f : s.fields ) {
                  if (as.name == _translate_type(remove_suffix(f.type)))
                     return true;
               }
               for ( auto v : _abi.variants ) {
                  for ( auto vt : v.types ) {
                     if (as.name == _translate_type(remove_suffix(vt)))
                        return true;
                  }
               }
               if (get_root_name(s.base) == as.name)
                  return true;
            }
            for ( auto a : _abi.actions ) {
               if (as.name == _translate_type(a.type))
                  return true;
            }
            for( auto t : set_of_tables ) {
               if (as.name == _translate_type(t.type))
                  return true;
            }
            for ( const auto t : _abi.kv_tables ) {
               if (as.name == _translate_type(t.type))
                  return true;
            }
            for( auto td : _abi.typedefs ) {
               if (as.name == _translate_type(remove_suffix(td.type)))
                  return true;
            }
            for( auto ar : _abi.action_results ) {
               if (as.name == _translate_type(ar.type))
                  return true;
            }
            return false;
         };

         auto validate_types = [&]( abi_typedef td ) {
            for ( auto as : _abi.structs )
               if (validate_struct(as)) {
                  for ( auto f : as.fields )
                     if ( remove_suffix(f.type) == td.new_type_name )
                        return true;
                  if (as.base == td.new_type_name)
                     return true;
               }

            for ( auto v : _abi.variants ) {
               for ( auto vt : v.types ) {
                  if ( remove_suffix(vt) == td.new_type_name )
                     return true;
               }
            }
            for ( auto t : _abi.tables )
               if ( t.type == td.new_type_name )
                  return true;
            for ( auto a : _abi.actions )
               if ( a.type == td.new_type_name )
                  return true;
            for ( auto _td : _abi.typedefs )
               if ( remove_suffix(_td.type) == td.new_type_name )
                  return true;
            for ( auto ar : _abi.action_results ) {
               if ( ar.type == td.new_type_name )
                  return true;
            }
            return false;
         };

         for ( auto s : _abi.structs ) {
            const auto res = validate_struct(s);
            if (res)
               o["structs"].push_back(struct_to_json(s));
         }
         o["types"]       = ojson::array();
         for ( auto t : _abi.typedefs ) {
            if (validate_types(t))
               o["types"].push_back(typedef_to_json( t ));
         }
         o["actions"]     = ojson::array();
         for ( auto a : _abi.actions ) {
            o["actions"].push_back(action_to_json( a ));
         }
         o["tables"]     = ojson::array();
         for ( auto t : set_of_tables ) {
            o["tables"].push_back(table_to_json( t ));
         }
         o["kv_tables"]  = ojson::object();
         for ( const auto& t : _abi.kv_tables ) {
            auto kv_table = kv_table_to_json(t);
            o["kv_tables"].insert_or_assign(kv_table.first, kv_table.second);
         }
         o["ricardian_clauses"]  = ojson::array();
         for ( auto rc : _abi.ricardian_clauses ) {
            o["ricardian_clauses"].push_back(clause_to_json( rc ));
         }
         o["variants"]   = ojson::array();
         for ( auto v : _abi.variants ) {
            o["variants"].push_back(variant_to_json( v ));
         }
         o["abi_extensions"]     = ojson::array();
         if (_abi.version_major == 1 && _abi.version_minor >= 2) {
            o["action_results"]  = ojson::array();
            for ( auto ar : _abi.action_results ) {
               o["action_results"].push_back(action_result_to_json( ar ));
            }
         }
         return o;
      }

      ojson to_json_debug() {
         auto o = to_json();
         o["wasm_actions"] = ojson::array();
         for (auto& a : _abi.wasm_actions) {
            o["wasm_actions"].push_back(wasm_action_to_json(a));
         }
         o["wasm_notifies"] = ojson::array();
         for (auto& n : _abi.wasm_notifies) {
            o["wasm_notifies"].push_back(wasm_notify_to_json(n));
         }
         o["wasm_entries"] = ojson::array();
         for (auto& e : _abi.wasm_entries) {
            o["wasm_entries"].push_back(e);
         }
         return o;
      }

      private:
         abi                                   _abi;
         std::set<const clang::CXXRecordDecl*> tables;
         std::set<abi_table>                   ctables;
         std::map<std::string, std::string>    rcs;
         std::set<const clang::Type*>          evaluated;

         std::string get_type_string_from_kv_index_macro_decltype(const clang::DecltypeType* decl) {
            if (const auto ref_type = dyn_cast<clang::LValueReferenceType>(decl->desugar())) {
               const auto pt = ref_type->getPointeeType();
               if (const auto record_type = dyn_cast<clang::RecordType>(pt)) {
                  const auto gdt = record_type->getDecl();
                  if (const auto ctsd = dyn_cast<clang::ClassTemplateSpecializationDecl>(gdt)) {
                     const auto& template_args = ctsd->getTemplateArgs();
                     // Handle tuples, which have the template arguments treated as a pack
                     if (template_args.size() == 1 && template_args[0].pack_size() > 1) {
                        std::string ret = ctsd->getName().str();
                        for (const auto& pack_elem : template_args[0].getPackAsArray()) {
                           ret += "_";
                           ret += get_type(pack_elem.getAsType());
                        }
                        return ret;
                     } else {
                        // Handle non-tuple templates
                        return get_type(record_type->desugar());
                     }
                  } else {
                     // Handle non-template records
                     return get_type(record_type->desugar());
                  }
               } else {
                  // Handle non-records
                  return get_type(pt);
               }
            }
            CDT_INTERNAL_ERROR("Error generating type from kv index definition");
            return "";
         }

   };

   class eosio_abigen_visitor : public RecursiveASTVisitor<eosio_abigen_visitor>, public generation_utils {
      private:
         bool has_added_clauses = false;
         abigen& ag = abigen::get();

      public:
         explicit eosio_abigen_visitor(CompilerInstance *CI) {
            get_error_emitter().set_compiler_instance(CI);
            set_suppress_ricardian_warning(ag.suppress_ricardian_warnings);
         }

         bool shouldVisitTemplateInstantiations() const {
            return true;
         }

         virtual bool VisitCXXMethodDecl(clang::CXXMethodDecl* _decl) {
            auto decl = clang_wrapper::wrap_decl(_decl);
            if (!has_added_clauses) {
               ag.add_clauses(parse_clauses());
               ag.add_contracts(parse_contracts());
               has_added_clauses = true;
            }

            if (decl.isEosioAction() && ag.is_eosio_contract(decl, ag.get_contract_name())) {
               ag.add_struct(*decl);
               ag.add_action(*decl);
               for (auto param : decl->parameters()) {
                  ag.add_type( param->getType() );
               }
            }
            return true;
         }
         virtual bool VisitCXXRecordDecl(clang::CXXRecordDecl* _decl) {
            auto decl = clang_wrapper::wrap_decl(_decl);
            if (!has_added_clauses) {
               ag.add_clauses(parse_clauses());
               ag.add_contracts(parse_contracts());
               has_added_clauses = true;
            }
            if ((decl.isEosioAction() || decl.isEosioTable()) && ag.is_eosio_contract(decl, ag.get_contract_name())) {
               ag.add_struct(*decl);
               if (decl.isEosioAction())
                  ag.add_action(*decl);
               if (decl.isEosioTable())
                  ag.add_table(*decl);
               for (auto field : decl->fields()) {
                  ag.add_type( field->getType() );
               }
            }
            return true;
         }
         virtual bool VisitDecl(clang::Decl* decl) {
            if (const auto* d = dyn_cast<clang::ClassTemplateSpecializationDecl>(decl)) {
               if (d->getName() == "multi_index") {
                  ag.add_table(d->getTemplateArgs()[0].getAsIntegral().getExtValue(),
                        (clang::CXXRecordDecl*)((clang::RecordType*)d->getTemplateArgs()[1].getAsType().getTypePtr())->getDecl());
               } else if (d->getName() == "map") {
                  auto decl = clang_wrapper::wrap_decl(d->getSpecializedTemplate()->getTemplatedDecl());
                  if (decl.isEosioTable())
                     ag.add_kv_map(d);
               }
            }
            return true;
         }
   };

   class eosio_abigen_consumer : public ASTConsumer {
      private:
         eosio_abigen_visitor *visitor;
         std::string main_file;
         CompilerInstance* ci;

      public:
         explicit eosio_abigen_consumer(CompilerInstance *CI, std::string file)
            : visitor(new eosio_abigen_visitor(CI)), main_file(file), ci(CI) { }

         virtual void HandleTranslationUnit(ASTContext &Context) {
            auto& src_mgr = Context.getSourceManager();
            auto& f_mgr = src_mgr.getFileManager();
            auto main_fe = f_mgr.getFile(main_file);
            if (main_fe) {
               auto fid = src_mgr.getOrCreateFileID(*f_mgr.getFile(main_file), SrcMgr::CharacteristicKind::C_User);
               visitor->TraverseDecl(Context.getTranslationUnitDecl());
            }
         }
   };

   class eosio_abigen_frontend_action : public ASTFrontendAction {
      public:
         virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
            CI.getPreprocessor().addPPCallbacks(std::make_unique<eosio_ppcallbacks>(CI.getSourceManager(), file.str()));
            return std::make_unique<eosio_abigen_consumer>(&CI, file.str());
         }
   };
}} // ns eosio::cdt
