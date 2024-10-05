#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

namespace {
  struct IrCollector : public FunctionPass {
    static char ID;
    IrCollector() : FunctionPass(ID) {}

    bool is_logger(StringRef name) {
      return name == "bin_op_logger" || name == "call_logger" ||name == "func_start_logger" || name == "func_end_logger";
    }

    virtual bool runOnFunction(Function& F) {
      if (is_logger(F.getName()))
        return false;

      //--------------------------------------------------------------------------
      // Builder
      //--------------------------------------------------------------------------
      LLVMContext& ctx = F.getContext();
      IRBuilder<> builder (ctx);
      Type* void_ret_type = Type::getVoidTy(ctx);

      //--------------------------------------------------------------------------
      // Prepare call to func_start_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> func_start_param_types = {builder.getInt8Ty()->getPointerTo()};

      FunctionType* func_start_log_type = FunctionType::get(void_ret_type, func_start_param_types, false);
      FunctionCallee func_start_log = F.getParent()->getOrInsertFunction("func_start_logger", func_start_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to func_end_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> func_end_param_types = {builder.getInt8Ty()->getPointerTo(), Type::getInt64Ty(ctx)};

      FunctionType* func_end_log_type = FunctionType::get(void_ret_type, func_end_param_types, false);
      FunctionCallee func_end_log = F.getParent()->getOrInsertFunction("func_end_logger", func_end_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to call_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> call_param_types = {builder.getInt8Ty()->getPointerTo(),
                                           builder.getInt8Ty()->getPointerTo(),
                                           Type::getInt64Ty(ctx)};

      FunctionType* call_log_type = FunctionType::get(void_ret_type, call_param_types, false);
      FunctionCallee call_log = F.getParent()->getOrInsertFunction("call_logger", call_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to bin_op_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> bin_op_param_types = {Type::getInt32Ty(ctx),
                                            Type::getInt32Ty(ctx),
                                            Type::getInt32Ty(ctx),
                                            builder.getInt8Ty()->getPointerTo(),
                                            builder.getInt8Ty()->getPointerTo(),
                                            Type::getInt64Ty(ctx)};

      FunctionType* bin_op_log_type = FunctionType::get(void_ret_type, bin_op_param_types, false);
      FunctionCallee bin_op_log = F.getParent()->getOrInsertFunction("bin_op_logger", bin_op_log_type);

      //--------------------------------------------------------------------------
      // Start inserting calls
      //--------------------------------------------------------------------------
      //Insert call to func_start_logger in the function begin
      BasicBlock& entry_bb = F.getEntryBlock();
      builder.SetInsertPoint(&entry_bb.front());

      Value* func_name = builder.CreateGlobalStringPtr(F.getName());
      Value* args[] = {func_name};
      builder.CreateCall(func_start_log, args);

      for (auto& B : F) {
        for (auto& I : B) {
          Value* insn_addr = ConstantInt::get(builder.getInt64Ty(), (int64_t)(&I));

          // Insert call to call_logger() before CALL
          if (auto* call = dyn_cast<CallInst>(&I)) {
            builder.SetInsertPoint(call);
            
            Function* callee = call->getCalledFunction();
            if (callee && !is_logger(callee->getName())) {
              Value* callee_name = builder.CreateGlobalStringPtr(callee->getName());
              Value* func_name = builder.CreateGlobalStringPtr(F.getName());
              Value* args[] = {func_name, callee_name, insn_addr};
              builder.CreateCall(call_log, args);
            }
          }

          // Insert call to func_end_logger() before RET
          if (auto* ret = dyn_cast<ReturnInst>(&I)) {
            builder.SetInsertPoint(ret);
            
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* args[] = {func_name, insn_addr};
            builder.CreateCall(func_end_log, args);
          }

          //Insert call to bin_op_logger() after OP
          if (auto* op = dyn_cast<BinaryOperator>(&I)) {
            builder.SetInsertPoint(op);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
           
            Value* lhs = op->getOperand(0);
            Value* rhs = op->getOperand(1);
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* op_name = builder.CreateGlobalStringPtr(op->getOpcodeName());
            Value* args[] = {func_name, op, lhs, rhs, op_name, insn_addr};
            builder.CreateCall(bin_op_log, args);
          }
        }
      }

      return true;
    }
  };
}

char IrCollector::ID = 0;

static void registerIrCollector(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new IrCollector());
}
static RegisterStandardPasses
  RegisterIrCollector(PassManagerBuilder::EP_EarlyAsPossible,
                 registerIrCollector);