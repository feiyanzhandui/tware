#ifndef COMPILER_H
#define COMPILER_H

#include <cxxabi.h>
#include <llvm/PassManager.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Transforms/Vectorize.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <sstream>
#include <string>
#include <vector>

#include "compiler/Executable.h"
#include "compiler/LlvmIr.h"

using abi::__cxa_demangle;
using llvm::createBasicAliasAnalysisPass;
using llvm::createCFGSimplificationPass;
using llvm::createFunctionInliningPass;
using llvm::createGVNPass;
using llvm::createInstructionCombiningPass;
using llvm::createPromoteMemoryToRegisterPass;
using llvm::createReassociatePass;
using llvm::DiagnosticHandlerFunction;//
using llvm::EngineBuilder;
using llvm::ExecutionEngine;
using llvm::Function;
using llvm::FunctionPassManager;
using llvm::getGlobalContext;
using llvm::Linker;
using llvm::LLVMContext;
using llvm::MemoryBuffer;
using llvm::Module;
//using llvm::ParseIR;
using llvm::PassManager;
using llvm::PassManagerBuilder;
using llvm::parseIR;//
using llvm::raw_string_ostream;
using llvm::SMDiagnostic;
using llvm::Triple;
using llvm::CodeGenOpt::Level;
using llvm::sys::getHostCPUName;
using std::ostringstream;
using std::string;
using std::to_string;
using std::vector;

using namespace llvm;
//#include "llvm/IR/DataLayout.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/LegacyPassManager.h"
//#include "llvm/IR/Verifier.h"
//#include "llvm/Support/TargetSelect.h"
//#include "llvm/ADT/STLExtras.h"

#include <iostream>
using namespace std;

namespace tupleware {
class Compiler {
public:
    //static LlvmIr getLlvm(string body) {
    static string getLlvm(string body) {
        string cmd = "echo '" + body + "' |"
                   //+ " clang++ -cc1 -xc++ -O0 -std=c++14 -stdlib=libstdc++"
                   + " clang++-3.6 -cc1 -xc++ -O0 -std=c++14 -stdlib=libstdc++"
                   + " -fno-use-cxa-atexit -fcxx-exceptions"
                   + " -S -emit-llvm"
                   + " -I/usr/include/"
                   + " -I/usr/include/clang/3.6/include/"
                   + " -I/usr/include/c++/4.9/"
                   + " -I/usr/include/c++/4.9/backward/"
                   + " -I/usr/include/x86_64-linux-gnu/"
                   + " -I/usr/include/x86_64-linux-gnu/c++/4.9/"
                   + " -I/usr/include/x86_64-linux-gnu/c++/4.9/bits/"
                   + " -I/usr/lib/gcc/x86_64-linux-gnu/4.9/include/"
                   + " -I/usr/lib/gcc/x86_64-linux-gnu/4.9/include-fixed/"
                   + " -I/usr/local/include/"
                   + " -I" + string(getenv("TUPLEWARE_HOME")) + "/src/"
                   + " -o /dev/stdout 2> /dev/stdout";

        FILE *file = popen(cmd.c_str(), "r");
        if (!file) {
            //error
        }

        ostringstream llvm;
        constexpr size_t LINE_SIZE = 1024;
        char line[LINE_SIZE];
        while (fgets(line, LINE_SIZE, file))
            llvm << line;

        int stat = pclose(file);
        //if (stat/256 == 1) {
        //    Logging::fatal(__FILE__,__LINE__, "UDFs did not compile\n\n"
        //        + llvm.str() + "\n\n");
        //}

        /*for (Module::iterator itr = mod->begin(); itr != mod->end(); ++itr) {
            Function &fun = *itr;
            string name = fun.getName();
            string body;
            raw_string_ostream buf(body);
            fun.print(buf);
            if (name.compare(0, 4, "_Z2f") == 0) {
                m_udfMsgs[i]->set_llvm(body);
                i++;
            }

        //LLVMContext &ctx = getGlobalContext();
        //SMDiagnostic err;
        //Module *mod = ParseIR(MemoryBuffer::getMemBuffer(llvm), err, ctx);
        //Function &fun = *(mod->begin());
        //string name = fun.getName();
        //Logging::info(name);
        //string s = __cxa_demangle(fun.getName().str().c_str(), NULL, NULL, NULL);

        //Function *f = Function::Create(fun.getFunctionType(), Function::ExternalLinkage,
        //                  "f_" + name, mod);

        //CloneFunctionInto(f, temp, NULL, false, NULL, NULL, NULL, NULL, NULL);

        //fun.eraseFromParent();
        //Function *fun = mod->getFunction(name);
        //    string body;
        //    raw_string_ostream buf(body);
        //    fun.print(buf);
//cout << body << endl;
        //    if (name.compare(0, 4, "_Z2f") == 0) {
//cout << body << endl;
//                m_udfMsgs[i]->set_llvm(body);
//                i++;
//            }
        }*/

        return llvm.str();
    }

    static string link(string body, string fun) {
        LLVMContext &ctx = getGlobalContext();
        SMDiagnostic err;
        string *serr = new string();
        //Module *dest = ParseIR(MemoryBuffer::getMemBuffer(body), err, ctx);
        auto x1 = MemoryBuffer::getMemBuffer(body)->getMemBufferRef();
        auto dest = parseIR(x1, err, ctx);
        //Module *src = ParseIR(MemoryBuffer::getMemBuffer(fun), err, ctx);
        auto x2 = MemoryBuffer::getMemBuffer(fun)->getMemBufferRef();
        auto src = parseIR(x2, err, ctx);
        //Linker::LinkModules(dest, src, 0, serr);
        DiagnosticHandlerFunction dhf;
        Linker::LinkModules(dest.get(), src.get(), dhf);
        string llvm;
        raw_string_ostream buff(llvm);
        dest->print(buff, NULL);
        return llvm;
    }

    static string optimize(string llvm) {
        LLVMContext &ctx = getGlobalContext();
        SMDiagnostic err;
        //Module *ir = ParseIR(MemoryBuffer::getMemBuffer(llvm), err, ctx);
        auto x = MemoryBuffer::getMemBuffer(llvm)->getMemBufferRef();
        auto ir = parseIR(x, err, ctx);

        FunctionPassManager *fpm = new FunctionPassManager(ir.get());
        PassManager *pm = new PassManager();

        int optLevel = 3;
        int sizeLevel = 0;
        PassManagerBuilder builder;
        builder.OptLevel = optLevel;
        builder.SizeLevel = sizeLevel;
        builder.Inliner = createFunctionInliningPass(optLevel, sizeLevel);
        builder.DisableUnitAtATime = false;
        builder.DisableUnrollLoops = false;
        //builder.BBVectorize = true;
        builder.LoopVectorize = true;
        builder.SLPVectorize = true;
        //builder.populateFunctionPassManager(*fpm);
        builder.populateModulePassManager(*pm);

/*
    FPM = new FunctionPassManager(m);

#ifdef __has_feature
#   if __has_feature(address_sanitizer)
    FPM->add(createAddressSanitizerFunctionPass());
#   endif
#   if __has_feature(memory_sanitizer)
    FPM->add(llvm::createMemorySanitizerPass(true));
#   endif
#endif
#ifndef LLVM37
    jl_TargetMachine->addAnalysisPasses(*FPM);
#endif
    FPM->add(createTypeBasedAliasAnalysisPass());
    if (jl_options.opt_level>=1)
        FPM->add(createBasicAliasAnalysisPass());
    // list of passes from vmkit
    FPM->add(createCFGSimplificationPass()); // Clean up disgusting code
    FPM->add(createPromoteMemoryToRegisterPass());// Kill useless allocas

#ifndef INSTCOMBINE_BUG
    FPM->add(createInstructionCombiningPass()); // Cleanup for scalarrepl.
#endif
    FPM->add(createScalarReplAggregatesPass()); // Break up aggregate allocas
#ifndef INSTCOMBINE_BUG
    FPM->add(createInstructionCombiningPass()); // Cleanup for scalarrepl.
#endif
    FPM->add(createJumpThreadingPass());        // Thread jumps.
    // NOTE: CFG simp passes after this point seem to hurt native codegen.
    // See issue #6112. Should be re-evaluated when we switch to MCJIT.
    //FPM->add(createCFGSimplificationPass());    // Merge & remove BBs
#ifndef INSTCOMBINE_BUG
    FPM->add(createInstructionCombiningPass()); // Combine silly seq's
#endif

    //FPM->add(createCFGSimplificationPass());    // Merge & remove BBs
    FPM->add(createReassociatePass());          // Reassociate expressions

    // this has the potential to make some things a bit slower
    //FPM->add(createBBVectorizePass());

    FPM->add(createEarlyCSEPass()); //// ****

    FPM->add(createLoopIdiomPass()); //// ****
    FPM->add(createLoopRotatePass());           // Rotate loops.
    // LoopRotate strips metadata from terminator, so run LowerSIMD afterwards
    FPM->add(createLowerSimdLoopPass());        // Annotate loop marked with "simdloop" as LLVM parallel loop
    FPM->add(createLICMPass());                 // Hoist loop invariants
    FPM->add(createLoopUnswitchPass());         // Unswitch loops.
    // Subsequent passes not stripping metadata from terminator
#ifndef INSTCOMBINE_BUG
    FPM->add(createInstructionCombiningPass());
#endif
    FPM->add(createIndVarSimplifyPass());       // Canonicalize indvars
    FPM->add(createLoopDeletionPass());         // Delete dead loops
#if LLVM35
    FPM->add(createSimpleLoopUnrollPass());     // Unroll small loops
#else
    FPM->add(createLoopUnrollPass());           // Unroll small loops
#endif
#if !LLVM35 && !defined(INSTCOMBINE_BUG)
    FPM->add(createLoopVectorizePass());        // Vectorize loops
#endif
    //FPM->add(createLoopStrengthReducePass());   // (jwb added)

#ifndef INSTCOMBINE_BUG
    FPM->add(createInstructionCombiningPass()); // Clean up after the unroller
#endif
    FPM->add(createGVNPass());                  // Remove redundancies
    //FPM->add(createMemCpyOptPass());            // Remove memcpy / form memset
    FPM->add(createSCCPPass());                 // Constant prop with SCCP

    // Run instcombine after redundancy elimination to exploit opportunities
    // opened up by them.
    FPM->add(createSinkingPass()); ////////////// ****
    FPM->add(createInstructionSimplifierPass());///////// ****
#ifndef INSTCOMBINE_BUG
    FPM->add(createInstructionCombiningPass());
#endif
    FPM->add(createJumpThreadingPass());         // Thread jumps
    FPM->add(createDeadStoreEliminationPass());  // Delete dead stores
#if !defined(INSTCOMBINE_BUG)
        FPM->add(createSLPVectorizerPass());     // Vectorize straight-line code
#endif

    FPM->add(createAggressiveDCEPass());         // Delete dead instructions
#if !defined(INSTCOMBINE_BUG)
        FPM->add(createInstructionCombiningPass());   // Clean up after SLP loop vectorizer
#endif
#if LLVM35
    FPM->add(createLoopVectorizePass());         // Vectorize loops
    FPM->add(createInstructionCombiningPass());  // Clean up after loop vectorizer
#endif
    //FPM->add(createCFGSimplificationPass());     // Merge & remove BBs

    FPM->doInitialization();
}

#if defined(_CPU_X86_64_)
    const char *const argv[] = {"", "-disable-copyprop"}; // llvm bug 21743
    cl::ParseCommandLineOptions(sizeof(argv)/sizeof(argv[0]), argv, "disable-copyprop\n");
#endif
#if defined(_CPU_PPC_) || defined(_CPU_PPC64_)
    imaging_mode = true;
#endif

#if LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR <= 3
    llvm::DisablePrettyStackTrace = true;
#endif

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

    Module *m, *engine_module;

    TargetOptions options = TargetOptions();
    options.JITEmitDebugInfo = true;
#ifndef LLVM37
    options.NoFramePointerElim = true;
#endif
#ifndef LLVM34
    options.NoFramePointerElimNonLeaf = true;
#endif
*/
//fpm->add(createTypeBasedAliasAnalysisPass());
//fpm->add(createBasicAliasAnalysisPass());
//fpm->add(createCFGSimplificationPass());
//fpm->add(createPromoteMemoryToRegisterPass());
//    fpm->add(createInstructionCombiningPass());
//fpm->add(createScalarReplAggregatesPass());
//    fpm->add(createInstructionCombiningPass());
//    fpm->add(createJumpThreadingPass());
//    fpm->add(createInstructionCombiningPass());
//fpm->add(createReassociatePass());
//fpm->add(createEarlyCSEPass());
//fpm->add(createLoopIdiomPass());
//fpm->add(createLoopRotatePass());
//createLowerSIMDLoopPass
//fpm->add(createLICMPass());
//fpm->add(createLoopUnswitchPass());
//    fpm->add(createInstructionCombiningPass());
//fpm->add(createIndVarSimplifyPass());
//fpm->add(createLoopDeletionPass());
//    fpm->add(createInstructionCombiningPass());
//fpm->add(createGVNPass());
//fpm->add(createSCCPPass());
//fpm->add(createSinkingPass());
//fpm->add(createInstructionSimplifierPass());
//    fpm->add(createInstructionCombiningPass());
//fpm->add(createJumpThreadingPass());
//fpm->add(createDeadStoreEliminationPass());
//fpm->add(createSLPVectorizerPass());
//fpm->add(createAggressiveDCEPass());
//    fpm->add(createInstructionCombiningPass());
//fpm->add(createLoopVectorizePass());
//fpm->add(createInstructionCombiningPass());

    //fpm->add(createLoadCombinePass());
    //fpm->add(createLoopInstSimplifyPass());
    //fpm->add(createLoopDistributePass());
    //fpm->add(createPartiallyInlineLibCallsPass());
    //fpm->add(createSampleProfileLoaderPass());
    //fpm->add(createScalarizerPass());
    //fpm->add(createAddDiscriminatorsPass());
    //fpm->add(createSpeculativeExecutionPass());
    //fpm->add(createStraightLineStrengthReducePass());
    //fpm->add(createPlaceSafepointsPass());
    //fpm->add(createRewriteStatepointsForGCPass());
    //fpm->add(createNaryReassociatePass());
    //fpm->add(createFloat2IntPass());
    //fpm->add(createLowerExpectIntrinsicPass());
    //fpm->add(createCorrelatedValuePropagationPass());
    //fpm->add(createLowerAtomicPass());
    //fpm->add(createInstructionNamerPass());
    //fpm->add(createConstantHoistingPass());
    //fpm->add(createMemCpyOptPass());
    //fpm->add(createMergedLoadStoreMotionPass());
    //fpm->add(createLCSSAPass());
    //fpm->add(createLowerInvokePass());
    //fpm->add(createLowerSwitchPass());
    //fpm->add(createTailCallEliminationPass());
    //fpm->add(createLoopSimplifyPass());
    //fpm->add(createBreakCriticalEdgesPass());
    //fpm->add(createStructurizeCFGPass());
    //fpm->add(createFlattenCFGPass());
    //fpm->add(createCFGSimplificationPass());
    //fpm->add(createReassociatePass());
    //fpm->add(createDemoteRegisterToMemoryPass());
    //fpm->add(createPromoteMemoryToRegisterPass());
    //fpm->add(createLoopRerollPass());
    //fpm->add(createSimpleLoopUnrollPass());
    //fpm->add(createGlobalMergePass());
    //fpm->add(createLoopStrengthReducePass());
    //fpm->add(createLoopInterchangePass());
    //fpm->add(createInductiveRangeCheckEliminationPass());
    //fpm->add(createSROAPass());
    //fpm->add(createBitTrackingDCEPass());
    //fpm->add(createDeadCodeEliminationPass());
    //fpm->add(createDeadInstEliminationPass());
    //fpm->add(createAlignmentFromAssumptionsPass());
    //fpm->add(createConstantPropagationPass());

//fpm->add(createSampleProfileLoaderPass());
//fpm->add(createScalarizerPass());
//fpm->add(createAddDiscriminatorsPass());
    //fpm->add(createLoadCombinePass());
//fpm->add(createLoopInstSimplifyPass());
//fpm->add(createLoopDistributePass());
//fpm->add(createPlaceSafepointsPass());
//fpm->add(createRewriteStatepointsForGCPass());
//fpm->add(createNaryReassociatePass());
//fpm->add(createSpeculativeExecutionPass());
//fpm->add(createStraightLineStrengthReducePass());
//fpm->add(createFloat2IntPass());
//fpm->add(createPartiallyInlineLibCallsPass());
//fpm->add(createLowerExpectIntrinsicPass());
//fpm->add(createCorrelatedValuePropagationPass());
//fpm->add(createLowerAtomicPass());
//fpm->add(createInstructionNamerPass());
//fpm->add(createLowerInvokePass());
//fpm->add(createLowerSwitchPass());
//fpm->add(createConstantHoistingPass());
//fpm->add(createMemCpyOptPass());
//fpm->add(createMergedLoadStoreMotionPass());
//fpm->add(createLCSSAPass());

//fpm->add(createPromoteMemoryToRegisterPass());
//fpm->add(createLoopRerollPass());
//fpm->add(createSimpleLoopUnrollPass());
//fpm->add(createGlobalMergePass());
//fpm->add(createLoopStrengthReducePass());
//fpm->add(createTailCallEliminationPass());
//fpm->add(createLoopSimplifyPass());
//fpm->add(createBreakCriticalEdgesPass());
//fpm->add(createFlattenCFGPass());
//fpm->add(createBitTrackingDCEPass());
//fpm->add(createConstantPropagationPass());
//fpm->add(createCFGSimplificationPass());
//fpm->add(createReassociatePass());
//fpm->add(createStructurizeCFGPass());

//fpm->add(createDemoteRegisterToMemoryPass());

//fpm->add(createLoopInterchangePass());

//fpm->add(createInductiveRangeCheckEliminationPass());
//fpm->add(createDeadCodeEliminationPass());
//fpm->add(createDeadInstEliminationPass());
//fpm->add(createAlignmentFromAssumptionsPass());
//fpm->add(createSROAPass());

        fpm->doInitialization();
        for (Function &f : *ir)
            fpm->run(f);
        fpm->doFinalization();
        pm->run(*ir);

        delete fpm;
        delete pm;

        string llvm2;
        raw_string_ostream buff(llvm2);
        ir->print(buff, NULL);
        return llvm2;
    }

    static Executable *compile(string llvm) {
/*
    TargetOptions Options;
    Options.LessPreciseFPMADOption = EnableFPMAD;
    Options.NoFramePointerElim = DisableFPElim;
    Options.NoFramePointerElimNonLeaf = DisableFPElimNonLeaf;
    Options.AllowFPOpFusion = FuseFPOps;
    Options.UnsafeFPMath = EnableUnsafeFPMath;
    Options.NoInfsFPMath = EnableNoInfsFPMath;
    Options.NoNaNsFPMath = EnableNoNaNsFPMath;
    Options.HonorSignDependentRoundingFPMathOption =
    EnableHonorSignDependentRoundingFPMath;
    Options.UseSoftFloat = GenerateSoftFloatCalls;
    if (FloatABIForCalls != FloatABI::Default)
        Options.FloatABIType = FloatABIForCalls;
    Options.NoZerosInBSS = DontPlaceZerosInBSS;
    Options.GuaranteedTailCallOpt = EnableGuaranteedTailCallOpt;
    Options.DisableTailCalls = DisableTailCalls;
    Options.StackAlignmentOverride = OverrideStackAlignment;
    Options.RealignStack = EnableRealignStack;
    Options.TrapFuncName = TrapFuncName;
    Options.PositionIndependentExecutable = EnablePIE;
    Options.EnableSegmentedStacks = SegmentedStacks;
    Options.UseInitArray = UseInitArray;
    Options.SSPBufferSize = SSPBufferSize;
*/


        LLVMInitializeNativeTarget();
        LLVMInitializeNativeAsmPrinter();
        LLVMContext &ctx = getGlobalContext();
        SMDiagnostic err;
        //Module *mod = ParseIR(MemoryBuffer::getMemBuffer(llvm), err, ctx);
        //mod->setTargetTriple(sys::getProcessTriple());
        auto x = MemoryBuffer::getMemBuffer(llvm)->getMemBufferRef();
        auto mod = parseIR(x, err, ctx);
        EngineBuilder builder(move(mod));
        builder.setMCPU(getHostCPUName());
        //builder.setEngineKind(EngineKind::JIT);
        builder.setOptLevel(Level::Aggressive);
        //builder.setUseMCJIT(true);
        //builder.setMCPU("corei7-avx");
        ExecutionEngine *ee = builder.create();
        return new Executable(ee);
    }
};
}
#endif
