#pragma once
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

#include <jsoncons/json.hpp>

using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;
using jsoncons::json;
using jsoncons::ojson;

namespace eosio { namespace cdt {
   struct abigen_exception : public std::exception {
      virtual const char* what() const throw() {
         return "eosio.abigen fatal error";
      }
   };
   extern abigen_exception abigen_ex;

   class abigen : public generation_utils {
      public:

      abigen() : generation_utils([&](){throw abigen_ex;}) {
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

      void add_action( const clang::CXXRecordDecl* decl ) {
         abi_action ret;
         auto action_name = decl->getEosioActionAttr()->getName();

         if (rcs[get_action_name(decl)].empty())
            std::cout << "Warning, action <"+get_action_name(decl)+"> does not have a ricardian contract\n";

         ret.ricardian_contract = rcs[get_action_name(decl)];

         if (action_name.empty()) {
            try {
               validate_name( decl->getName().str(), error_handler );
            } catch (...) {
               std::cout << "Error, name <" <<decl->getName().str() << "> is an invalid EOSIO name.\n";
               throw;
            }
            ret.name = decl->getName().str();
         }
         else {
            try {
               validate_name( action_name.str(), error_handler );
            } catch (...) {
               std::cout << "Error, name <" << action_name.str() << "> is an invalid EOSIO name.\n";
               throw;
            }
            ret.name = action_name.str();
         }
         ret.type = decl->getName().str();
         _abi.actions.insert(ret);
      }

      void add_action( const clang::CXXMethodDecl* decl ) {
         abi_action ret;

         auto action_name = decl->getEosioActionAttr()->getName();

         if (rcs[get_action_name(decl)].empty())
            std::cout << "Warning, action <"+get_action_name(decl)+"> does not have a ricardian contract\n";

         ret.ricardian_contract = rcs[get_action_name(decl)];

         if (action_name.empty()) {
            try {
               validate_name( decl->getNameAsString(), error_handler );
            } catch (...) {
               std::cout << "Error, name <" <<decl->getNameAsString() << "> is an invalid EOSIO name.\n";
            }
            ret.name = decl->getNameAsString();
         }
         else {
            try {
               validate_name( action_name.str(), error_handler );
            } catch (...) {
               std::cout << "Error, name <" << action_name.str() << "> is an invalid EOSIO name.\n";
            }
            ret.name = action_name.str();
         }
         ret.type = decl->getNameAsString();
         _abi.actions.insert(ret);
      }

      void add_tuple(const clang::QualType& type) {
         auto pt = llvm::dyn_cast<clang::ElaboratedType>(type.getTypePtr());
         auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>((pt) ? pt->desugar().getTypePtr() : type.getTypePtr());
         if (!tst)
            throw abigen_ex;
         abi_struct tup;
         tup.name = get_type(type);
         for (int i = 0; i < tst->getNumArgs(); ++i) {
            clang::QualType ftype = get_template_argument(type, i).getAsType();
            add_type(ftype);
            tup.fields.push_back( {"field_"+std::to_string(i),
                  translate_type(ftype)} );
         }
         _abi.structs.insert(tup);
      }

      void add_pair(const clang::QualType& type) {
         for (int i = 0; i < 2; ++i) {
            clang::QualType ftype = get_template_argument(type, i).getAsType();
            std::string ty = translate_type(ftype);
            add_type(ftype);
         }
         abi_struct pair;
         pair.name = get_type(type);
         pair.fields.push_back( {"first", translate_type(get_template_argument(type).getAsType())} );
         pair.fields.push_back( {"second", translate_type(get_template_argument(type, 1).getAsType())} );   
         add_type(get_template_argument(type).getAsType());
         add_type(get_template_argument(type, 1).getAsType());
         _abi.structs.insert(pair);
      } 

      void add_map(const clang::QualType& type) {
         for (int i = 0; i < 2; ++i) {
            clang::QualType ftype = get_template_argument(type, i).getAsType();
            std::string ty = translate_type(ftype);
            add_type(ftype);
         }
         abi_struct kv;
         std::string name = get_type(type);
         kv.name = name.substr(0, name.length() - 2);
         kv.fields.push_back( {"key", translate_type(get_template_argument(type).getAsType())} );
         kv.fields.push_back( {"value", translate_type(get_template_argument(type, 1).getAsType())} );   
         add_type(get_template_argument(type).getAsType());
         add_type(get_template_argument(type, 1).getAsType());
         _abi.structs.insert(kv);
      }

      void add_struct( const clang::CXXRecordDecl* decl, const std::string& rname="" ) {
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
         _abi.structs.insert(ret);
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

      void add_table( const clang::CXXRecordDecl* decl ) {
         tables.insert(decl);
         abi_table t;
         t.type = decl->getNameAsString();
         auto table_name = decl->getEosioTableAttr()->getName();
         if (!table_name.empty()) {
            try {
               validate_name( table_name.str(), error_handler );
            } catch (...) {
            }
            t.name = table_name.str();
         }
         else {
            t.name = t.type;
         }
         ctables.insert(t);
      }

      void add_table( uint64_t name, const clang::CXXRecordDecl* decl ) {
         if (!(decl->isEosioTable() && abigen::is_eosio_contract(decl, get_contract_name())))
            return;
         abi_table t;
         t.type = decl->getNameAsString();
         t.name = name_to_string(name);
         _abi.tables.insert(t);
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
         auto tst = llvm::dyn_cast<clang::TemplateSpecializationType>(pt->desugar().getTypePtr());
         var.name = get_type(t);
         for (int i=0; i < tst->getNumArgs(); ++i) {
            var.types.push_back(translate_type(get_template_argument( t, i ).getAsType()));
            add_type(get_template_argument( t, i ).getAsType());
         }
         _abi.variants.insert(var); 
      }

      void add_type( const clang::QualType& t ) {
         if (evaluated.count(t.getTypePtr()))
            return;
         evaluated.insert(t.getTypePtr());
         auto type = get_ignored_type(t);
         if (!is_builtin_type(translate_type(type))) {
            if (is_aliasing(type))
               add_typedef(type);
            else if (is_template_specialization(type, {"vector", "set", "deque", "list", "optional", "binary_extension", "ignore"})) {
               add_type(get_template_argument(type).getAsType());
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
         o["version"]     = _abi.version;
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
            for( auto td : _abi.typedefs ) {
               if (as.name == _translate_type(remove_suffix(td.type)))
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
            return false;
         };

         for ( auto s : _abi.structs ) {
            if (validate_struct(s))
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
         o["ricardian_clauses"]  = ojson::array();
         for ( auto rc : _abi.ricardian_clauses ) {
            o["ricardian_clauses"].push_back(clause_to_json( rc ));
         }
         o["variants"]   = ojson::array();
         for ( auto v : _abi.variants ) {
            o["variants"].push_back(variant_to_json( v ));
         }
         o["abi_extensions"]     = ojson::array();
         return o;
      }

      private: 
         abi                                   _abi;
         std::set<const clang::CXXRecordDecl*> tables;
         std::set<abi_table>                   ctables;
         std::map<std::string, std::string>    rcs;
         std::set<const clang::Type*>          evaluated;
   };
}} // ns eosio::cdt
