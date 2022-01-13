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

#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>
#include <utility>

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

namespace eosio { namespace cdt {
   // replace with std::quoted and std::make_unique when we can get better C++14 support for Centos
   std::string _quoted(const std::string& instr) {
      std::stringstream ss;
      for (char c : instr) {
         if (c == '"' || c == '\\')
            ss << '\\';
         ss << c;
      }
      return ss.str();
   }
   template<typename T, typename... Args>
   std::unique_ptr<T> _make_unique(Args&&... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
   }

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

   std::map<std::string, std::vector<include_double>>  global_includes;

   // remove after v1.7.0
   bool has_eosiolib = false;

   class eosio_ppcallbacks : public PPCallbacks {
      public:
         eosio_ppcallbacks(SourceManager& sm, std::string file) : sources(sm), fn(file) {}
      protected:
         virtual void InclusionDirective(
            SourceLocation hash_loc,
            const Token &include_token,
            StringRef file_name,
            bool is_angled,
            CharSourceRange filename_range,
            const FileEntry *file,
            StringRef search_path,
            StringRef relative_path,
            const clang::Module *imported,
            clang::SrcMgr::CharacteristicKind file_type) {
            auto fid = sources.getFileID(hash_loc);
            auto fe  = sources.getFileEntryForID(fid);

            if (!is_angled && !llvm::sys::path::is_absolute(fn) && llvm::sys::path::filename(fe->getName()) == llvm::sys::path::filename(fn)) {
               llvm::SmallString<64> abs_search_path(search_path);
               llvm::sys::fs::make_absolute(abs_search_path);
               global_includes[fe->getName().str()].emplace_back(
                     (abs_search_path + llvm::sys::path::get_separator() + file_name).str(),
                     filename_range.getAsRange());
            }

            if ( file_name.find("eosiolib") != StringRef::npos )
               has_eosiolib = true;
         }

         std::string fn;
         SourceManager& sources;
   };

   class eosio_codegen_visitor : public RecursiveASTVisitor<eosio_codegen_visitor>, public generation_utils {
      private:
         codegen& cg = codegen::get();
         FileID    main_fid;
         StringRef main_name;
         std::string main_file;
         std::stringstream ss;
         CompilerInstance* ci;
         bool apply_was_found = false;
         SourceManager *sm = nullptr;

      public:

         bool in_const_func = false;
         clang::Stmt *func_body = nullptr;

         struct check_point_t {
            enum type_t { invalid = 0, 
                          func_enter, single_back, single_front }; // just add check_point right after or front
            int type = invalid;
            std::string func;
         };
         std::map<uint64_t, std::vector<check_point_t> >  check_points;
         //std::vector<check_point_t>         check_points_stack;
         bool insert_check_point(uint32_t line0, uint32_t col0, uint32_t line1, uint32_t col1, int start_type, std::string func_ = "") {
            if (in_const_func) return false;
            uint64_t line_col0 = ((((uint64_t)line0) << 32) | col0);
            uint64_t line_col1 = ((((uint64_t)line1) << 32) | col1);
            if (line_col0 >= line_col1) return false;
            if (!line0 || !col0 || !line1 || !col1) return false;
            
            if (check_points[line_col0].size() == 0) {
               check_points[line_col0].push_back(check_point_t{.type = start_type, .func = func_});
            }
            return true;
         }


      public:
         std::vector<CXXMethodDecl*> action_decls;
         std::vector<CXXMethodDecl*> notify_decls;

         explicit eosio_codegen_visitor(CompilerInstance *CI, std::string main_file_)
               : generation_utils([&](){throw cg.codegen_ex;}), ci(CI) {
            cg.ast_context = &(CI->getASTContext());
            cg.codegen_ci = CI;
            sm = &(CI->getSourceManager());
            main_file = main_file_;
         }

         void set_main_fid(FileID fid) {
            main_fid = fid;
         }

         void set_main_name(StringRef mn) {
            main_name = mn;
         }

         auto& get_ss() { return ss; }

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

         /*
         virtual bool VisitFunctionTemplateDecl(FunctionTemplateDecl* decl) {
            if (decl->getNameAsString() == "operator<<") {
               if (decl->getTemplatedDecl()->getNumParams() == 2) {
                  auto param0 = decl->getTemplatedDecl()->getParamDecl(0)->getOriginalType();
                  if (is_datastream(param0)) {
                     if (auto tp = dyn_cast<NamedDecl>(decl->getTemplatedDecl()->getParamDecl(1)->getOriginalType().getTypePtr()->getPointeeCXXRecordDecl())) {
                        cg.defined_datastreams.insert(tp->getQualifiedNameAsString());
                     }
                  }
               }
            }
            return true;
         }
         */

         template <typename F>
         void create_dispatch(const std::string& attr, const std::string& func_name, F&& get_str, CXXMethodDecl* decl) {
            constexpr static uint32_t max_stack_size = 512;
            codegen& cg = codegen::get();
            std::string nm = decl->getNameAsString()+"_"+decl->getParent()->getNameAsString();
            if (cg.is_eosio_contract(decl, cg.contract_name)) {
               if (has_eosiolib) {
                  ss << "\n\n#include <eosiolib/datastream.hpp>\n";
                  ss << "#include <eosiolib/name.hpp>\n";
               } else {
                  ss << "\n\n#include <eosio/datastream.hpp>\n";
                  ss << "#include <eosio/name.hpp>\n";
               }
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
                  clang::PrintingPolicy policy(lang_opts);
                  auto qt = param->getOriginalType().getNonReferenceType();
                  qt.removeLocalConst();
                  qt.removeLocalVolatile();
                  qt.removeLocalRestrict();
                  std::string tn = clang::TypeName::getFullyQualifiedName(qt, *(cg.ast_context), policy);
                  tn = tn == "_Bool" ? "bool" : tn; // TODO look out for more of these oddities
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
//               validate_name(name, [&]() {emitError(*ci, decl->getLocation(), "action not a valid eosio name");});
               if (!_action_set.count(name))
                  _action_set.insert(name);
               else {
                  auto itr = _action_set.find(name);
                  if (*itr != name)
                     emitError(*ci, decl->getLocation(), "action declaration doesn't match previous declaration");
               }
               std::string full_action_name = decl->getNameAsString() + ((decl->getParent()) ? decl->getParent()->getNameAsString() : "");
               if (cg.actions.count(full_action_name) == 0) {
                  create_action_dispatch(decl);
               }
               cg.actions.insert(full_action_name); // insert the method action, so we don't create the dispatcher twice
            }
            else if (decl->isEosioNotify()) {

               name = generation_utils::get_notify_pair(decl);
               auto first = name.substr(0, name.find("::"));
               if (first != "*")
                  validate_name(first, [&]() {emitError(*ci, decl->getLocation(), "invalid contract name");});
               auto second = name.substr(name.find("::")+2);
     //          validate_name(second, [&]() {emitError(*ci, decl->getLocation(), "invalid action name");});

               if (!_notify_set.count(name))
                  _notify_set.insert(name);
               else {
                  auto itr = _notify_set.find(name);
                  if (*itr != name)
                     emitError(*ci, decl->getLocation(), "notify handler declaration doesn't match previous declaration");
               }

               std::string full_notify_name = decl->getNameAsString() + ((decl->getParent()) ? decl->getParent()->getNameAsString() : "");
               if (cg.notify_handlers.count(full_notify_name) == 0) {
                  create_notify_dispatch(decl);
               }
               cg.notify_handlers.insert(full_notify_name); // insert the method action, so we don't create the dispatcher twice
            }

            return true;
         }

         bool insert_check_point_by_location(const char *mark, clang::SourceLocation locstart, clang::SourceLocation locend, int type, std::string func_name = "") {
            bool invalid = false;
            if (locstart.isFileID() && locend.isFileID()) {
               clang::PresumedLoc plocs = sm->getPresumedLoc(locstart);
               clang::PresumedLoc ploce = sm->getPresumedLoc(locend);
               if (plocs.isInvalid() || ploce.isInvalid()) return false;
               if (plocs.getLine() > ploce.getLine()) return false;
               //if (plocs.getLine() == ploce.getLine() && plocs.getColumn() >= ploce.getColumn()) return true;

               std::string file_name = plocs.getFilename();
               if (file_name == main_file) {
                  if (insert_check_point(plocs.getLine(), plocs.getColumn(), ploce.getLine(), ploce.getColumn(), type, func_name)) {
                     std::cout << "inserted checkpoint at " << locstart.printToString(*sm) << "(" << mark << ")" << std::endl;
                  }
               }
            }
            return true;
         }

         void visit_expr(clang::Expr *expr_, const ASTContext& ctx) {
            if (!expr_) return;
            //std::cout << " " << expr_->getStmtClassName() << " ";
            if (clang::ExprWithCleanups *expr = llvm::dyn_cast<clang::ExprWithCleanups>(expr_)) {
               visit_expr(expr->getSubExpr(), ctx);
            } else if (clang::CallExpr *expr = llvm::dyn_cast<clang::CallExpr>(expr_)) {
               for (auto itr = expr->arg_begin(); itr != expr->arg_end(); ++itr) {
                  visit_expr(*itr, ctx);
               }
            } else if (clang::LambdaExpr *expr = llvm::dyn_cast<clang::LambdaExpr>(expr_)) {
               visit_compound_stmt(expr->getBody(), ctx);
            } else if (clang::MaterializeTemporaryExpr *expr = llvm::dyn_cast<clang::MaterializeTemporaryExpr>(expr_)) {
               clang::Stmt *tmp = expr->getTemporary();
               //std::cout << " " << tmp->getStmtClassName() << " ";
               visit_expr(llvm::dyn_cast<Expr>(tmp), ctx);
            }
         }

         void visit_compound_stmt(clang::Stmt *stmt_, const ASTContext& ctx) {
            if (!stmt_ || stmt_->getStmtClass() != clang::Stmt::CompoundStmtClass) return;

            clang::CompoundStmt *cpst = (clang::CompoundStmt *)stmt_;
            for (auto body_itr = cpst->body_begin(); body_itr != cpst->body_end(); ++body_itr) {
               clang::Stmt *s = *body_itr;
               if (!s) continue;

               const char *label_name = "";
               // remove label & case recursively
               while (true) {
                  if (auto *ls = llvm::dyn_cast<clang::LabelStmt>(s)) {
                     label_name = ls->getName();
                     s = ls->getSubStmt();
                  }
                  else if (auto *sc = llvm::dyn_cast<clang::SwitchCase>(s))
                     s = sc->getSubStmt();
                  else break;
               }

               insert_check_point_by_location((s->getStmtClassName() + std::string(" ")).c_str(), s->getLocStart(), s->getLocEnd(), check_point_t::single_front, label_name);

               if (s->getStmtClass() == clang::Stmt::IfStmtClass) {
                  clang::IfStmt *stmt = (clang::IfStmt *)s;
                  visit_compound_stmt(stmt->getThen(), ctx);
                  visit_compound_stmt(stmt->getElse(), ctx);
               } else if (s->getStmtClass() == clang::Stmt::WhileStmtClass) {
                  clang::WhileStmt *stmt = (clang::WhileStmt *)s;
                  visit_compound_stmt(stmt->getBody(), ctx);
               } else if (s->getStmtClass() == clang::Stmt::DoStmtClass) {
                  clang::DoStmt *stmt = (clang::DoStmt *)s;
                  visit_compound_stmt(stmt->getBody(), ctx);
               } else if (s->getStmtClass() == clang::Stmt::ForStmtClass) {
                  clang::ForStmt *stmt = (clang::ForStmt *)s;
                  visit_compound_stmt(stmt->getBody(), ctx);
               } else if (s->getStmtClass() == clang::Stmt::SwitchStmtClass) {
                  clang::SwitchStmt *stmt = (clang::SwitchStmt *)s;
                  visit_compound_stmt(stmt->getBody(), ctx);
               } else if (clang::Expr *expr = llvm::dyn_cast<clang::Expr>(s)) {
                  visit_expr(expr, ctx);
               }
            }
         }

         virtual bool VisitDecl(clang::Decl* decl) {
            if (clang::FunctionDecl* fd = dyn_cast<clang::FunctionDecl>(decl)) {
               if (fd->getNameInfo().getAsString() == "apply")
                  apply_was_found = true;

               func_body = fd->getBody();
               in_const_func = fd->isConstexpr();
               if (!in_const_func && fd->hasBody()) {
                  clang::Stmt *stmt = fd->getBody();
                  if (stmt->getStmtClass() == clang::Stmt::CompoundStmtClass) {
                     clang::CompoundStmt *cpst = (clang::CompoundStmt *)stmt;
                     insert_check_point_by_location("CompoundStmt(FuncEnter) ", stmt->getLocStart(), stmt->getLocEnd(), check_point_t::func_enter, fd->getNameInfo().getAsString());
                     visit_compound_stmt(cpst, decl->getASTContext());
                  }
               }
            }
            return true;
         }

         // FIXME: the expression 
         // virtual bool WalkUpFromCallExpr(clang::CallExpr *stmt) {
         //    if (stmt->getStmtClass() == clang::Stmt::CXXOperatorCallExprClass ||
         //        stmt->getStmtClass() == clang::Stmt::UserDefinedLiteralClass ||
         //        stmt->getStmtClass() == clang::Stmt::CXXMemberCallExprClass
         //        ) {
         //       return true;
         //    }
         //    insert_check_point_by_location("CallExpr ", stmt->getLocStart(), stmt->getLocEnd(), check_point_t::exp_start);  
         //    return true;
         // }
         
         //virtual bool WalkUpFromReturnStmt(clang::ReturnStmt *stmt) {
         //   insert_check_point_by_location(stmt->getLocStart(), stmt->getLocEnd(), check_point_t::stmt_start);  
         //   return true;            
         //}

      };

      class eosio_codegen_consumer : public ASTConsumer {
      private:
         eosio_codegen_visitor *visitor;
         std::string main_file;
         CompilerInstance* ci;

      public:
         explicit eosio_codegen_consumer(CompilerInstance *CI, std::string file)
            : visitor(new eosio_codegen_visitor(CI, file)), main_file(file), ci(CI) { }


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
               for (auto ad : visitor->action_decls)
                  visitor->create_action_dispatch(ad);

               for (auto nd : visitor->notify_decls)
                  visitor->create_notify_dispatch(nd);

               if (cg.actions.size() < 1 && cg.notify_handlers.size() < 1) {
                  return;
               }

               llvm::SmallString<128> fn;
               try {
                  llvm::sys::fs::createTemporaryFile("eosio", ".cpp", fn);

                  std::stringstream out_data;
                  std::ofstream out(fn.c_str());


                  // {
                  //    llvm::SmallString<64> abs_file_path(main_fe->getName());
                  //    llvm::sys::fs::make_absolute(abs_file_path);
                  //    out_data << "#include \"" << abs_file_path.c_str() << "\"\n";
                  // }

                  {
                     llvm::SmallString<64> abs_file_path(main_fe->getName());
                     llvm::sys::fs::make_absolute(abs_file_path);
                     std::string short_fn = abs_file_path.c_str();
                     if (short_fn.find_last_of("/") != std::string::npos) {
                        short_fn = short_fn.substr(short_fn.find_last_of("/") + 1);
                     }
                     FILE *fin = fopen(abs_file_path.c_str(), "rb");
                     if (!fin) {
                        std::cout << "failed to open " << abs_file_path.c_str() << " for read\n";
                        out_data << "#include \"" << abs_file_path.c_str() << "\"\n";
                     } else {
                        uint32_t line = 1, col = 1;
                        while (true) {
                           char c = fgetc(fin);
                           if (feof(fin)) break;
                           uint64_t line_col = ((uint64_t)line << 32) | col;
                           if (c == '\r') continue;
                           if (c == '\n') {
                              out_data << c;
                              ++line;
                              col = 1; 
                              continue;
                           }
                           if (visitor->check_points.find(line_col) != visitor->check_points.end()) {
                              const std::vector<eosio_codegen_visitor::check_point_t> &list = visitor->check_points[line_col];
                              for (size_t i = 0; i < list.size(); ++i) {
                                 if (list[i].type == eosio_codegen_visitor::check_point_t::single_front) {
                                    out_data << "printf(\"@" << short_fn << ":" << line << ":" << col;
                                    if (list[i].func != "") {
                                       out_data << "(@" << list[i].func << ")";
                                    }
                                    out_data << "_\");";
                                 }
                              }
                              out_data << c;
                              for (int i = list.size() - 1; i >= 0; --i) {
                                 if (list[i].type == eosio_codegen_visitor::check_point_t::func_enter) {
                                    std::stringstream tmp_struct_ss;
                                    tmp_struct_ss << "_cdt_debug_func_" << line;
                                    std::string tmp_struct_name = tmp_struct_ss.str();
                                    out_data << "struct " << tmp_struct_name
                                             << "{"
                                             << tmp_struct_name
                                             << "(){printf(\"@" << short_fn << ":" << line << ":" << col << "(@" << list[i].func << ")_\");}"
                                             << "~" << tmp_struct_name 
                                             << "(){printf(\"@" << short_fn << "(~" << list[i].func << ")_\");}"
                                             << "} " 
                                             << tmp_struct_name << "_1;";
                                 } else if (list[i].type == eosio_codegen_visitor::check_point_t::single_back) {
                                    out_data << "printf(\"@" << short_fn << ":" << line << ":" << col << "_\");";
                                 }
                              }
                           } else {
                              out_data << c;
                           }
                           ++col;
                        }
                        fclose(fin);
                        out_data << "\n\n";
                     }
                  }

                  // generate apply stub with abi
                  std::stringstream& ss = visitor->get_ss();
                  ss << "extern \"C\" {\n";
                  ss << "__attribute__((eosio_wasm_import))\n";
                  ss << "void eosio_assert_code(uint32_t, uint64_t);";
                  ss << "\t__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi(";
                  std::string abi = cg.abi;
                  ss << "\"" << _quoted(abi) << "\"";
                  ss << ")))\n";
                  ss << "\tvoid __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){";
                  ss << "eosio_assert_code(false, 1);";
                  ss << "}\n";
                  ss << "}";

                  out_data << ss.str();

                  out << out_data.str();
                  cg.tmp_files.emplace(main_file, fn.str());
                  out.close();
                  std::cout << "main_file:" << main_file << ", temp_file:" << std::string(fn.str()) << std::endl;

                  std::string debug_file = std::string(main_fe->getName()) + ".debug";
                  std::ofstream out2(debug_file.c_str());
                  out2 << out_data.str();
                  out2.close();

                  std::cout << "debug file " << debug_file << " written\n";
               } catch (...) {
                  llvm::outs() << "Failed to create temporary file\n";
               }
            }
         }

      };

      class eosio_codegen_frontend_action : public ASTFrontendAction {
      public:
         virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
            std::cout << "CreateASTConsumer() file=" << std::string(file) << std::endl;
            CI.getPreprocessor().addPPCallbacks(_make_unique<eosio_ppcallbacks>(CI.getSourceManager(), file.str()));
            return _make_unique<eosio_codegen_consumer>(&CI, file);
         }
   };

}} // ns eosio::cdt
