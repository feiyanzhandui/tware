#ifndef T_STREAM_H
#define T_STREAM_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "llvm/ADT/StringRef.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_os_ostream.h"
#include "thread/Runnable.h"
#include "net/Socket.h"

#include "data/Schema.h"
#include "frontend/Connection.h"
#include "frontend/Context.h"
#include "frontend/Input.h"
#include "message/AttributeMessage.pb.h"
#include "message/ExpressionMessage.pb.h"
#include "message/JobMessage.pb.h"
#include "net/Socket.h"
#include "util/Defaults.h"
#include "util/Logging.h"

using std::ifstream;
using std::ostringstream;
using std::string;

using llvm::Function;
using llvm::getGlobalContext;
using llvm::LLVMContext;
using llvm::MemoryBuffer;
using llvm::Module;
using llvm::ParseIR;
using llvm::raw_string_ostream;
using llvm::SMDiagnostic;
using llvm::StringRef;

namespace tupleware {
class TStream {
public:
    TSet(string filename) {
        m_conn = Connection::getConnection("localhost", Defaults::CONN_PORT);
        m_exprId = 0;
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_id(m_exprId++);
        exprMsg->set_type(ExpressionMessage::LOAD);
        LoadMessage *loadMsg = exprMsg->MutableExtension(LoadMessage::ext);
        loadMsg->set_filename("");
        m_wflowMsg = exprMsg;
        m_ctx = new Context();
    }

    TupleSet(Connection *conn, Input *in) {
        m_conn = conn;
        m_exprId = 0;
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_id(m_exprId++);
        exprMsg->set_type(ExpressionMessage::LOAD);
        LoadMessage *loadMsg = exprMsg->MutableExtension(LoadMessage::ext);
        loadMsg->set_filename("");
        m_wflowMsg = exprMsg;
        m_ctx = new Context();
    }

TSet *kmeans(string type, int numAttr, int numCent) {
    ExpressionMessage *exprMsg = new ExpressionMessage();
    exprMsg->set_id(m_exprId++);
    exprMsg->set_type(ExpressionMessage::KMEANS);

    KmeansMessage *kmeansMsg = exprMsg->MutableExtension(KmeansMessage::ext);
    kmeansMsg->set_src(0);
    kmeansMsg->set_type(type);
    kmeansMsg->set_num_attr(numAttr);
    kmeansMsg->set_num_cent(numCent);

    m_wflowMsg = exprMsg;
    return this;
}

    //TupleSet(TupleSet *other) {
    //    m_conn = other->m_conn;
    //    m_workflow = other->m_workflow;
    //    m_ctx = other->m_ctx;
    //}
/*
    TupleSet *filter(Udf f) {
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_type(ExpressionMessage::FILTER);
        FilterMessage *ext = exprMsg->MutableExtension(FilterMessage::ext);
        ExpressionMessage *extSrc = ext->mutable_source();
        extSrc->CopyFrom(*m_exprMsg);
        ext->set_function(funct);
        m_exprMsg = exprMsg;
        return this;
    }

    TupleSet *flatMap(string funct) {
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_type(ExpressionMessage::FLAT_MAP);
        FlatMapMessage *ext = exprMsg->MutableExtension(FlatMapMessage::ext);
        ExpressionMessage *extSrc = ext->mutable_source();
        extSrc->CopyFrom(*m_exprMsg);
        ext->set_function(funct);
        m_exprMsg = exprMsg;
        return this;
    }

    TupleSet *join(TupleSet *other, string funct) {
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_type(ExpressionMessage::JOIN);
        JoinMessage *ext = exprMsg->MutableExtension(JoinMessage::ext);
        ExpressionMessage *extLhs = ext->mutable_left_hand_side();
        extLhs->CopyFrom(*m_exprMsg);
        ExpressionMessage *extRhs = ext->mutable_right_hand_side();
        extRhs->CopyFrom(*other->m_exprMsg);
        ext->set_function(funct);
        m_exprMsg = exprMsg;
        return this;
    }

    TupleSet *loop(string funct) {
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_type(ExpressionMessage::LOOP);
        LoopMessage *ext = exprMsg->MutableExtension(LoopMessage::ext);
        //
        m_exprMsg = exprMsg;
        return this;
    }
*/
    TSet *map(string udf) {
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_id(m_exprId++);
        exprMsg->set_type(ExpressionMessage::MAP);

        MapMessage *mapMsg = exprMsg->MutableExtension(MapMessage::ext);
        ExpressionMessage *srcMsg = m_wflowMsg;
        mapMsg->set_allocated_src(srcMsg);

        UdfMessage *udfMsg = mapMsg->mutable_udf();
        udfMsg->set_llvm(udf);
        m_udfMsgs.push_back(udfMsg);

        m_wflowMsg = exprMsg;
        return this;
    }

    TSet *reduce(string udf, string key = "") {
        ExpressionMessage *exprMsg = new ExpressionMessage();
        exprMsg->set_id(m_exprId++);
        exprMsg->set_type(ExpressionMessage::REDUCE);

        ReduceMessage *redMsg = exprMsg->MutableExtension(ReduceMessage::ext);
        ExpressionMessage *srcMsg = m_wflowMsg;
        redMsg->set_allocated_src(srcMsg);

        UdfMessage *udfMsg = redMsg->mutable_udf();
        udfMsg->set_llvm(udf);
        m_udfMsgs.push_back(udfMsg);

        if (key.length() > 0) {
            UdfMessage *keyMsg = redMsg->mutable_key();
            keyMsg->set_llvm(key);
            m_udfMsgs.push_back(keyMsg);
        }

        m_wflowMsg = exprMsg;
        return this;
    }

    TSet *evaluate() {
        m_exprId = 0;
        JobMessage *jobMsg = new JobMessage();
        jobMsg->set_conn_id(m_conn->id());
        jobMsg->set_allocated_wflow(m_wflowMsg);

        ostringstream buff;
        for (int i = 0; i < m_udfMsgs.size(); i++) {
            UdfMessage *udfMsg = m_udfMsgs[i];
            udfMsg->set_id(i);
            buff << "auto f" << i << udfMsg->llvm() << "\n";
        }

        string cmd = "echo '" + buff.str() + "' |"
                   + " clang++ -cc1 -xc++ -O0 -std=c++1y -fno-use-cxa-atexit"
                   + " -I/usr/local/lib/clang/3.5.0/include/"
        + " -I/usr/include/c++/4.9/"
        + " -I/usr/include/x86_64-linux-gnu/c++/4.9/bits/"
        + " -I/usr/include/x86_64-linux-gnu -I/usr/include/"
        + " -I/usr/include/x86_64-linux-gnu/c++/4.9/"
        + " -I/usr/lib/gcc/x86_64-linux-gnu/4.9/include/"
        + " -I/usr/include/c++/4.9/backward/"
        + " -I/usr/lib/gcc/x86_64-linux-gnu/4.9/include-fixed/"
        + " -I" + string(getenv("TUPLEWARE_HOME")) + "/src/data/"
        + " -I" + string(getenv("TUPLEWARE_HOME")) + "/src/"
        + " -stdlib=libstdc++ -S -emit-llvm -o /dev/stdout 2> /dev/stdout";

        FILE *file = popen(cmd.c_str(), "r");
        if (!file) {
            //error
        }

        //if (llvm_functs.size() != functs.size())
        //    Logging::fatal(__FILE__,__LINE__,"Not all UDFs were compiled");

        ostringstream llvm;
        constexpr size_t LINE_SIZE = 1024;
        char line[LINE_SIZE];
        while (fgets(line, LINE_SIZE, file))
            llvm << line;
        int stat = pclose(file);
        if (stat/256 == 1) {
            Logging::fatal(__FILE__,__LINE__, "UDFs did not compile\n\n"
                + llvm.str() + "\n\n");
        }

        LLVMInitializeNativeTarget();
        LLVMInitializeNativeAsmPrinter();
        LLVMContext &ctx = getGlobalContext();
        SMDiagnostic *err;
        StringRef ref = StringRef(llvm.str().c_str());
        Module *mod = ParseIR(MemoryBuffer::getMemBuffer(ref), *err, ctx);
        int i = 0;
        for (Module::iterator itr = mod->begin(); itr != mod->end(); ++itr) {
            Function &fun = *itr;
            string name = fun.getName();
            string body;
            raw_string_ostream buf(body);
            fun.print(buf);
cout << body << endl;
            if (name.compare(0, 4, "_Z2f") == 0) {
                m_udfMsgs[i]->set_llvm(body);
                i++;
            }
        }
        //if i == udf.size() then we're good
jobMsg->set_stored_proc(-1);
        Socket *jobSock = m_conn->m_jobSock;
        jobSock->send(jobMsg);
        //jobSock->close();

        int resPort = Defaults::RESULT_PORT;
        //ZmqSocket *rsock = NULL;
        return this;
    }

    TSet *save() {//Output *out) {
        return evaluate();
        //m_expr = new ExpressionMessage();
        //m_expr->set_type(ExpressionMessage::SAVE);
        //SaveMessage *ext = m_expr->MutableExtension(SaveMessage::ext);
        //ExpressionMessage *extsrc = ext->mutable_source();
        //extsrc->CopyFrom(*src->m_expr);
        //ext->set_filename(filename);
    }
/*
/*
    bool hasNext() {
        return true;
    }

    T *next() {
        return NULL;
    }
*/
protected:
    Connection *m_conn;
    int m_exprId;
    ExpressionMessage *m_wflowMsg;
    vector<UdfMessage *> m_udfMsgs;
    Context *m_ctx;
};
}
#endif
