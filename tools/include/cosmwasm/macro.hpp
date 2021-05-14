#pragma once

#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>

#define COSMWASM_ATTR(NAME, name, GNU, CXX11, NUM, ONUM, SUBJECTS) \
   namespace blanc { namespace cosmwasm { \
      struct NAME ## AttrInfo : public ParsedAttrInfo { \
        unsigned _NumArgs = NUM; \
        NAME ## AttrInfo() { \
          OptArgs = ONUM; \
          static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, #GNU}, \
                                           {ParsedAttr::AS_CXX11, #CXX11}}; \
          Spellings = S; \
        } \
        bool diagAppertainsToDecl(Sema &S, const ParsedAttr &Attr, \
                                  const Decl *D) const override { \
          if (SUBJECTS) { \
            S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type_str) \
              << Attr << "declarations"; \
            return false; \
          } \
          return true; \
        } \
        AttrHandling handleDeclAttribute(Sema &S, Decl *D, \
                                         const ParsedAttr &Attr) const override { \
          StringRef Str = ""; \
          if (Attr.getNumArgs() > 0) { \
            Expr *ArgExpr = Attr.getArgAsExpr(0); \
            clang::StringLiteral *Literal = \
                dyn_cast<clang::StringLiteral>(ArgExpr->IgnoreParenCasts()); \
            if (Literal) { \
              Str = Literal->getString(); \
            } else { \
              S.Diag(ArgExpr->getExprLoc(), diag::err_attribute_argument_type) \
                  << Attr.getAttrName() << AANT_ArgumentString; \
              return AttributeNotApplied; \
            } \
          } \
          else if (spellingIndexToSemanticSpelling(Attr)) { \
             auto& SM = S.getSourceManager(); \
             auto AttrRange = SM.getExpansionRange(Attr.getRange()); \
             auto LangOpts = S.Context.getLangOpts(); \
             auto offset = Lexer::getSourceText(SM.getExpansionRange(AttrRange.getEnd()), SM, LangOpts).size(); \
             auto Begin = AttrRange.getEnd().getLocWithOffset(offset); \
             if (Lexer::getSourceText(CharSourceRange(SourceRange(Begin), true), SM, LangOpts) == "(") { \
                Str = Lexer::getSourceText(CharSourceRange(SourceRange(Begin.getLocWithOffset(1)), true), SM, LangOpts); \
             } else if (_NumArgs) { \
               S.Diag(Attr.getLoc(), diag::err_attribute_argument_type) \
                   << Attr.getAttrName() << "attribute takes one argument"; \
               return AttributeNotApplied; \
             } \
          } \
          auto arg = Str.str(); \
          if (arg[0] == '\"') arg = arg.substr(1, arg.size()-2); \
          auto* anno = D->getAttr<AnnotateAttr>(); \
          if (anno) { \
             if (Str.empty()) { \
                anno->setAnnotation(S.Context, (anno->getAnnotation().str() + ", " + #name)); \
             } else { \
                anno->setAnnotation(S.Context, (anno->getAnnotation().str() + ", " + #name + "(" + arg + ")")); \
             } \
          } else { \
             if (Str.empty()) { \
                D->addAttr(AnnotateAttr::Create(S.Context, #name, \
                                                Attr.getRange())); \
             } else { \
                D->addAttr(AnnotateAttr::Create(S.Context, #name + std::string("(") + arg + ")", \
                                                Attr.getRange())); \
             } \
          } \
          return AttributeApplied; \
        } \
      }; \
   } } \
   static ParsedAttrInfoRegistry::Add<blanc::cosmwasm::NAME ## AttrInfo> NAME(#GNU, "");
