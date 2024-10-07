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
      return name == "func_start_logger" || name == "func_end_logger"     || 
             name == "call_logger"       || name == "bin_op_logger"       || 
             name == "load_logger"       || name == "store_logger"        || 
             name == "cast_logger"       || name == "unreachable_logger"  ||
             name == "uncond_br_logger"  || name == "cond_br_logger"      ||
             name == "bb_start_logger"   || name == "alloca_logger"       ||
             name == "icmp_logger"       || name == "getelementptr_logger";}

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

      auto* func_start_log_type = FunctionType::get(void_ret_type, func_start_param_types, false);
      FunctionCallee func_start_log = F.getParent()->getOrInsertFunction("func_start_logger", func_start_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to func_end_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> func_end_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                              Type::getInt64Ty(ctx)};              // inst_addr

      auto* func_end_log_type = FunctionType::get(void_ret_type, func_end_param_types, false);
      FunctionCallee func_end_log = F.getParent()->getOrInsertFunction("func_end_logger", func_end_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to call_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> call_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                          builder.getInt8Ty()->getPointerTo(), // callee_name
                                          Type::getInt64Ty(ctx)};              // inst_addr

      auto* call_log_type = FunctionType::get(void_ret_type, call_param_types, false);
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

      auto* bin_op_log_type = FunctionType::get(void_ret_type, bin_op_param_types, false);
      FunctionCallee bin_op_log = F.getParent()->getOrInsertFunction("bin_op_logger", bin_op_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to load_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> load_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                          Type::getInt32Ty(ctx),               // val
                                          Type::getInt64Ty(ctx),               // p_operand
                                          Type::getInt64Ty(ctx)};              // inst_addr

      auto* load_log_type = FunctionType::get(void_ret_type, load_param_types, false);
      FunctionCallee load_log = F.getParent()->getOrInsertFunction("load_logger", load_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to store_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> store_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                           Type::getInt32Ty(ctx),               // v_operand
                                           Type::getInt64Ty(ctx),               // p_operand
                                           Type::getInt64Ty(ctx)};              // inst_addr

      auto* store_log_type = FunctionType::get(void_ret_type, store_param_types, false);
      FunctionCallee store_log = F.getParent()->getOrInsertFunction("store_logger", store_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to cast_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> cast_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                          Type::getInt32Ty(ctx),               // val
                                          Type::getInt32Ty(ctx),               // operand
                                          builder.getInt8Ty()->getPointerTo(), // cast_name
                                          Type::getInt64Ty(ctx)};              // inst_addr

      auto* cast_log_type = FunctionType::get(void_ret_type, cast_param_types, false);
      FunctionCallee cast_log = F.getParent()->getOrInsertFunction("cast_logger", cast_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to unreachable_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> unreachable_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                                 Type::getInt64Ty(ctx)};              // inst_addr

      auto* unreachable_log_type = FunctionType::get(void_ret_type, unreachable_param_types, false);
      FunctionCallee unreachable_log = F.getParent()->getOrInsertFunction("unreachable_logger", unreachable_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to uncond_br_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> uncond_br_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                               Type::getInt64Ty(ctx),               // dest_addr
                                               Type::getInt64Ty(ctx)};              // inst_addr

      auto* uncond_br_log_type = FunctionType::get(void_ret_type, uncond_br_param_types, false);
      FunctionCallee uncond_br_log = F.getParent()->getOrInsertFunction("uncond_br_logger", uncond_br_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to cond_br_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> cond_br_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                             Type::getInt32Ty(ctx),               // cond
                                             Type::getInt64Ty(ctx),               // true_dest_addr
                                             Type::getInt64Ty(ctx),               // false_dest_addr
                                             Type::getInt64Ty(ctx)};              // inst_addr

      auto* cond_br_log_type = FunctionType::get(void_ret_type, cond_br_param_types, false);
      FunctionCallee cond_br_log = F.getParent()->getOrInsertFunction("cond_br_logger", cond_br_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to bb_start_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> bb_start_param_types = {builder.getInt8Ty()->getPointerTo(), // bb_func_name
                                              Type::getInt64Ty(ctx)};              // bb_addr

      auto* bb_start_log_type = FunctionType::get(void_ret_type, bb_start_param_types, false);
      FunctionCallee bb_start_log = F.getParent()->getOrInsertFunction("bb_start_logger", bb_start_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to alloca_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> alloca_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                            Type::getInt32Ty(ctx),               // val
                                            Type::getInt64Ty(ctx)};              // inst_addr

      auto* alloca_log_type = FunctionType::get(void_ret_type, alloca_param_types, false);
      FunctionCallee alloca_log = F.getParent()->getOrInsertFunction("alloca_logger", alloca_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to icmp_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> icmp_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                          Type::getInt32Ty(ctx),               // val
                                          builder.getInt8Ty()->getPointerTo(), // p_name
                                          Type::getInt32Ty(ctx),               // lhs
                                          Type::getInt32Ty(ctx),               // rhs
                                          Type::getInt64Ty(ctx)};              // inst_addr

      auto* icmp_log_type = FunctionType::get(void_ret_type, icmp_param_types, false);
      FunctionCallee icmp_log = F.getParent()->getOrInsertFunction("icmp_logger", icmp_log_type);

      //--------------------------------------------------------------------------
      // Prepare call to getelementptr_logger
      //--------------------------------------------------------------------------
      ArrayRef<Type*> getelementptr_param_types = {builder.getInt8Ty()->getPointerTo(), // func_name
                                                   Type::getInt32Ty(ctx),               // val
                                                   Type::getInt32Ty(ctx),               // ptr_op
                                                   Type::getInt64Ty(ctx)};              // inst_addr

      auto* getelementptr_log_type = FunctionType::get(void_ret_type, getelementptr_param_types, false);
      FunctionCallee getelementptr_log = F.getParent()->getOrInsertFunction("getelementptr_logger", getelementptr_log_type);

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
        // Insert call to bb_start_logger in the bb begin
        builder.SetInsertPoint(&B.front());

        Value* bb_func_name = builder.CreateGlobalStringPtr(F.getName());
        Value* bb_addr = ConstantInt::get(builder.getInt64Ty(), (int64_t)(&B));
        Value* args[] = {bb_func_name, bb_addr};
        builder.CreateCall(bb_start_log, args);

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

          // Insert call to load_logger() after LOAD
          else if (auto* load = dyn_cast<LoadInst>(&I)) {
            builder.SetInsertPoint(load);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
           
            Value* p_operand = load->getPointerOperand();
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* args[] = {func_name, load, p_operand, inst_addr};
            builder.CreateCall(load_log, args);
          }

          // Insert call to store_logger() after STORE
          else if (auto* store = dyn_cast<StoreInst>(&I)) {
            builder.SetInsertPoint(store);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

            Value* v_operand = store->getValueOperand();
            Value* p_operand = store->getPointerOperand();
            Value* func_name  = builder.CreateGlobalStringPtr(F.getName());
            Value* args[] = {func_name, v_operand, p_operand, inst_addr};
            builder.CreateCall(store_log, args);
          }

          // Insert call to cast_logger() after CAST
          else if (auto* cast = dyn_cast<CastInst>(&I)) {
            builder.SetInsertPoint(cast);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

            Value* operand = cast->getOperand(0);
            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* cast_name = builder.CreateGlobalStringPtr(cast->getOpcodeName());
            Value* args[] = {func_name, cast, operand, cast_name, inst_addr};
            builder.CreateCall(cast_log, args);
          }
          
          // Insert call to unreachable_logger() before UNREACHABLE
          else if (auto* unreachable = dyn_cast<UnreachableInst>(&I)) {
            builder.SetInsertPoint(unreachable);

            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* args[] = {func_name, inst_addr};
            builder.CreateCall(unreachable_log, args);
          }

          // Insert call to br_logger() before BR
          else if (auto* br = dyn_cast<BranchInst>(&I)) {
            builder.SetInsertPoint(br);

            if (br->isUnconditional()) {
              Value* func_name = builder.CreateGlobalStringPtr(F.getName());
              Value* dest_bb = br->getOperand(0);
              Value* dest_addr = ConstantInt::get(builder.getInt64Ty(), (int64_t)(dest_bb));
              Value* args[] = {func_name, dest_addr, inst_addr};
              builder.CreateCall(uncond_br_log, args);
            }

            else { // if (br->isConditional())
              Value* func_name = builder.CreateGlobalStringPtr(F.getName());
              Value* cond = br->getCondition();
              Value* true_dest_bb = br->getOperand (1);
              Value* false_dest_bb = br->getOperand (2);
              Value* true_dest_addr = ConstantInt::get(builder.getInt64Ty(), (int64_t)(true_dest_bb));
              Value* false_dest_addr = ConstantInt::get(builder.getInt64Ty(), (int64_t)(false_dest_bb));
              Value* args[] = {func_name, cond, true_dest_addr, false_dest_addr};
              builder.CreateCall(cond_br_log, args);
            }
          }

          // Insert call to alloca_logger() after ALLOCA
          else if (auto* alloca = dyn_cast<AllocaInst>(&I)) {
            builder.SetInsertPoint(alloca);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* args[] = {func_name, alloca, inst_addr};
            builder.CreateCall(alloca_log, args);
          }

          // Insert call to icmp_logger() after ICMP
          else if (auto* icmp = dyn_cast<ICmpInst>(&I)) {
            builder.SetInsertPoint(icmp);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* p_name = builder.CreateGlobalStringPtr(ICmpInst::getPredicateName(icmp->getPredicate()));
            Value* lhs = icmp->getOperand(0);
            Value* rhs = icmp->getOperand(1);
            Value* args[] = {func_name, icmp, p_name, lhs, rhs, inst_addr};
            builder.CreateCall(icmp_log, args);
          }

          // Insert call to getelementptr_logger() after GETELEMENTPTR
          else if (auto* getelementptr = dyn_cast<GetElementPtrInst>(&I)) {
            builder.SetInsertPoint(getelementptr);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

            Value* func_name = builder.CreateGlobalStringPtr(F.getName());
            Value* ptr_op = getelementptr->getPointerOperand();
            Value* args[] = {func_name, getelementptr, ptr_op, inst_addr};
            builder.CreateCall(getelementptr_log, args);
          }

          // If we got some instructions, that were not mantioned above
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