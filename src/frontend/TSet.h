#ifndef T_SET_H
#define T_SET_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "compiler/Compiler.h"
#include "frontend/Connection.h"
#include "frontend/Context.h"
#include "frontend/Input.h"
#include "job/Result.h"
#include "job/Udf.h"
#include "job/op/FilterOp.h"
#include "job/op/LoadOp.h"
#include "job/op/LoopOp.h"
#include "job/op/MapOp.h"
#include "job/op/Op.h"
#include "job/op/ReduceOp.h"
#include "job/op/SaveOp.h"
#include "net/Socket.h"
#include "util/Defaults.h"
#include "util/Logging.h"

using std::ifstream;
using std::ostringstream;
using std::string;

#include <iostream>
using namespace std;

namespace tupleware {
class TSet {
public:
    TSet(string filename) {
        m_conn = Connection::getConnection("localhost", 4000);
        m_wflow = new LoadOp(filename);
        m_ctx = new Context();
    }

    TSet(Connection *conn, Input *in) {
        m_conn = conn;
        m_wflow = new LoadOp("TEST");
        m_ctx = new Context();
    }
/*
TSet *kmeans(string type, int numAttr, int numCent) {
    KmeansOp;
    kmeansMsg->set_src(0);
    kmeansMsg->set_type(type);
    kmeansMsg->set_num_attr(numAttr);
    kmeansMsg->set_num_cent(numCent);
}
*/
    //TupleSet(TupleSet *other) {
    //    m_conn = other->m_conn;
    //    m_workflow = other->m_workflow;
    //    m_ctx = other->m_ctx;
    //}
/*
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
*/

    TSet *filter(string udf) {
        Udf *fun = Udf::getUdf(udf);
        FilterOp *filtOp = new FilterOp(m_wflow, fun);
        m_wflow = filtOp;
        return this;
    }

    TSet *loop(string udf) {
        Udf *fun = Udf::getUdf(udf);
        LoopOp *loopOp = new LoopOp(m_wflow, fun);
        m_wflow = loopOp;
        return this;
    }

    TSet *map(string udf) {
        Udf *fun = Udf::getUdf(udf);
        MapOp *mapOp = new MapOp(m_wflow, fun);
        m_wflow = mapOp;
        return this;
    }

    TSet *reduce(string udf, string key = "") {
        Udf *fun = Udf::getUdf(udf);
        Udf *keyFun = NULL;
        if (key.length() > 0)
            keyFun = Udf::getUdf(key);
        ReduceOp *redOp = new ReduceOp(m_wflow, fun, keyFun);
        m_wflow = redOp;
        return this;
    }

    TSet *evaluate() {
        Job *job = new Job(m_wflow, m_ctx);
        m_conn->m_sock->send(job);
        Serializable *res = m_conn->m_sock->receive();
        return this;
    }

    TSet *save() {//Output *out) {
        SaveOp *saveOp = new SaveOp(m_wflow);
        m_wflow = saveOp;
        return evaluate();
    }

    Context *context() {
        return m_ctx;
    }
protected:
    Connection *m_conn;
    Op *m_wflow;
    Context *m_ctx;
};
}
#endif
