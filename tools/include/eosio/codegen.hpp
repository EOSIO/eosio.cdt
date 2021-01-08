#pragma once
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
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
#include <chrono>
#include <ctime>
#include <utility>

#include "abigen.hpp"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

namespace eosio { namespace cdt {

   struct codegen_exception : public std::exception {
      virtual const char* what() const throw() {
         return "eosio.codegen fatal error";
      }
   };

   struct include_double {
      include_double(std::string fn, SourceRange sr) : file_name(fn), range(sr) {}
      std::string    file_name;
      SourceRange    range;
   };

   class codegen : public generation_utils {
   public:
      codegen_exception codegen_ex;
      Rewriter          codegen_rewriter;
      CompilerInstance* codegen_ci;
      std::string       contract_name;
      std::string       abi;
      abigen            ag;
      std::map<std::string,std::string> actions;
      std::set<std::string> notify_handlers;
      ASTContext *ast_context;
      std::string output;
      bool has_added_clauses = false;

      codegen() : generation_utils([&](){throw codegen_ex;}) {
      }

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
   };

   class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
   private:
      codegen& cg = codegen::get();
      FileID    main_fid;
      StringRef main_name;
      Rewriter  rewriter;
      CompilerInstance* ci;

   public:
      std::vector<CXXMethodDecl*> action_decls;
      std::vector<CXXMethodDecl*> notify_decls;

      explicit eosio_codegen_visitor(CompilerInstance *CI)
            : generation_utils([&](){throw cg.codegen_ex;}), ci(CI) {
         cg.ast_context = &(CI->getASTContext());
         cg.codegen_ci = CI;
         rewriter.setSourceMgr(CI->getASTContext().getSourceManager(), CI->getASTContext().getLangOpts());
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

      template <size_t N>
      void emitError(CompilerInstance& inst, SourceLocation loc, const char (&err)[N]) {
         FullSourceLoc full(loc, inst.getSourceManager());
         unsigned id = inst.getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Error, err);
         inst.getDiagnostics().Report(full, id);
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

      template <typename F, typename D>
      void create_dispatch(const std::string& attr, const std::string& func_name, F&& get_str, D decl) {
         constexpr static uint32_t max_stack_size = 512;
         std::stringstream ss;
         codegen& cg = codegen::get();
         std::string nm = decl->getNameAsString()+"_"+decl->getParent()->getNameAsString();
         if (cg.is_eosio_contract(decl, cg.contract_name)) {
            ss << "\n\n#include <eosio/datastream.hpp>\n";
            ss << "#include <eosio/name.hpp>\n";
            ss << "extern \"C\" {\n";
            ss << "  uint32_t action_data_size();\n";
            ss << "  uint32_t read_action_data(void*, uint32_t);\n";
            ss << "  __attribute__((weak))\n";
            ss << "  void " << func_name << nm << "(unsigned long long r, unsigned long long c) {\n";
            ss << "    size_t as = ::action_data_size();\n";
            ss << "    void* buff = nullptr;\n";
            ss << "    if (as > 0) {\n";
            ss << "      buff = as >= " << max_stack_size << " ? malloc(as) : alloca(as);\n";
            ss << "    ::read_action_data(buff, as);\n";
            ss << "    }\n";
            ss << "    eosio::datastream<const char*> ds{(char*)buff, as};\n";
            int i=0;
            for (auto param : decl->parameters()) {
               clang::LangOptions lang_opts;
               lang_opts.CPlusPlus = true;
               clang::PrintingPolicy policy(lang_opts);
               auto qt = param->getOriginalType().getNonReferenceType();
               qt.removeLocalConst();
               qt.removeLocalVolatile();
               qt.removeLocalRestrict();
               std::string tn = clang::TypeName::getFullyQualifiedName(qt, *(cg.ast_context), policy);
               tn = tn == "_Bool" ? "bool" : tn; // TODO look out for more of these oddities
               ss << "    " << tn << " arg" << i << "; ds >> arg" << i << ";\n";
               i++;
            }
            ss << "    " << decl->getParent()->getQualifiedNameAsString() << "{eosio::name{r},eosio::name{c},ds}." << decl->getNameAsString() << "(";
            for (int i=0; i < decl->parameters().size(); i++) {
               ss << "arg" << i;
               if (i < decl->parameters().size()-1)
                  ss << ", ";
            }
            ss << ");\n";
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

      bool shouldVisitTemplateInstantiations() const {
         return true;
      }

      virtual bool VisitCXXMethodDecl(CXXMethodDecl* _decl) {
         auto decl = clang_wrapper::make_decl(_decl);
         std::string name = decl->getNameAsString();
         static std::set<std::string> _action_set; //used for validations
         static std::set<std::string> _notify_set; //used for validations
         if (decl.isEosioAction() && abigen::is_eosio_contract(decl, cg.ag.get_contract_name())) {
            name = generation_utils::get_action_name(decl);
            validate_name(name, [&]() {emitError(*ci, decl->getLocation(), "action not a valid eosio name");});
            if (!_action_set.count(name))
               _action_set.insert(name);
            else {
               auto itr = _action_set.find(name);
               if (*itr != name)
                  emitError(*ci, decl->getLocation(), "action declaration doesn't match previous declaration");
            }
            std::string full_action_name = "__eosio_action_" + decl->getNameAsString() + ((decl->getParent()) ? "_"+decl->getParent()->getNameAsString() : "");
            if (cg.actions.find(name) == cg.actions.end()) {
               create_action_dispatch(decl);
               cg.actions.emplace(name, full_action_name);
               cg.ag.add_wasm_action(decl, full_action_name);
            }
         }
         else if (decl.isEosioNotify() && abigen::is_eosio_contract(decl, cg.ag.get_contract_name())) {
            name = generation_utils::get_notify_pair(decl);
            auto first = name.substr(0, name.find("::"));
            if (first != "*")
               validate_name(first, [&]() {emitError(*ci, decl->getLocation(), "invalid contract name");});
            auto second = name.substr(name.find("::")+2);
            validate_name(second, [&]() {emitError(*ci, decl->getLocation(), "invalid action name");});

            if (!_notify_set.count(name))
               _notify_set.insert(name);
            else {
               auto itr = _notify_set.find(name);
               if (*itr != name)
                  emitError(*ci, decl->getLocation(), "notify handler declaration doesn't match previous declaration");
            }

            std::string full_notify_name = "__eosio_notify_" + decl->getNameAsString() + ((decl->getParent()) ? "_"+decl->getParent()->getNameAsString() : "");
            if (cg.notify_handlers.count(full_notify_name) == 0) {
               create_notify_dispatch(decl);
               cg.ag.add_wasm_notify(decl, full_notify_name);
            }
            cg.notify_handlers.insert(full_notify_name); // insert the method action, so we don't create the dispatcher twice
         }

         if (decl.isEosioAction() && abigen::is_eosio_contract(decl, cg.ag.get_contract_name())) {
            cg.ag.add_struct(_decl);
            cg.ag.add_action(_decl);
            auto params = _decl->parameters();
            for (auto param : params) {
               cg.ag.add_type(param->getType());
            }
         }

         return true;
      }

      virtual bool VisitDecl(clang::Decl* decl) {
         auto _decl = clang_wrapper::make_decl(decl);
         if (auto* fd = dyn_cast<clang::FunctionDecl>(decl)) {
            if (fd->getNameInfo().getAsString() == "apply" && _decl.isEosioWasmEntry()) {
               cg.ag.add_wasm_entries(_decl);
            }
         }
         if (const auto* d = dyn_cast<clang::ClassTemplateSpecializationDecl>(decl)) {
            if (d->getName() == "multi_index") {
               cg.ag.add_table(d->getTemplateArgs()[0].getAsIntegral().getExtValue(),
                  (clang::CXXRecordDecl*)((clang::RecordType*)d->getTemplateArgs()[1].getAsType().getTypePtr())->getDecl());
            }
         }
         return true;
      }

      virtual bool VisitCXXRecordDecl(CXXRecordDecl* _decl) {
         auto decl = clang_wrapper::make_decl(_decl);
         auto& cg = codegen::get();

         if (!cg.has_added_clauses) {
            cg.ag.add_clauses(cg.ag.parse_clauses());
            cg.ag.add_contracts(cg.ag.parse_contracts());
            cg.has_added_clauses = true;
         }
         if (decl.isEosioAction() && abigen::is_eosio_contract(decl, cg.ag.get_contract_name())) {
            cg.ag.add_struct(_decl);
            cg.ag.add_action(_decl);
            for (auto field : decl->fields()) {
               cg.ag.add_type(field->getType());
            }
         }
         if (decl.isEosioTable() && abigen::is_eosio_contract(decl, cg.ag.get_contract_name())) {
            cg.ag.add_struct(_decl);
            cg.ag.add_table(_decl);
            for (auto field : decl->fields()) {
               cg.ag.add_type(field->getType());
            }
         }
         return true;
      }
   };

   class eosio_codegen_consumer : public ASTConsumer {
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
            auto fid = src_mgr.getOrCreateFileID(main_fe.get(), SrcMgr::CharacteristicKind::C_User);
            visitor->set_main_fid(fid);
            visitor->set_main_name(main_fe.get()->getName());
            visitor->TraverseDecl(Context.getTranslationUnitDecl());
            for (auto ad : visitor->action_decls)
               visitor->create_action_dispatch(ad);
            
            for (auto nd : visitor->notify_decls)
               visitor->create_notify_dispatch(nd);

            if (cg.actions.size() < 1 && cg.notify_handlers.size() < 1) {
               return;
            }

            try {
               std::ofstream ofs(get_temporary_path(std::to_string(std::hash<std::string>{}(cg.output))+".cpp"));
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

   class eosio_codegen_frontend_action : public ASTFrontendAction {
   public:
      virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
         return std::make_unique<eosio_codegen_consumer>(&CI, file.str());
      }
   };

}} // ns eosio::cdt
