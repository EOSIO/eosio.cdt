#pragma once

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include <exception>
#include <map>
#include <string>

namespace eosio { namespace cdt {
   struct internal_error : public std::exception {
      virtual const char* what() const throw() { return "eosio tool internal error"; }
   } internal_error_ex;

   struct error_emitter {
      error_emitter() = default;
      error_emitter(clang::CompilerInstance* i) : ci(i), diags(ci->getDiagnostics()) {}

      struct diag_ids {
         diag_ids() = default;
         template <typename T>
         diag_ids(T&& diag) {
            diags.emplace("abigen_error", diag.getCustomDiagID(clang::DiagnosticsEngine::Error, "abigen error (%0)"));
            diags.emplace("abigen_warning", diag.getCustomDiagID(clang::DiagnosticsEngine::Warning, "abigen warning (%0)"));
            diags.emplace("codegen_error", diag.getCustomDiagID(clang::DiagnosticsEngine::Error, "codegen error (%0)"));
            diags.emplace("codegen_warning", diag.getCustomDiagID(clang::DiagnosticsEngine::Warning, "codegen warning (%0)"));
         }
         uint32_t get(const std::string& s) { return diags.at(s); }
         std::map<std::string, uint32_t> diags;
      };

      void set_compiler_instance(clang::CompilerInstance* i) {
         ci = i;
         diags = diag_ids(i->getDiagnostics());
      }
      template <typename SrcLoc>
      void emit_error(SrcLoc&& loc, uint32_t id, const std::string& err) {
         clang::FullSourceLoc full(loc, ci->getSourceManager());
         ci->getDiagnostics().Report(full, id) << err;
      }
      template <typename SrcLoc>
      void emit_warning(SrcLoc&& loc, uint32_t id, const std::string& err) {
         clang::FullSourceLoc full(loc, ci->getSourceManager());
         ci->getDiagnostics().Report(full, id) << err;
      }
      clang::CompilerInstance* ci = nullptr;
      diag_ids diags;
   };
}}

#define CDT_CHECK_ERROR(x, e, l, s) \
   if (!(x)) \
      get_error_emitter().emit_error(l, get_error_emitter().diags.get(e), s);

#define CDT_CHECK_WARN(x, e, l, s) \
   if (!(x)) \
      get_error_emitter().emit_warning(l, get_error_emitter().diags.get(e), s);

#define CDT_WARN(e, l, s) \
   get_error_emitter().emit_warning(l, get_error_emitter().diags.get(e), s);

#define CDT_ERROR(e, l, s) \
   get_error_emitter().emit_error(l, get_error_emitter().diags.get(e), s);

#define CDT_INTERNAL_ERROR(s) \
   std::cerr << s << "\n";    \
   throw internal_error_ex;
