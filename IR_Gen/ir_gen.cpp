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
        
    ArrayRef<Type*> run_game_func_args_ref ({IntegerType::getInt32PtrTy(ctx), IntegerType::getInt32PtrTy(ctx)});
    FunctionType* run_game_func_type = FunctionType::get (Type::getVoidTy(ctx), run_game_func_args_ref, false);
    FunctionCallee run_game_func = module->getOrInsertFunction ("run_game", run_game_func_type);

    FunctionType* simRand_func_type = FunctionType::get (Type::getInt32Ty(ctx), false);
    FunctionCallee simRand_func = module->getOrInsertFunction ("simRand", simRand_func_type);

    ArrayRef<Type*> simPutPixel_func_args_ref = {Type::getInt32Ty(ctx), Type::getInt32Ty(ctx), Type::getInt32Ty(ctx)};
    FunctionType* simPutPixel_func_type = FunctionType::get (Type::getVoidTy(ctx), simPutPixel_func_args_ref, false);
    FunctionCallee simPutPixel_func = module->getOrInsertFunction("simPutPixel", simPutPixel_func_type);

    FunctionType* simFlush_func_type = FunctionType::get(Type::getVoidTy(ctx), false);
    FunctionCallee simFlush_func = module->getOrInsertFunction("simFlush", simFlush_func_type);

    //----------------------------------------------------------------------------------------------------------
    // basic blocks
    //----------------------------------------------------------------------------------------------------------
    BasicBlock* entry_app_bb = BasicBlock::Create (ctx, "entry_app_bb", app_func);
    BasicBlock* start_config_bb = BasicBlock::Create (ctx, "start_config_bb", app_func);
    BasicBlock* run_game_bb = BasicBlock::Create (ctx, "run_game_bb", app_func);

    //----------------------------------------------------------------------------------------------------------
    // entry_app_bb
    //----------------------------------------------------------------------------------------------------------
    builder.SetInsertPoint (entry_app_bb);

    Value* appv_1 = builder.CreateAlloca (ArrayType::get (IntegerType::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE));
    Value* appv_2 = builder.CreateAlloca (ArrayType::get (IntegerType::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE)); 

    Value* br_start_config_bb = builder.CreateBr (start_config_bb);

    //----------------------------------------------------------------------------------------------------------
    // start_config_bb:
    //----------------------------------------------------------------------------------------------------------
    builder.SetInsertPoint (start_config_bb);

    PHINode* appv_6 = builder.CreatePHI (IntegerType::getInt32Ty(ctx), 2);
    Value* appv_7 = builder.CreateCall (simRand_func);
    Value* appv_8 = builder.CreateSRem (appv_7, ConstantInt::get (IntegerType::getInt32Ty(ctx), 512));
    Value* appv_9 = builder.CreateCall (simRand_func);
    Value* appv_10 = builder.CreateSRem (appv_9, ConstantInt::get (IntegerType::getInt32Ty(ctx), 256));
    Value* appv_11 = builder.CreateShl (appv_10, ConstantInt::get (IntegerType::getInt32Ty(ctx), 9), "", false, true);
    Value* appv_12 = builder.CreateAdd (appv_11, appv_8, "", false, true);
    Value* appv_13 = builder.CreateSExt (appv_12, IntegerType::getInt64Ty(ctx));
    Value* appv_14 = builder.CreateInBoundsGEP (ArrayType::get (IntegerType::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE), 
                                                appv_1, 
                                                {ConstantInt::get (IntegerType::getInt64Ty(ctx), 0), appv_13});
    builder.CreateStore (ConstantInt::get (IntegerType::getInt32Ty(ctx), 1), appv_14);
    Value* appv_15 = builder.CreateInBoundsGEP (ArrayType::get (IntegerType::getInt32Ty(ctx), SIM_X_SIZE * SIM_Y_SIZE), 
                                                appv_2, 
                                                {ConstantInt::get (IntegerType::getInt64Ty(ctx), 0), appv_13});
    builder.CreateStore (ConstantInt::get (IntegerType::getInt32Ty(ctx), 1), appv_15);
    Value* appv_16 = builder.CreateAdd (appv_6, ConstantInt::get (IntegerType::getInt32Ty(ctx), 1), "", true, true);
    Value* appv_17 = builder.CreateICmp (CmpInst::ICMP_EQ, appv_16, ConstantInt::get (IntegerType::getInt32Ty(ctx), 100000));

    // setting cond branch
    builder.CreateCondBr (appv_17, run_game_bb, start_config_bb);

    // adding phi args
    appv_6->addIncoming (ConstantInt::get (IntegerType::getInt32Ty(ctx), 0), entry_app_bb);
    appv_6->addIncoming (appv_16, start_config_bb);

    //builder.CreateBr (run_game_bb);

    //----------------------------------------------------------------------------------------------------------
    // run_game_bb:
    //----------------------------------------------------------------------------------------------------------
    builder.SetInsertPoint (run_game_bb);
    //builder.CreateCall(run_game_func);
    builder.CreateRetVoid();

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