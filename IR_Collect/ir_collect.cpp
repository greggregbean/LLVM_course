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
      return name == "bin_op_logger"     || name == "call_logger"     ||
             name == "func_start_logger" || name == "func_end_logger" ||
             name == "load_logger"       || name == "store_logger";
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
      ArrayRef<Type*> func_start_param_types = {builder.getInt8Ty()->getPointerTo()}; // func_name

      FunctionType* func_start_log_type = FunctionType::get(void_ret_type, func_start_param_types, false);
      FunctionCallee func_start_log = F.getParent()->getOrInsertFunction("func_start_logger", func_start_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to func_end_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> func_end_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                              Type::getInt64Ty(ctx)};              // inst_addr

      FunctionType* func_end_log_type = FunctionType::get(void_ret_type, func_end_param_types, false);
      FunctionCallee func_end_log = F.getParent()->getOrInsertFunction("func_end_logger", func_end_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to call_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> call_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                          builder.getInt8Ty()->getPointerTo(), // callee_name
                                          Type::getInt64Ty(ctx)};              // inst_addr

      FunctionType* call_log_type = FunctionType::get(void_ret_type, call_param_types, false);
      FunctionCallee call_log = F.getParent()->getOrInsertFunction("call_logger", call_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to bin_op_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> bin_op_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                            Type::getInt32Ty(ctx),               // val
                                            Type::getInt32Ty(ctx),               // lhs
                                            Type::getInt32Ty(ctx),               // rhs
                                            builder.getInt8Ty()->getPointerTo(), // op_name
                                            Type::getInt64Ty(ctx)};              // inst_addr

      FunctionType* bin_op_log_type = FunctionType::get(void_ret_type, bin_op_param_types, false);
      FunctionCallee bin_op_log = F.getParent()->getOrInsertFunction("bin_op_logger", bin_op_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to load_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> load_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                          Type::getInt32Ty(ctx),               // val
                                          Type::getInt32Ty(ctx),               // p_operand
                                          builder.getInt8Ty()->getPointerTo(), // load_name
                                          Type::getInt64Ty(ctx)};              // inst_addr

      FunctionType* load_log_type = FunctionType::get(void_ret_type, load_param_types, false);
      FunctionCallee load_log = F.getParent()->getOrInsertFunction("load_logger", load_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to store_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> store_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                           Type::getInt32Ty(ctx),               // v_operand
                                           Type::getInt32Ty(ctx),               // p_operand
                                           builder.getInt8Ty()->getPointerTo(), // store_name
                                           Type::getInt64Ty(ctx)};              // inst_addr

      FunctionType* store_log_type = FunctionType::get(void_ret_type, store_param_types, false);
      FunctionCallee store_log = F.getParent()->getOrInsertFunction("store_logger", store_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to cast_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> cast_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                          Type::getInt32Ty(ctx),               // val
                                          Type::getInt32Ty(ctx),               // operand
                                          builder.getInt8Ty()->getPointerTo(), // cast_name
                                          Type::getInt64Ty(ctx)};              // inst_addr

      FunctionType* cast_log_type = FunctionType::get(void_ret_type, cast_param_types, false);
      FunctionCallee cast_log = F.getParent()->getOrInsertFunction("cast_logger", cast_log_type);

      //--------------------------------------------------------------------------
      // Start inserting calls
      //--------------------------------------------------------------------------
      // Insert call to func_start_logger in the function begin
      BasicBlock& entry_bb = F.getEntryBlock();
      builder.SetInsertPoint(&entry_bb.front());

      Value* func_name = builder.CreateGlobalStringPtr(F.getName());
      Value* args[] = {func_name};
      builder.CreateCall(func_start_log, args);

      for (auto& B : F) {
        for (auto& I : B) {
          Value* inst_addr = ConstantInt::get(builder.getInt64Ty(), (int64_t)(&I));

          // Insert call to call_logger() before CALL
          if (auto* call = dyn_cast<CallInst>(&I)) {
            builder.SetInsertPoint(call);
            
            Function* callee = call->getCalledFunction();
            if (callee && !is_logger(callee->getName())) {
              Value* callee_name = builder.CreateGlobalStringPtr(callee->getName());
              Value* func_name = builder.CreateGlobalStringPtr(F.getName());
              Value* args[] = {func_name, callee_name, inst_addr};
              builder.CreateCall(call_log, args);
            }
          }

          // Insert call to func_end_logger() before RET
          else if (auto* ret = dyn_cast<ReturnInst>(&I)) {
            builder.SetInsertPoint(ret);
            
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* args[] = {func_name, inst_addr};
            builder.CreateCall(func_end_log, args);
          }

          // Insert call to bin_op_logger() after OP
          else if (auto* bin_op = dyn_cast<BinaryOperator>(&I)) {
            builder.SetInsertPoint(bin_op);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
           
            Value* lhs = bin_op->getOperand(0);
            Value* rhs = bin_op->getOperand(1);
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* bin_op_name = builder.CreateGlobalStringPtr(bin_op->getOpcodeName());
            Value* args[] = {func_name, bin_op, lhs, rhs, bin_op_name, inst_addr};
            builder.CreateCall(bin_op_log, args);
          }

          // Insert call to load_logger() after load
          else if (auto* load = dyn_cast<LoadInst>(&I)) {
            builder.SetInsertPoint(load);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
           
            Value* p_operand = load->getPointerOperand();
            Type* type = load->getPointerOperandType();
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* load_name = builder.CreateGlobalStringPtr(load->getOpcodeName());
            Value* args[] = {func_name, load, p_operand, load_name, inst_addr};
            builder.CreateCall(load_log, args);
          }

          // Insert call to store_logger() after store
          else if (auto* store = dyn_cast<StoreInst>(&I)) {
            builder.SetInsertPoint(store);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

            Value* v_operand = store->getValueOperand();
            Value* p_operand = store->getPointerOperand();
            Type* type = store->getPointerOperandType();
            Value* func_name  = builder.CreateGlobalStringPtr(F.getName());
            Value* store_name = builder.CreateGlobalStringPtr(store->getOpcodeName());
            Value* args[] = {func_name, v_operand, p_operand, store_name, inst_addr};
            builder.CreateCall(store_log, args);
          }

          else if (auto* cast = dyn_cast<CastInst>(&I)) {
            builder.SetInsertPoint(cast);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

            Value* operand = cast->getOperand(0);
            Type* src_type = cast->getSrcTy();
            Type* dest_type = cast->getDestTy();
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* cast_name = builder.CreateGlobalStringPtr(cast->getOpcodeName());
            Value* args[] = {func_name, cast, operand, cast_name, inst_addr};
            builder.CreateCall(cast_log, args);
          }

          else {
            outs() << I.getOpcodeName() << "\n";
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