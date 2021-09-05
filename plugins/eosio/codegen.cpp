#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/QualTypeNames.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <fstream>
#include <blanc/tokenize.hpp>
#include <blanc/utils.hpp>
#include <eosio/gen.hpp>
#include <eosio/abigen.hpp>

using namespace clang;
using namespace eosio::cdt;

std::string output;
std::set<std::string> actions;
std::set<std::string> notify_handlers;

class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
private:
   FileID    main_fid;
   StringRef main_name;
   Rewriter  rewriter;
   CompilerInstance* ci;
   bool      warn_action_read_only = false;

public:
   using call_map_t = std::map<FunctionDecl*, std::vector<CallExpr*>>;
   using indirect_func_map_t = std::map<NamedDecl*, FunctionDecl*>;

   std::set<CXXMethodDecl*>    read_only_actions;
   call_map_t                  func_calls;
   indirect_func_map_t         indi_func_map;

   explicit eosio_codegen_visitor(CompilerInstance* CI): ci(CI) {
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

   template <typename F, typename D>
   void create_dispatch(const std::string& attr, const std::string& func_name, F&& get_str, D decl) {
      constexpr static uint32_t max_stack_size = 512;
      std::stringstream ss;
      std::string nm = decl->getNameAsString()+"_"+decl->getParent()->getNameAsString();
      if (is_eosio_contract(decl, contract_name)) {
         ss << "\n\n#include <eosio/datastream.hpp>\n";
         ss << "#include <eosio/name.hpp>\n";
         ss << "extern \"C\" {\n";
         ss << "  uint32_t action_data_size();\n";
         ss << "  uint32_t read_action_data(void*, uint32_t);\n";
         const auto& return_ty = decl->getReturnType().getAsString();
         if (return_ty != "void")
            ss << "  void set_action_return_value(void*, size_t);\n";
         ss << "  __attribute__((weak))\n";
         ss << "  void " << func_name << nm << "(unsigned long long r, unsigned long long c) {\n";
         ss << "    size_t as = ::action_data_size();\n";
         ss << "    void* buff = nullptr;\n";
         ss << "    if (as > 0) {\n";
         ss << "      buff = as >= " << max_stack_size << " ? malloc(as) : alloca(as);\n";
         ss << "      ::read_action_data(buff, as);\n";
         ss << "    }\n";
         ss << "    eosio::datastream<const char*> ds{(char*)buff, as};\n";
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
            std::string tn = clang::TypeName::getFullyQualifiedName(qt, ci->getASTContext(), policy);
            ss << "    " << tn << " arg" << i << "; ds >> arg" << i << ";\n";
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
         ss << "    ";
         if (return_ty != "void") {
            ss << "const auto& result = ";
         }
         call_action();
         if (return_ty != "void") {
            ss << "    const auto& packed_result = eosio::pack(result);\n";
            ss << "    set_action_return_value((void*)packed_result.data(), packed_result.size());\n";
         }
         ss << "  }\n";
         ss << "}\n";

         rewriter.InsertTextAfter(ci->getSourceManager().getLocForEndOfFile(main_fid), ss.str());
      }
   }

   void create_action_dispatch(clang_wrapper::Decl<CXXMethodDecl*> decl) {
      auto func = [](clang_wrapper::Decl<CXXMethodDecl*> d) { return generation_utils::get_action_name(d); };
      create_dispatch("eosio_wasm_action", "__eosio_action_", func, decl);
   }

   void create_action_dispatch(CXXMethodDecl* decl) {
      auto func = [](CXXMethodDecl* d) { return generation_utils::get_action_name(d); };
      create_dispatch("eosio_wasm_action", "__eosio_action_", func, decl);
   }

   void create_notify_dispatch(clang_wrapper::Decl<CXXMethodDecl*> decl) {
      auto func = [](clang_wrapper::Decl<CXXMethodDecl*> d) { return generation_utils::get_notify_pair(d); };
      create_dispatch("eosio_wasm_notify", "__eosio_notify_", func, decl);
   }

   void create_notify_dispatch(CXXMethodDecl* decl) {
      auto func = [](CXXMethodDecl* d) { return generation_utils::get_notify_pair(d); };
      create_dispatch("eosio_wasm_notify", "__eosio_notify_", func, decl);
   }

   virtual bool VisitCXXMethodDecl(CXXMethodDecl* _decl) {
      auto decl = clang_wrapper::wrap_decl(_decl);
      std::string name = decl->getNameAsString();
      static std::set<std::string> _action_set; //used for validations
      static std::set<std::string> _notify_set; //used for validations
      if (decl.isEosioAction() && is_eosio_contract(decl, contract_name)) {
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
         std::string full_action_name = "__eosio_action_" + decl->getNameAsString() + ((decl->getParent()) ? "_"+decl->getParent()->getNameAsString() : "");
         if (actions.count(full_action_name) == 0) {
            create_action_dispatch(decl);
            abigen::get().add_wasm_action(decl, full_action_name);
         }
         actions.insert(full_action_name); // insert the method action, so we don't create the dispatcher twice

         if (decl.isEosioReadOnly()) {
            read_only_actions.insert(*decl);
         }
      } else if (decl.isEosioNotify() && is_eosio_contract(decl, contract_name)) {
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

         std::string full_notify_name = "__eosio_notify_" + decl->getNameAsString() + ((decl->getParent()) ? "_"+decl->getParent()->getNameAsString() : "");
         if (notify_handlers.count(full_notify_name) == 0) {
            create_notify_dispatch(decl);
            abigen::get().add_wasm_notify(decl, full_notify_name);
         }
         notify_handlers.insert(full_notify_name); // insert the method action, so we don't create the dispatcher twice
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
      auto _decl = clang_wrapper::wrap_decl(decl);
      if (auto* fd = dyn_cast<clang::FunctionDecl>(decl)) {
         if (fd->getNameInfo().getAsString() == "apply" && _decl.isEosioWasmEntry())
            abigen::get().add_wasm_entries(_decl);
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
         process_global_var(decl); }
      return true;
   }

   virtual bool VisitCXXRecordDecl(CXXRecordDecl* _decl) {
      auto decl = clang_wrapper::wrap_decl(_decl);
      if (decl.isEosioContract()) {
         auto process_data_member = [this]( CXXRecordDecl* rd ) {
            for (auto it = rd->decls_begin(); it != rd->decls_end(); ++it) {
               if (auto* f = dyn_cast<FieldDecl>(*it) ) {
                  if (Expr *init = f->getInClassInitializer()) {
                     process_decl_init(f, init);
                  }
               }
            }
         };
         process_data_member(*decl);
      }
      return true;
   }

   void process_read_only_actions() const {
      for (auto const& ra : read_only_actions) {
         auto it = func_calls.find(ra);
         if (it != func_calls.end()) {
            std::string msg = "read-only action cannot call write host function";
            if (warn_action_read_only) {
               CDT_WARN("codegen_warning", ra->getLocation(), msg);
            } else {
               CDT_ERROR("codegen_error", ra->getLocation(), msg);
            }
         }
      }
   }
};

class eosio_codegen_consumer : public ASTConsumer {
private:
   std::string main_file;
   CompilerInstance* ci;

public:
   std::unique_ptr<eosio_codegen_visitor> visitor;

   explicit eosio_codegen_consumer(CompilerInstance* CI, StringRef file)
      : visitor(std::make_unique<eosio_codegen_visitor>(CI)), main_file(file), ci(CI) {}

   void HandleTranslationUnit(ASTContext& Context) override {
      auto& src_mgr = Context.getSourceManager();
      auto& f_mgr = src_mgr.getFileManager();
      auto main_fe = *f_mgr.getFile(main_file);
      if (main_fe) {
         auto fid = src_mgr.getOrCreateFileID(*f_mgr.getFile(main_file), SrcMgr::CharacteristicKind::C_User);
         visitor->set_main_fid(fid);
         visitor->set_main_name(main_fe->getName());
         visitor->TraverseDecl(Context.getTranslationUnitDecl());
         visitor->process_read_only_actions();

         try {
            std::string fn = std::to_string(std::hash<std::string>{}(output))+".cpp";
            std::ofstream ofs(get_temporary_path(fn));
            if (!ofs) throw;
            auto& RewriteBuf = visitor->get_rewriter().getEditBuffer(fid);
            ofs << std::string(RewriteBuf.begin(), RewriteBuf.end());
            ofs.close();
         } catch (...) {
            llvm::outs() << "Failed to create temporary file\n";
         }
      }
   }
};

class eosio_codegen_frontend_action: public PluginASTAction {
private:
   std::string contract_name;

public:
   std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef file) override {
      auto consumer = std::make_unique<eosio_codegen_consumer>(&CI, file);
      consumer->visitor->set_contract_name(contract_name);
      return consumer;
   }

   bool ParseArgs(const CompilerInstance& CI, const std::vector<std::string>& args) override {
      if (args.empty())
         return true;

      for (const auto& arg : tokenize(args[0])) {
         if (arg.starts_with("output=")) {
            output = arg.substr(arg.find("=")+1);
         } else if (arg.starts_with("contract=")) {
            contract_name = arg.substr(arg.find("=")+1);
         } else {
            return false;
         }
      }
      return true;
   }

   ActionType getActionType() override {
      return AddBeforeMainAction;
   }
};

static FrontendPluginRegistry::Add<eosio_codegen_frontend_action> eosio_codegen("eosio_codegen", "");
static FrontendPluginRegistry::Add<eosio_abigen_frontend_action> eosio_abigen("eosio_abigen", "");
