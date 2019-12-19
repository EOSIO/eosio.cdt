#pragma once

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"

struct error_emitter {
   error_emitter() = default;
   error_emitter(clang::CompilerInstance* i) : ci(i) {}
   void set_compiler_instance(clang::CompilerInstance* i) { ci = i; }
   template <size_t N, typename SrcLoc>
   void emit_error(SrcLoc&& loc, const char (&err)[N]) {
      clang::FullSourceLoc full(loc, ci->getSourceManager());
      uint32_t id = ci->getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Error, err);
      ci->getDiagnostics().Report(full, id);
   }
   template <size_t N, typename SrcLoc>
   void emit_warning(SrcLoc&& loc, const char (&err)[N]) {
      clang::FullSourceLoc full(loc, ci->getSourceManager());
      uint32_t id = ci->getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Warning, err);
      ci->getDiagnostics().Report(full, id);
   }
   clang::CompilerInstance* ci = nullptr;
};

#define CDT_CHECK_ERROR(x, s) \
   if (!x) \
      get_error_emitter().emit_error(s);

#define CDT_CHECK_WARN(x, s) \
   if (!x) \
      get_error_emitter().emit_warning(s);

#define CDT_WARN(s) \
   get_error_emitter().emit_warning(s);

#define CDT_ERROR(s) \
   get_error_emitter().emit_error(s);

