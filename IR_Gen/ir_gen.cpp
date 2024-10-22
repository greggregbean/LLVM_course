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
    
    ArrayRef<Type*> run_game_func_args_ref ({Type::getInt32PtrTy(ctx), Type::getInt32PtrTy(ctx)});
    FunctionType* run_game_func_type = FunctionType::get (Type::getVoidTy(ctx), run_game_func_args_ref, false);
    Function* run_game_func = Function::Create (run_game_func_type, Function::ExternalLinkage, "run_game", module);

    FunctionType* app_func_type = FunctionType::get (Type::getVoidTy(ctx), false);
    Function* app_func = Function::Create (app_func_type, Function::ExternalLinkage, "app", module);

    //----------------------------------------------------------------------------------------------------------
    // App_func
    //----------------------------------------------------------------------------------------------------------
        // basic blocks
        BasicBlock* app_entry_bb = BasicBlock::Create (ctx, "", app_func);
        BasicBlock* start_config_bb = BasicBlock::Create (ctx, "", app_func);
        BasicBlock* call_run_game_bb = BasicBlock::Create (ctx, "", app_func);

        // entry_app_bb
        builder.SetInsertPoint (app_entry_bb);
        Value* appv_1 = builder.CreateAlloca (ArrayType::getScalarSizeInBits (Type::getIntNTy(ctx, 32), SIM_X_SIZE * SIM_Y_SIZE));
        Value* appv_2 = builder.CreateAlloca (ArrayType::getInt64Ty (Type::getIntNTy(ctx, 32), SIM_X_SIZE * SIM_Y_SIZE));
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
        Value* appv_19 = builder.CreateInBoundsGEP (ArrayType::get (Type::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE), appv_2, {ConstantInt::get (Type::getInt64Ty(ctx), 0), ConstantInt::get (Type::getInt64Ty(ctx), 0)});
        Value* appv_20 = builder.CreateInBoundsGEP (ArrayType::get (Type::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE), appv_1, {ConstantInt::get (Type::getInt64Ty(ctx), 0), ConstantInt::get (Type::getInt64Ty(ctx), 0)});
        builder.CreateCall (run_game_func, {appv_20, appv_19});
        builder.CreateUnreachable();

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
        BasicBlock* can_bb_7 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_9 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_11 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_19 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_22 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_31 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_35 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_44 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_47 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_49 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_58 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_62 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_71 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_75 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_77 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_86 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_89 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_98 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);
        BasicBlock* can_bb_102 = BasicBlock::Create (ctx, "", count_alive_neighbors_func);

        // can_entry_bb
        builder.SetInsertPoint (can_entry_bb);
        Value* canv_4 = builder.CreateAdd (canv_0, ConstantInt::get (Type::getInt32Ty(ctx), -1), "", false, true);
        Value* canv_5 = builder.CreateAdd (canv_1, ConstantInt::get (Type::getInt32Ty(ctx), -1), "", false, true);
        Value* canv_6 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_4, ConstantInt::get (Type::getInt32Ty(ctx), 511));
        builder.CreateCondBr (canv_6, can_bb_44, can_bb_9);

        // can_bb_7
        builder.SetInsertPoint (can_bb_7);
        PHINode* canv_8 = builder.CreatePHI (Type::getInt32Ty(ctx), 3);
        builder.CreateRet (canv_8);

        // can_bb_9
        builder.SetInsertPoint (can_bb_9);
        Value* canv_10 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_5, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_10, can_bb_19, can_bb_11);

        // can_bb_11
        builder.SetInsertPoint (can_bb_11);
        Value* canv_12 = builder.CreateShl (canv_5, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_13 = builder.CreateAdd (canv_12, canv_4, "", true, true);
        Value* canv_14 = builder.CreateZExt (canv_13, Type::getInt64Ty(ctx));
        Value* canv_15 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_14);
        Value* canv_16 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_15);
        Value* canv_17 = builder.CreateICmp (CmpInst::ICMP_NE, canv_16, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_18 = builder.CreateZExt (canv_17, Type::getInt32Ty(ctx));
        builder.CreateBr (can_bb_19);

        // can_bb_19
        builder.SetInsertPoint (can_bb_19);
        PHINode* canv_20 = builder.CreatePHI (Type::getInt32Ty(ctx), 2);
        Value* canv_21 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_1, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_21, can_bb_31, can_bb_22);
        
        // can_bb_22
        builder.SetInsertPoint (can_bb_22);
        Value* canv_23 = builder.CreateShl (canv_1, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_24 = builder.CreateAdd (canv_23, canv_4, "", true, true);
        Value* canv_25 = builder.CreateZExt (canv_24, Type::getInt64Ty(ctx));
        Value* canv_26 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_25);
        Value* canv_27 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_26);
        Value* canv_28 = builder.CreateICmp (CmpInst::ICMP_NE, canv_27, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_29 = builder.CreateZExt (canv_28, Type::getInt32Ty(ctx));
        Value* canv_30 = builder.CreateAdd (canv_20, canv_29, "", true, true);
        builder.CreateBr (can_bb_31);  

        // can_bb_31
        builder.SetInsertPoint (can_bb_31);
        PHINode* canv_32 = builder.CreatePHI (Type::getInt32Ty(ctx), 2);
        Value* canv_33 = builder.CreateAdd (canv_1, ConstantInt::get (Type::getInt32Ty(ctx), 1), "", false, true);
        Value* canv_34 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_33, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_34, can_bb_44, can_bb_35);

        // can_bb_35              
        builder.SetInsertPoint (can_bb_35);
        Value* canv_36 = builder.CreateShl (canv_33, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_37 = builder.CreateAdd (canv_36, canv_4, "", true, true);
        Value* canv_38 = builder.CreateZExt (canv_37, Type::getInt64Ty(ctx));
        Value* canv_39 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_38);
        Value* canv_40 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_39);
        Value* canv_41 = builder.CreateICmp (CmpInst::ICMP_NE, canv_40, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_42 = builder.CreateZExt (canv_41, Type::getInt32Ty(ctx));
        Value* canv_43 = builder.CreateAdd (canv_32, canv_42, "", true, true);
        builder.CreateBr (can_bb_44);

        // can_bb_44
        builder.SetInsertPoint (can_bb_44);
        PHINode* canv_45 = builder.CreatePHI (Type::getInt32Ty(ctx), 3);
        Value* canv_46 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_0, ConstantInt::get (Type::getInt32Ty(ctx), 511));
        builder.CreateCondBr (canv_46, can_bb_71, can_bb_47);  

        // can_bb_47      
        builder.SetInsertPoint (can_bb_47);
        Value* canv_48 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_5, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_48, can_bb_58, can_bb_49); 

        // can_bb_49              
        builder.SetInsertPoint (can_bb_49);
        Value* canv_50 = builder.CreateShl (canv_5, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_51 = builder.CreateAdd (canv_50, canv_0, "", true, true);
        Value* canv_52 = builder.CreateZExt (canv_51, Type::getInt64Ty(ctx));
        Value* canv_53 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_52);
        Value* canv_54 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_53);
        Value* canv_55 = builder.CreateICmp (CmpInst::ICMP_NE, canv_54, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_56 = builder.CreateZExt (canv_55, Type::getInt32Ty(ctx));
        Value* canv_57 = builder.CreateAdd (canv_45, canv_56, "", true, true);
        builder.CreateBr (can_bb_58);

        // can_bb_58
        builder.SetInsertPoint (can_bb_58);
        PHINode* canv_59 = builder.CreatePHI (Type::getInt32Ty(ctx), 2);
        Value* canv_60 = builder.CreateAdd (canv_1, ConstantInt::get (Type::getInt32Ty(ctx), 1), "", false, true);
        Value* canv_61 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_60, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_61, can_bb_71, can_bb_62);

        // can_bb_62              
        builder.SetInsertPoint (can_bb_62);
        Value* canv_63 = builder.CreateShl (canv_60, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_64 = builder.CreateAdd (canv_63, canv_0, "", true, true);
        Value* canv_65 = builder.CreateZExt (canv_64, Type::getInt64Ty(ctx));
        Value* canv_66 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_65);
        Value* canv_67 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_66);
        Value* canv_68 = builder.CreateICmp (CmpInst::ICMP_NE, canv_67, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_69 = builder.CreateZExt (canv_68, Type::getInt32Ty(ctx));
        Value* canv_70 = builder.CreateAdd (canv_59, canv_69, "", true, true);
        builder.CreateBr (can_bb_71);

        // can_bb_71
        builder.SetInsertPoint (can_bb_71);
        PHINode* canv_72 = builder.CreatePHI (Type::getInt32Ty(ctx), 3);
        Value* canv_73 = builder.CreateAdd (canv_0, ConstantInt::get (Type::getInt32Ty(ctx), 1), "", false, true);
        Value* canv_74 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_73, ConstantInt::get (Type::getInt32Ty(ctx), 511));
        builder.CreateCondBr (canv_74, can_bb_7, can_bb_75);

        // can_bb_75      
        builder.SetInsertPoint (can_bb_75);
        Value* canv_76 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_5, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_76, can_bb_86, can_bb_77); 

        // can_bb_77              
        builder.SetInsertPoint (can_bb_77);
        Value* canv_78 = builder.CreateShl (canv_5, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_79 = builder.CreateAdd (canv_78, canv_73, "", true, true);
        Value* canv_80 = builder.CreateZExt (canv_79, Type::getInt64Ty(ctx));
        Value* canv_81 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_80);
        Value* canv_82 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_81);
        Value* canv_83 = builder.CreateICmp (CmpInst::ICMP_NE, canv_82, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_84 = builder.CreateZExt (canv_83, Type::getInt32Ty(ctx));
        Value* canv_85 = builder.CreateAdd (canv_72, canv_84, "", true, true);
        builder.CreateBr (can_bb_86);

        // can_bb_86
        builder.SetInsertPoint (can_bb_86);
        PHINode* canv_87 = builder.CreatePHI (Type::getInt32Ty(ctx), 2);
        Value* canv_88 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_1, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_88, can_bb_98, can_bb_89);

        // can_bb_89              
        builder.SetInsertPoint (can_bb_89);
        Value* canv_90 = builder.CreateShl (canv_1, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_91 = builder.CreateAdd (canv_90, canv_73, "", true, true);
        Value* canv_92 = builder.CreateZExt (canv_91, Type::getInt64Ty(ctx));
        Value* canv_93 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_92);
        Value* canv_94 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_93);
        Value* canv_95 = builder.CreateICmp (CmpInst::ICMP_NE, canv_94, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_96 = builder.CreateZExt (canv_95, Type::getInt32Ty(ctx));
        Value* canv_97 = builder.CreateAdd (canv_87, canv_96, "", true, true);
        builder.CreateBr (can_bb_98);

        // can_bb_98
        builder.SetInsertPoint (can_bb_98);
        PHINode* canv_99 = builder.CreatePHI (Type::getInt32Ty(ctx), 2);
        Value* canv_100 = builder.CreateAdd (canv_1, ConstantInt::get (Type::getInt32Ty(ctx), 1), "", false, true);
        Value* canv_101 = builder.CreateICmp (CmpInst::ICMP_UGT, canv_100, ConstantInt::get (Type::getInt32Ty(ctx), 255));
        builder.CreateCondBr (canv_101, can_bb_7, can_bb_102);

        // can_bb_102              
        builder.SetInsertPoint (can_bb_102);
        Value* canv_103 = builder.CreateShl (canv_100, ConstantInt::get (Type::getInt32Ty(ctx), 9), "", true, true);
        Value* canv_104 = builder.CreateAdd (canv_103, canv_73, "", true, true);
        Value* canv_105 = builder.CreateZExt (canv_104, Type::getInt64Ty(ctx));
        Value* canv_106 = builder.CreateInBoundsGEP (Type::getInt32Ty(ctx), canv_2, canv_105);
        Value* canv_107 = builder.CreateLoad (Type::getInt32Ty(ctx), canv_106);
        Value* canv_108 = builder.CreateICmp (CmpInst::ICMP_NE, canv_107, ConstantInt::get (Type::getInt32Ty(ctx), 0));
        Value* canv_109 = builder.CreateZExt (canv_108, Type::getInt32Ty(ctx));
        Value* canv_110 = builder.CreateAdd (canv_99, canv_109, "", true, true);
        builder.CreateBr (can_bb_7);

        // adding phi args
        canv_8->addIncoming (canv_72, can_bb_71);
        canv_8->addIncoming (canv_99, can_bb_98);
        canv_8->addIncoming (canv_110, can_bb_102);

        canv_20->addIncoming (ConstantInt::get (Type::getInt32Ty(ctx), 0), can_bb_9);
        canv_20->addIncoming (canv_18, can_bb_11);

        canv_32->addIncoming (canv_20, can_bb_19);
        canv_32->addIncoming (canv_30, can_bb_22);

        canv_45->addIncoming (ConstantInt::get (Type::getInt32Ty(ctx), 0), can_entry_bb);
        canv_45->addIncoming (canv_32, can_bb_31);
        canv_45->addIncoming (canv_43, can_bb_35);

        canv_59->addIncoming (canv_45, can_bb_47);
        canv_59->addIncoming (canv_57, can_bb_49);

        canv_72->addIncoming (canv_45, can_bb_44);
        canv_72->addIncoming (canv_59, can_bb_58);
        canv_72->addIncoming (canv_70, can_bb_62);

        canv_87->addIncoming (canv_72, can_bb_75);
        canv_87->addIncoming (canv_85, can_bb_77);

        canv_99->addIncoming (canv_87, can_bb_86);
        canv_99->addIncoming (canv_97, can_bb_89);

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