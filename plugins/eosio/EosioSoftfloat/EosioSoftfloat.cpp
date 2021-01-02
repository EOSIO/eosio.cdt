//===- EosioSoftfloat ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Pass.h"
#include "llvm/IR/Attributes.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <set>
#include <string>
#include <utility>

using namespace llvm;

#define DEBUG_TYPE "eosio_softfloat"

namespace {
  // EosioSoftfloat - Mutate the apply function as needed 
  struct EosioSoftfloatPass : public FunctionPass {
    static char ID; 
    EosioSoftfloatPass() : FunctionPass(ID) {}
  
    bool runOnFunction(Function &f) override {
       Function* f32add = (Function*)f.getParent()->getOrInsertFunction("_eosio_f32_add", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f32sub = (Function*)f.getParent()->getOrInsertFunction("_eosio_f32_sub", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f32mul = (Function*)f.getParent()->getOrInsertFunction("_eosio_f32_mul", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f32div = (Function*)f.getParent()->getOrInsertFunction("_eosio_f32_div", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f32rem = (Function*)f.getParent()->getOrInsertFunction("_eosio_f32_rem", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));

       Function* f64add = (Function*)f.getParent()->getOrInsertFunction("_eosio_f64_add", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f64sub = (Function*)f.getParent()->getOrInsertFunction("_eosio_f64_sub", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f64mul = (Function*)f.getParent()->getOrInsertFunction("_eosio_f64_mul", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f64div = (Function*)f.getParent()->getOrInsertFunction("_eosio_f64_div", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));
       Function* f64rem = (Function*)f.getParent()->getOrInsertFunction("_eosio_f64_rem", AttributeList{},
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()),
                                                                       Type::getFloatTy(f.getContext()));


       bool changed = false;
       std::set<std::pair<Instruction*, Instruction*>> working_set;
       for ( Function::iterator bb = f.begin(); bb != f.end(); bb++ ) {
          for ( BasicBlock::iterator i = bb->begin(); i != bb->end(); i++ ) {
             Instruction* inst = &*i;
             if (AllocaInst* alloca_inst = dyn_cast<AllocaInst>(&*inst)) {
                if (alloca_inst->getAllocatedType()->getTypeID() == Type::TypeID::X86_FP80TyID) {
                   alloca_inst->setAllocatedType(Type::getFP128Ty(f.getContext()));
                }
             }
             else if (BitCastInst* bc = dyn_cast<BitCastInst>(&*inst)) {
                auto src_ty  = bc->getSrcTy();
                auto dest_ty = bc->getDestTy();
                /*
                if (bc->get->getType()->getTypeID() == Type::TypeID::X86_FP80TyID) {
                   outs() << *(bc->getOperand(0)->getType()) <<  (*bc) << "\n";
                }
                */
             }
             else if (BinaryOperator* binop = dyn_cast<BinaryOperator>(&*inst)) {
                Function* func = nullptr;
                Type::TypeID tyid = binop->getOperand(0)->getType()->getTypeID();
                switch (i->getOpcode()) {
                   case Instruction::FAdd:
                      if (tyid == Type::TypeID::FloatTyID)
                        func = f32add;
                      else if (tyid == Type::TypeID::DoubleTyID)
                        func = f64add;
                      break;
                   case Instruction::FSub:
                      if (tyid == Type::TypeID::FloatTyID)
                        func = f32sub;
                      else if (tyid == Type::TypeID::DoubleTyID)
                        func = f64sub;
                      break;
                   case Instruction::FMul:
                      if (tyid == Type::TypeID::FloatTyID)
                        func = f32mul;
                      else if (tyid == Type::TypeID::DoubleTyID)
                        func = f64mul;
                      break;
                   case Instruction::FDiv:
                      if (tyid == Type::TypeID::FloatTyID)
                        func = f32div;
                      else if (tyid == Type::TypeID::DoubleTyID)
                        func = f64div;
                      break;
                }
                if (func) {
                   CallInst* f_call = CallInst::Create(func, {binop->getOperand(0), binop->getOperand(1)});
                   working_set.insert({binop, f_call});
                   changed = true;
                }
             }
          }
       }
       for (auto item : working_set) {
          ReplaceInstWithInst(std::get<0>(item), std::get<1>(item));
          //outs() << (*(std::get<1>(item)->getParent()));
       }
 
       return changed;
    }
  };
}

char EosioSoftfloatPass::ID = 0;
static RegisterPass<EosioSoftfloatPass> X("softfloat_fixup", "Eosio Softfloat Fixups");

static void registerEosioSoftfloatPass(const PassManagerBuilder&, legacy::PassManagerBase& PM) { PM.add(new EosioSoftfloatPass()); }
static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible, registerEosioSoftfloatPass);
