#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/Sema/ParsedAttr.h>
#include <clang/Sema/Sema.h>
#include <clang/Sema/SemaDiagnostic.h>
#include <llvm/IR/Attributes.h>

#include "macro.hpp"

using namespace clang;

COSMWASM_ATTR(CosmWasmInstantiate, instantiate, cosmwasm_instantiate, cosmwasm::instantiate, 0, 1, (!isa<CXXMethodDecl>(D)))
COSMWASM_ATTR(CosmWasmExecute, execute, cosmwasm_execute, cosmwasm::execute, 0, 1, (!isa<CXXMethodDecl>(D)))
COSMWASM_ATTR(CosmWasmQuery, query, cosmwasm_query, cosmwasm::query, 0, 1, (!isa<CXXMethodDecl>(D)))
