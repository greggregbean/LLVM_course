#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#include "../SDL/sim.h"

int main() {
    LLVMContext ctx;
    Module* module = new Module("top", ctx);
    IRBuilder<> builder(ctx);

//--------------------------------------------------------------------------------------------------------------  
// Building module
//-------------------------------------------------------------------------------------------------------------- 
    //----------------------------------------------------------------------------------------------------------
    // functions
    //----------------------------------------------------------------------------------------------------------
    FunctionType* app_func_type = FunctionType::get (Type::getVoidTy(ctx), false);
    Function* app_func = Function::Create (app_func_type, Function::ExternalLinkage, "app", module);
        
    ArrayRef<Type*> run_game_func_args_ref ({Type::getInt32PtrTy(ctx), Type::getInt32PtrTy(ctx)});
    FunctionType* run_game_func_type = FunctionType::get (Type::getVoidTy(ctx), run_game_func_args_ref, false);
    Function* run_game_func = Function::Create (run_game_func_type, Function::ExternalLinkage, "run_game", module);

    FunctionType* simRand_func_type = FunctionType::get (Type::getInt32Ty(ctx), false);
    FunctionCallee simRand_func = module->getOrInsertFunction ("simRand", simRand_func_type);

    ArrayRef<Type*> simPutPixel_func_args_ref ({Type::getInt32Ty(ctx), Type::getInt32Ty(ctx), Type::getInt32Ty(ctx)});
    FunctionType* simPutPixel_func_type = FunctionType::get (Type::getVoidTy(ctx), simPutPixel_func_args_ref, false);
    FunctionCallee simPutPixel_func = module->getOrInsertFunction("simPutPixel", simPutPixel_func_type);

    FunctionType* simFlush_func_type = FunctionType::get(Type::getVoidTy(ctx), false);
    FunctionCallee simFlush_func = module->getOrInsertFunction("simFlush", simFlush_func_type);

    ArrayRef<Type*> count_alive_neighbors_func_args_ref ({Type::getInt32Ty(ctx), Type::getInt32Ty(ctx), Type::getInt32PtrTy(ctx)});
    FunctionType* count_alive_neighbors_func_type = FunctionType::get (Type::getInt32Ty(ctx), count_alive_neighbors_func_args_ref, false);
    Function* count_alive_neighbors_func = Function::Create (count_alive_neighbors_func_type, Function::ExternalLinkage, "count_alive_neighbors", module);

    //----------------------------------------------------------------------------------------------------------
    // App_func
    //----------------------------------------------------------------------------------------------------------
        // basic blocks
        BasicBlock* app_entry_bb = BasicBlock::Create (ctx, "app_entry_bb", app_func);
        BasicBlock* start_config_bb = BasicBlock::Create (ctx, "start_config_bb", app_func);
        BasicBlock* call_run_game_bb = BasicBlock::Create (ctx, "call_run_game_bb", app_func);

        // entry_app_bb
        builder.SetInsertPoint (app_entry_bb);
        Value* appv_1 = builder.CreateAlloca (ArrayType::get (Type::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE));
        Value* appv_2 = builder.CreateAlloca (ArrayType::get (Type::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE));
        Value* br_start_config_bb = builder.CreateBr (start_config_bb);

        // start_config_bb
        builder.SetInsertPoint (start_config_bb);
        PHINode* appv_6 = builder.CreatePHI (Type::getInt32Ty(ctx), 2);
        Value* appv_7 = builder.CreateCall (simRand_func);
        Value* appv_8 = builder.CreateSRem (appv_7, ConstantInt::get (Type::getInt32Ty(ctx), 512));
        Value* appv_9 = builder.CreateCall (simRand_func);
        Value* appv_10 = builder.CreateSRem (appv_9, ConstantInt::get (Type::getInt32Ty(ctx), 256));
        Value* appv_11 = builder.CreateShl (appv_10, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", false, true);
        Value* appv_12 = builder.CreateAdd (appv_11, appv_8, "", false, true);
        Value* appv_13 = builder.CreateSExt (appv_12, Type::getInt64Ty(ctx));
        Value* appv_14 = builder.CreateInBoundsGEP (ArrayType::get (Type::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE), appv_1, {ConstantInt::get (Type::getInt64Ty(ctx), 0), appv_13});
        builder.CreateStore (ConstantInt::get (Type::getInt32Ty(ctx), 1), appv_14);
        Value* appv_15 = builder.CreateInBoundsGEP (ArrayType::get (Type::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE), appv_2, {ConstantInt::get (Type::getInt64Ty(ctx), 0), appv_13});
        builder.CreateStore (ConstantInt::get (Type::getInt32Ty(ctx), 1), appv_15);
        Value* appv_16 = builder.CreateAdd (appv_6, ConstantInt::get (Type::getInt32Ty(ctx), 1), "", true, true);
        Value* appv_17 = builder.CreateICmp (CmpInst::ICMP_EQ, appv_16, ConstantInt::get (Type::getInt32Ty(ctx), 100000));
        builder.CreateCondBr (appv_17, call_run_game_bb, start_config_bb);

        // call_run_game_bb
        builder.SetInsertPoint (call_run_game_bb);
        builder.CreateCall(run_game_func);
        builder.CreateRetVoid();

        // adding phi args
        appv_6->addIncoming (ConstantInt::get (Type::getInt32Ty(ctx), 0), app_entry_bb);
        appv_6->addIncoming (appv_16, start_config_bb);
    
    //----------------------------------------------------------------------------------------------------------
    // Run_game_func
    //----------------------------------------------------------------------------------------------------------
        // args
        Function::arg_iterator run_game_func_args_it = run_game_func->arg_begin();
        Value* run_gamev_0 = run_game_func_args_it;
        Value* run_gamev_1 = ++run_game_func_args_it;

        // basic blocks
        BasicBlock* run_game_entry_bb = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_3 = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_6 = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_10 = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_11 = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_14 = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_22 = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_26 = BasicBlock::Create (ctx, "", run_game_func);
        BasicBlock* run_game_bb_29 = BasicBlock::Create (ctx, "", run_game_func);

        // entry_run_game_bb
        builder.SetInsertPoint (run_game_entry_bb);
        builder.CreateBr (run_game_bb_3);

        // run_game_bb_3
        builder.SetInsertPoint (run_game_bb_3);
        PHINode* run_gamev_4 = builder.CreatePHI (Type::getInt32PtrTy(ctx), 2);
        PHINode* run_gamev_5 = builder.CreatePHI (Type::getInt32PtrTy(ctx), 2);
        builder.CreateBr (run_game_bb_6);

        // run_game_bb_6
        builder.SetInsertPoint (run_game_bb_6);
        PHINode* run_gamev_7 = builder.CreatePHI (Type::getInt64Ty(ctx), 2);
        Value* run_gamev_8 = builder.CreateTrunc (run_gamev_7, Type::getInt32Ty(ctx));
        Value* run_gamev_9 = builder.CreateTrunc (run_gamev_7, Type::getInt32Ty(ctx));
        builder.CreateBr (run_game_bb_14);

        // run_game_bb_10
        builder.SetInsertPoint (run_game_bb_10);
        builder.CreateCall (simFlush_func);
        builder.CreateBr (run_game_bb_3);

        // run_game_bb_11
        builder.SetInsertPoint (run_game_bb_11);
        Value* run_gamev_12 = builder.CreateAdd (run_gamev_7, ConstantInt::get (Type::getInt64Ty(ctx), 1), "", true, true);
        Value* run_gamev_13 = builder.CreateICmp (CmpInst::ICMP_EQ, run_gamev_12, ConstantInt::get (Type::getInt64Ty(ctx), 512));
        builder.CreateCondBr (run_gamev_13, run_game_bb_10, run_game_bb_6);

        // run_game_bb_14
        builder.SetInsertPoint (run_game_bb_14);
        PHINode* run_gamev_15 = builder.CreatePHI (Type::getInt64Ty(ctx), 2);
        Value* run_gamev_16 = builder.CreateShl (run_gamev_15, ConstantInt::get (Type::getInt64Ty(ctx), 9), "", true, true);
        Value* run_gamev_17 = builder.CreateAdd (run_gamev_16, run_gamev_7, "", true, true);
        Value* run_gamev_18 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), run_gamev_5, run_gamev_17);
        Value* run_gamev_19 = builder.CreateLoad (Type::getInt32Ty(ctx), run_gamev_18);
        Value* run_gamev_20 = builder.CreateICmp (CmpInst::ICMP_EQ, run_gamev_19, ConstantInt::get (Type::getInt32Ty(ctx), 1));
        Value* run_gamev_21 = builder.CreateTrunc (run_gamev_15, Type::getInt32Ty(ctx));
        builder.CreateCondBr (run_gamev_20, run_game_bb_22, run_game_bb_26);

        // run_game_bb_22
        builder.SetInsertPoint (run_game_bb_22);
        builder.CreateCall (simPutPixel_func, {run_gamev_9, run_gamev_21, ConstantInt::get (Type::getInt32Ty(ctx), 8388736)});
        Value* run_gamev_23 = builder.CreateCall(count_alive_neighbors_func, {run_gamev_9, run_gamev_21, run_gamev_5});
        Value* run_gamev_24 = builder.CreateAnd (run_gamev_23, ConstantInt::get (Type::getInt32Ty(ctx), -2));
        Value* run_gamev_25 = builder.CreateICmp (CmpInst::ICMP_EQ, run_gamev_24, ConstantInt::get (Type::getInt32Ty(ctx), 2));
        builder.CreateBr (run_game_bb_29);

        // run_game_bb_26
        builder.SetInsertPoint (run_game_bb_26);
        builder.CreateCall (simPutPixel_func, {run_gamev_8, run_gamev_21, ConstantInt::get (Type::getInt32Ty(ctx), 16777215)});
        Value* run_gamev_27 = builder.CreateCall(count_alive_neighbors_func, {run_gamev_8, run_gamev_21, run_gamev_5});
        Value* run_gamev_28 = builder.CreateICmp (CmpInst::ICMP_EQ, run_gamev_27, ConstantInt::get (Type::getInt32Ty(ctx), 3));
        builder.CreateBr (run_game_bb_29);

        // run_game_bb_29
        builder.SetInsertPoint (run_game_bb_29);
        PHINode* run_gamev_30 = builder.CreatePHI (Type::getInt1Ty(ctx), 2);
        Value* run_gamev_31 = builder.CreateZExt (run_gamev_30, Type::getInt32Ty(ctx));
        Value* run_gamev_32 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), run_gamev_4, run_gamev_17);
        builder.CreateStore (run_gamev_31, run_gamev_32);
        Value* run_gamev_33 = builder.CreateAdd (run_gamev_15, ConstantInt::get (Type::getInt64Ty(ctx), 1), "", true, true);
        Value* run_gamev_34 = builder.CreateICmp (CmpInst::ICMP_EQ, run_gamev_33, ConstantInt::get (Type::getInt64Ty(ctx), 256));
        builder.CreateCondBr (run_gamev_34, run_game_bb_11, run_game_bb_14);  

        // adding phi args
        run_gamev_4->addIncoming (run_gamev_1, run_game_entry_bb);
        run_gamev_4->addIncoming (run_gamev_5, run_game_bb_10);
        run_gamev_5->addIncoming (run_gamev_0, run_game_entry_bb);
        run_gamev_5->addIncoming (run_gamev_4, run_game_bb_10);
        run_gamev_7->addIncoming (ConstantInt::get (Type::getInt64Ty(ctx), 0), run_game_bb_3);
        run_gamev_7->addIncoming (run_gamev_12, run_game_bb_11);
        run_gamev_15->addIncoming (ConstantInt::get (Type::getInt64Ty(ctx), 0), run_game_bb_6);
        run_gamev_15->addIncoming (run_gamev_33, run_game_bb_29);
        run_gamev_30->addIncoming (run_gamev_25, run_game_bb_22);
        run_gamev_30->addIncoming (run_gamev_28, run_game_bb_26);

    //----------------------------------------------------------------------------------------------------------
    // Count_alive_neighbors_func
    //----------------------------------------------------------------------------------------------------------
        // args
        Function::arg_iterator can_func_args_it = count_alive_neighbors_func->arg_begin();
        Value* canv_0 = can_func_args_it;
        Value* canv_1 = ++can_func_args_it;
        Value* canv_2 = ++can_func_args_it;

        // basic blocks
        BasicBlock* can_entry_bb = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb7 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb9 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb11 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb19 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb22 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb31 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb35 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb44 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb47 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb49 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb58 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb62 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb71 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb75 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb77 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb86 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb89 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb98 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb102 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);

//--------------------------------------------------------------------------------------------------------------  
// Dump LLVM IR
//--------------------------------------------------------------------------------------------------------------  
    module->print(outs(), nullptr);

//--------------------------------------------------------------------------------------------------------------  
// LLVM IR Interpreter
//--------------------------------------------------------------------------------------------------------------  
    outs() << "[EE] Run\n";
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine* ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    
    ee->InstallLazyFunctionCreator([&](const std::string& fnName) -> void* {
        if (fnName == "simRand") {
            return reinterpret_cast<void *>(simRand);
        }
        if (fnName == "simPutPixel") {
            return reinterpret_cast<void *>(simPutPixel);
        }
        if (fnName == "simFlush") {
            return reinterpret_cast<void *>(simFlush);
        }

        return nullptr;
    });

    ee->finalizeObject();

    simInit();

    ArrayRef<GenericValue> noargs;
    GenericValue v = ee->runFunction(app_func, noargs);
    outs() << "[EE] Result: " << v.IntVal << "\n";

    simExit();

    return 0;
}