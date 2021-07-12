#pragma once
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/QualTypeNames.h"
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
#include <eosio/ppcallbacks.hpp>

#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <set>
#include <map>
#include <chrono>
#include <ctime>
#include <utility>
#include <regex>

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

namespace eosio { namespace cdt {
   class codegen : public generation_utils {
      public:
         Rewriter          codegen_rewriter;
         CompilerInstance* codegen_ci;
         std::string       contract_name;
         std::string       abi;
         std::set<std::string> defined_datastreams;
         std::set<std::string> datastream_uses;
         std::set<std::string> actions;
         std::set<std::string> notify_handlers;
         ASTContext *ast_context;
         std::map<std::string, CXXMethodDecl*> cxx_methods;
         std::map<std::string, CXXRecordDecl*> cxx_records;
         std::map<std::string, RecordDecl*>    records;
         llvm::sys::fs::TempFile*              tmp_file;
         llvm::ArrayRef<std::string>           sources;
         size_t                                source_index = 0;
         std::map<std::string, std::string>    tmp_files;
         bool                                  warn_action_read_only;

         using generation_utils::generation_utils;

         static codegen& get() {
            static codegen inst;
            return inst;
         }

         void set_contract_name(std::string cn) {
            contract_name = cn;
         }

         void set_abi(std::string s) {
            abi = s;
         }

         void set_warn_action_read_only(bool w) {
            warn_action_read_only = w;
         }
   };

   class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
      private:
         codegen& cg = codegen::get();
         FileID    main_fid;
         StringRef main_name;
         Rewriter  rewriter;
         CompilerInstance* ci;
         bool apply_was_found = false;

      public:
         std::vector<CXXMethodDecl*> action_decls;
         std::vector<CXXMethodDecl*> notify_decls;

         using call_map_t = std::map<FunctionDecl*, std::vector<CallExpr*>>;
         using indirect_func_map_t = std::map<NamedDecl*, FunctionDecl*>;

         std::set<CXXMethodDecl*>    read_only_actions;
         call_map_t                  func_calls;
         indirect_func_map_t         indi_func_map;

         explicit eosio_codegen_visitor(CompilerInstance *CI)
               : generation_utils(), ci(CI) {
            cg.ast_context = &(CI->getASTContext());
            cg.codegen_ci = CI;
            rewriter.setSourceMgr(CI->getASTContext().getSourceManager(), CI->getASTContext().getLangOpts());
            get_error_emitter().set_compiler_instance(CI);
         }

         void set_main_fid(FileID fid) {
            main_fid = fid;
         }

         void set_main_name(StringRef mn) {
            main_name = mn;
         }

         Rewriter& get_rewriter() {
            return rewriter;
         }

         bool is_datastream(const QualType& qt) {
            auto str_name = qt.getAsString();
            auto ds_re    = std::regex("(((class eosio::)?datastream<[a-zA-Z]+[a-zA-Z0-9]*.*>)|(DataStream)) &");
            if (std::regex_match(str_name, ds_re))
               return true;
            return false;
         }
         bool is_type_of(const QualType& qt, const std::string& t, const std::string& ns="") {
            return true;
         }

         std::string get_base_type(const QualType& qt) {
            std::istringstream ss(qt.getAsString());
            std::vector<std::string> results((std::istream_iterator<std::string>(ss)),
                                             std::istream_iterator<std::string>());
            for (auto s : results) {
               if (s == "const"  || s == "volatile" ||
                  s == "struct" || s == "class" ||
                  s == "&"      || s == "*")
                  continue;
               return s;
            }
            return "";
         }

         template <typename F>
         void create_dispatch(const std::string& attr, const std::string& func_name, F&& get_str, CXXMethodDecl* decl) {
            constexpr static uint32_t max_stack_size = 512;
            std::stringstream ss;
            codegen& cg = codegen::get();
            std::string nm = decl->getNameAsString()+"_"+decl->getParent()->getNameAsString();
            if (cg.is_eosio_contract(decl, cg.contract_name)) {
               ss << "\n\n#include <eosio/datastream.hpp>\n";
               ss << "#include <eosio/name.hpp>\n";
               ss << "extern \"C\" {\n";
               ss << "__attribute__((eosio_wasm_import))\n";
               ss << "uint32_t action_data_size();\n";
               ss << "__attribute__((eosio_wasm_import))\n";
               ss << "uint32_t read_action_data(void*, uint32_t);\n";
               const auto& return_ty = decl->getReturnType().getAsString();	
               if (return_ty != "void") {	
                  ss << "__attribute__((eosio_wasm_import))\n";	
                  ss << "void set_action_return_value(void*, size_t);\n";	
               }
               ss << "__attribute__((weak, " << attr << "(\"";
               ss << get_str(decl);
               ss << ":";
               ss << func_name << nm;
               ss << "\"))) void " << func_name << nm << "(unsigned long long r, unsigned long long c) {\n";
               ss << "size_t as = ::action_data_size();\n";
               ss << "void* buff = nullptr;\n";
               ss << "if (as > 0) {\n";
               ss << "buff = as >= " << max_stack_size << " ? malloc(as) : alloca(as);\n";
               ss << "::read_action_data(buff, as);\n";
               ss << "}\n";
               ss << "eosio::datastream<const char*> ds{(char*)buff, as};\n";
               int i=0;
               for (auto param : decl->parameters()) {
                  clang::LangOptions lang_opts;
                  lang_opts.CPlusPlus = true;
                  lang_opts.Bool = true;
                  clang::PrintingPolicy policy(lang_opts);
                  auto qt = param->getOriginalType().getNonReferenceType();
                  qt.removeLocalConst();
                  qt.removeLocalVolatile();
                  qt.removeLocalRestrict();
                  std::string tn = clang::TypeName::getFullyQualifiedName(qt, *(cg.ast_context), policy);
                  ss << tn << " arg" << i << "; ds >> arg" << i << ";\n";
                  i++;
               }
               const auto& call_action = [&]() {
                  ss << decl->getParent()->getQualifiedNameAsString() << "{eosio::name{r},eosio::name{c},ds}." << decl->getNameAsString() << "(";
                  for (int i=0; i < decl->parameters().size(); i++) {
                     ss << "arg" << i;
                     if (i < decl->parameters().size()-1)
                        ss << ", ";
                  }
                  ss << ");\n";
               };
               if (return_ty != "void") {
                  ss << "const auto& result = ";
               }
               call_action();
               if (return_ty != "void") {
                  ss << "const auto& packed_result = eosio::pack(result);\n";
                  ss << "set_action_return_value((void*)packed_result.data(), packed_result.size());\n";
               }
               ss << "}}\n";

               rewriter.InsertTextAfter(ci->getSourceManager().getLocForEndOfFile(main_fid), ss.str());
            }
         }

         void create_action_dispatch(CXXMethodDecl* decl) {
            auto func = [](CXXMethodDecl* d) { return generation_utils::get_action_name(d); };
            create_dispatch("eosio_wasm_action", "__eosio_action_", func, decl);
         }

         void create_notify_dispatch(CXXMethodDecl* decl) {
            auto func = [](CXXMethodDecl* d) { return generation_utils::get_notify_pair(d); };
            create_dispatch("eosio_wasm_notify", "__eosio_notify_", func, decl);
         }

         virtual bool VisitCXXMethodDecl(CXXMethodDecl* decl) {
            std::string name = decl->getNameAsString();
            static std::set<std::string> _action_set; //used for validations
            static std::set<std::string> _notify_set; //used for validations
            if (decl->isEosioAction()) {
               name = generation_utils::get_action_name(decl);
               validate_name(name, [&](auto s) {
                  CDT_ERROR("codegen_error", decl->getLocation(), std::string("action name (")+s+") is not a valid eosio name");
               });

               if (!_action_set.count(name))
                  _action_set.insert(name);
               else {
                  auto itr = _action_set.find(name);
                  CDT_CHECK_ERROR(*itr == name, "codegen_error", decl->getLocation(), "action declaration doesn't match previous declaration");
               }
               std::string full_action_name = decl->getNameAsString() + ((decl->getParent()) ? decl->getParent()->getNameAsString() : "");
               if (cg.actions.count(full_action_name) == 0) {
                  create_action_dispatch(decl);
               }
               cg.actions.insert(full_action_name); // insert the method action, so we don't create the dispatcher twice

               if (decl->isEosioReadOnly()) {
                  read_only_actions.insert(decl);
               }
            }
            else if (decl->isEosioNotify()) {
               name = generation_utils::get_notify_pair(decl);
               auto first = name.substr(0, name.find("::"));
               if (first != "*")
                  validate_name(first, [&](auto s) {
                     CDT_ERROR("codegen_error", decl->getLocation(), std::string("name (")+s+") is invalid");
                  });
               auto second = name.substr(name.find("::")+2);
               validate_name(second, [&](auto s) {
                  CDT_ERROR("codegen_error", decl->getLocation(), std::string("name (")+s+") is invalid");
               });

               if (!_notify_set.count(name))
                  _notify_set.insert(name);
               else {
                  auto itr = _notify_set.find(name);
                  CDT_CHECK_ERROR(*itr == name, "codegen_error", decl->getLocation(), "action declaration doesn't match previous declaration");
               }

               std::string full_notify_name = decl->getNameAsString() + ((decl->getParent()) ? decl->getParent()->getNameAsString() : "");
               if (cg.notify_handlers.count(full_notify_name) == 0) {
                  create_notify_dispatch(decl);
               }
               cg.notify_handlers.insert(full_notify_name); // insert the method action, so we don't create the dispatcher twice
            }

            return true;
         }

         void process_indi_callee(FunctionDecl* fd, CallExpr *call) {
            if (Expr *expr = call->getCallee()) {
               while (auto* ice = dyn_cast<ImplicitCastExpr>(expr)) {
                  expr = ice->getSubExpr();
               }
               if (auto* dre = dyn_cast<DeclRefExpr>(expr)) {
                  if (indi_func_map.count(dre->getFoundDecl()) != 0) {
                     func_calls[fd].push_back(call);
                  }
               } else if (auto* me = dyn_cast<MemberExpr>(expr)) {
                  if (indi_func_map.count(me->getMemberDecl()) != 0) {
                     func_calls[fd].push_back(call);
                  }
               }
            }
         }

         FunctionDecl* get_rhs_fd(Expr *rhs) const {
            while (auto* ice = dyn_cast<ImplicitCastExpr>(rhs)) {
               rhs = ice->getSubExpr();
            }
            if (auto* rhs_dre = dyn_cast<DeclRefExpr>(rhs)) {
               if (auto* fd = dyn_cast<FunctionDecl>(rhs_dre->getFoundDecl())) {
                  return fd;
               }
            }
            return nullptr;
         }

         void update_indi_func_map(NamedDecl *nd, FunctionDecl *fd) {
            if (func_calls.count(fd) != 0) {
               indi_func_map[nd] = fd;
            } else if (indi_func_map.count(nd)) {
               indi_func_map.erase(nd);
            }
         }

         void process_decl_init(NamedDecl *nd, Expr *init) {
            if (FunctionDecl *fd = get_rhs_fd(init)) {
               if (func_calls.count(fd) != 0) {
                  indi_func_map[nd] = fd;
               }
            }
         }

         void process_function(FunctionDecl* func_decl) {
            if (func_decl->isThisDeclarationADefinition() && func_decl->hasBody()) {
               Stmt *stmts = func_decl->getBody();
               for (auto it = stmts->child_begin(); it != stmts->child_end(); ++it) {
                  if (Stmt *s = *it) {
                     if (auto* ec = dyn_cast<ExprWithCleanups>(s)) {
                        s = ec->getSubExpr();
                        while (auto* ice = dyn_cast<ImplicitCastExpr>(s))
                           s = ice->getSubExpr();
                     }

                     if (auto* call = dyn_cast<CallExpr>(s)) {
                        if (FunctionDecl *fd = call->getDirectCallee()) {
                           if (func_calls.count(fd) == 0) {
                              process_function(fd);
                           }
                           if (!func_calls[fd].empty()) {
                              func_calls[func_decl].push_back(call);
                              break;
                           }
                        } else {
                           process_indi_callee(func_decl, call);
                        }
                     } else if (auto* ds = dyn_cast<DeclStmt>(s)) {
                        auto process_decl = [this]( DeclStmt *s ) {
                           for (auto it = s->decl_begin(); it != s->decl_end(); ++it) {
                              if (auto* vd = dyn_cast<VarDecl>(*it)) {
                                 if (Expr *init = vd->getInit()) {
                                    process_decl_init(vd, init);
                                 }
                              }
                           }
                        };
                        process_decl(ds);
                     } else if (auto* bo = dyn_cast<BinaryOperator>(s)) {
                        auto process_assignment = [this]( BinaryOperator *b ) {
                           Expr *lhs = nullptr, *rhs = nullptr;
                           if ((lhs = b->getLHS()) && (rhs = b->getRHS())) {
                              if (FunctionDecl *fd = get_rhs_fd(rhs)) {
                                 if (auto* lhs_dre = dyn_cast<DeclRefExpr>(lhs)) {
                                    update_indi_func_map(lhs_dre->getFoundDecl(), fd);
                                 } else if (auto* lhs_me = dyn_cast<MemberExpr>(lhs)) {
                                    update_indi_func_map(lhs_me->getMemberDecl(), fd);
                                 }
                              }
                           }
                        };
                        process_assignment(bo);
                     }
                  }
               }
            }
         }

         virtual bool VisitFunctionDecl(FunctionDecl* func_decl) {
            if (func_calls.count(func_decl) == 0 && is_write_host_func(func_decl)) {
               func_calls[func_decl] = {(CallExpr*)func_decl};
            } else {
               process_function(func_decl);
            }
            return true;
         }

         virtual bool VisitDecl(clang::Decl* decl) {
            if (auto* fd = dyn_cast<clang::FunctionDecl>(decl)) {
               if (fd->getNameInfo().getAsString() == "apply")
                  apply_was_found = true;
            } else {
               auto process_global_var = [this]( clang::Decl* d ) {
                  if (auto* vd = dyn_cast<VarDecl>(d)) {
                     if (vd->hasGlobalStorage()) {
                        if (Expr *init = vd->getInit()) {
                           process_decl_init(vd, init);
                        }
                     }
                  }
               };
               process_global_var(decl);
            }
            return true;
         }

         virtual bool VisitCXXRecordDecl(CXXRecordDecl* decl) {
            if (decl->isEosioContract()) {
               auto process_data_member = [this]( CXXRecordDecl* rd ) {
                  for (auto it = rd->decls_begin(); it != rd->decls_end(); ++it) {
                     if (auto* f = dyn_cast<FieldDecl>(*it) ) {
                        if (Expr *init = f->getInClassInitializer()) {
                           process_decl_init(f, init);
                        }
                     }
                  }
               };
               process_data_member(decl);
            }
            return true;
         }

         void process_read_only_actions() const {
            codegen& cg = codegen::get();
            for (auto const& ra : read_only_actions) {
               auto it = func_calls.find(ra);
               if (it != func_calls.end()) {
                  std::string msg = "read-only action cannot call write host function";
                  if (cg.warn_action_read_only) {
                     CDT_WARN("codegen_warning", ra->getLocation(), msg);
                  } else {
                     CDT_ERROR("codegen_error", ra->getLocation(), msg);
                  }
               }
            }
         }

      };

      class eosio_codegen_consumer : public ASTConsumer, public generation_utils {
      private:
         eosio_codegen_visitor *visitor;
         std::string main_file;
         CompilerInstance* ci;

      public:
         explicit eosio_codegen_consumer(CompilerInstance *CI, std::string file)
            : visitor(new eosio_codegen_visitor(CI)), main_file(file), ci(CI) { }


         virtual void HandleTranslationUnit(ASTContext &Context) {
            codegen& cg = codegen::get();
            auto& src_mgr = Context.getSourceManager();
            auto& f_mgr = src_mgr.getFileManager();
            auto main_fe = f_mgr.getFile(main_file);
            if (main_fe) {
               auto fid = src_mgr.getOrCreateFileID(f_mgr.getFile(main_file), SrcMgr::CharacteristicKind::C_User);
               visitor->set_main_fid(fid);
               visitor->set_main_name(main_fe->getName());
               visitor->TraverseDecl(Context.getTranslationUnitDecl());
               visitor->process_read_only_actions();

               for (auto ad : visitor->action_decls)
                  visitor->create_action_dispatch(ad);

               for (auto nd : visitor->notify_decls)
                  visitor->create_notify_dispatch(nd);

               if (cg.actions.size() < 1 && cg.notify_handlers.size() < 1) {
                  return;
               }

               int fd;
               llvm::SmallString<128> fn;
               try {
                  SmallString<64> res;
                  llvm::sys::path::system_temp_directory(true, res);

                  std::ofstream out(std::string(res.c_str())+"/"+llvm::sys::path::filename(main_fe->getName()).str());
                  for (auto inc : global_includes[main_file]) {
                     visitor->get_rewriter().ReplaceText(inc.range,
                           std::string("\"")+inc.file_name+"\"\n");
                  }
                  const auto& quoted = [](const std::string& s) {
                     std::stringstream ss;
                     for (char c : s) {
                        if (c == '"' || c == '\\')
                           ss << '\\';
                        ss << c;
                     }
                     return ss.str();
                  };

                  // generate apply stub with abi
                  std::stringstream ss;
                  ss << "\n";
                  ss << "extern \"C\" {\n";
                  ss << "__attribute__((eosio_wasm_import))\n";
                  ss << "void eosio_assert_code(uint32_t, uint64_t);";
                  ss << "\t__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi(";
                  ss << "\"" << quoted(cg.abi) << "\"";
                  ss << ")))\n";
                  ss << "\tvoid __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){";
                  ss << "eosio_assert_code(false, 1);";
                  ss << "}\n";
                  ss << "}";
                  visitor->get_rewriter().InsertTextAfter(ci->getSourceManager().getLocForEndOfFile(fid), ss.str());
                  auto& RewriteBuf = visitor->get_rewriter().getEditBuffer(fid);
                  out << std::string(RewriteBuf.begin(), RewriteBuf.end());
                  cg.tmp_files.emplace(main_file, fn.str());
                  out.close();
               } catch (...) {
                  llvm::outs() << "Failed to create temporary file\n";
               }
            }
         }

      };

      class eosio_codegen_frontend_action : public ASTFrontendAction {
      public:
         virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
            CI.getPreprocessor().addPPCallbacks(std::make_unique<eosio_ppcallbacks>(CI.getSourceManager(), file.str()));
            return std::make_unique<eosio_codegen_consumer>(&CI, file);
         }
   };

}} // ns eosio::cdt
