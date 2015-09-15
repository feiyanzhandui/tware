#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstdint>
#include <string>
#include <typeinfo>

#include "codegen/strategy/Strategy.h"
#include "codegen/template/DummyTemplate.h"
#include "codegen/template/JobTemplate.h"
#include "codegen/template/PipelineTemplate.h"
#include "codegen/template/StageTemplate.h"
#include "codegen/template/TaskTemplate.h"
#include "codegen/template/data/CollectorTemplate.h"
#include "codegen/template/data/DataTemplate.h"
#include "codegen/template/data/ScalarTemplate.h"
#include "codegen/template/data/StoreTemplate.h"
//#include "codegen/template/op/FilterOpTemplate.h"
#include "codegen/template/op/LoadOpTemplate.h"
#include "codegen/template/op/LoopOpTemplate.h"
#include "codegen/template/op/MapOpTemplate.h"
#include "codegen/template/op/ReduceOpTemplate.h"
#include "codegen/template/op/SaveOpTemplate.h"
#include "job/op/FilterOp.h"
#include "job/op/LoadOp.h"
#include "job/op/LoopOp.h"
#include "job/op/MapOp.h"
#include "job/op/Op.h"
#include "job/op/ReduceOp.h"
#include "job/op/SaveOp.h"
#include "util/Logging.h"

using std::string;
using std::type_info;

namespace tupleware {
class Generator {
public:
    Generator(uint64_t jobId, Op *wflow, Strategy *strat) {
        m_jobId = jobId;
        m_wflow = wflow;
        m_strat = strat;
        m_jobTempl = new JobTemplate(m_jobId);
        m_taskTempl = new TaskTemplate(m_jobId);
        m_stageTempls = new vector<StageTemplate *>();
    }

    ~Generator() {}

    JobTemplate *generate() {
        getOpTemplate(m_wflow);
        for (StageTemplate *stageTempl : *m_stageTempls)
            m_taskTempl->addBodyTemplate(stageTempl);
            //m_taskTempl->addStageTemplate(m_stageTempl);
        addTaskTemplate();
        return m_jobTempl;
    }
private:
    uint64_t m_jobId;
    Op *m_wflow;
    Strategy *m_strat;
    JobTemplate *m_jobTempl;
    TaskTemplate *m_taskTempl;
    vector<StageTemplate *> *m_stageTempls;
    PipelineTemplate *m_pipeTempl;

    UdfTemplate *getUdfTemplate(Udf *udf) {
        return new UdfTemplate(udf);
    }

    Template *getOpTemplate(Op *op) {
        const type_info *type = op->type();
        if (*type == typeid(FilterOp))
            return getFilterOpTemplate(op);
        if (*type == typeid(LoadOp))
            return getLoadOpTemplate(op);
        if (*type == typeid(LoopOp))
            return getLoopOpTemplate(op);
        if (*type == typeid(MapOp))
            return getMapOpTemplate(op);
        if (*type == typeid(ReduceOp))
            return getReduceOpTemplate(op);
        if (*type == typeid(SaveOp))
            return getSaveOpTemplate(op);
        Logging::error(__FILE__, __LINE__, "undefined op type");
        return NULL;
    }

    Template *getFilterOpTemplate(Op *op) {
        //LoopOp *loopOp = (LoopOp *) op;
        //getOpTemplate(loopOp->source());

        //UdfTemplate *udfTempl = getUdfTemplate(loopOp->udf());
        //addUdfTemplate(udfTempl);

        //LoopTemplate *loopTempl = new LoopTemplate(m_stageTempl, udfTempl);
        //m_taskTempl->addStageTemplate(loopTempl);
        return NULL;
    }

    Template *getLoadOpTemplate(Op *op) {
        LoadOp *loadOp = (LoadOp *) op;
        string valType = "valarray<float>";
        StoreTemplate *dataTempl = new StoreTemplate(valType);
        LoadOpTemplate *loadOpTempl = new LoadOpTemplate(dataTempl);

        Request::Type type = Request::Type::ANY;
        string filename = loadOp->filename();
        uint64_t blkId = 0;
        StageTemplate *stageTempl = new StageTemplate(type, filename, blkId);
        stageTempl->setInputTemplate(dataTempl);
        m_stageTempls->push_back(stageTempl);
        addPipelineTemplate();
        m_taskTempl->addFilename(filename);

        dataTempl->setInputTemplate(stageTempl);
        m_pipeTempl->setInputTemplate(dataTempl);
        m_pipeTempl->addOpTemplate(loadOpTempl);
        return loadOpTempl;
    }

    Template *getLoopOpTemplate(Op *op) {
        LoopOp *loopOp = (LoopOp *) op;
        getOpTemplate(loopOp->source());
        UdfTemplate *udfTempl = getUdfTemplate(loopOp->udf());
        addUdfTemplate(udfTempl);
        LoopOpTemplate *loopOpTempl = new LoopOpTemplate(udfTempl);
        loopOpTempl->addStageTemplates(m_stageTempls);
        m_taskTempl->addBodyTemplate(loopOpTempl);
        m_stageTempls = new vector<StageTemplate *>();
        //m_taskTempl->addRequestTemplate(loopTempl);
        return loopOpTempl;
    }

    Template *getMapOpTemplate(Op *op) {
        MapOp *mapOp = (MapOp *) op;
        Template *srcTempl = getOpTemplate(mapOp->source());

        UdfTemplate *udfTempl = getUdfTemplate(mapOp->udf());
        addUdfTemplate(udfTempl);

        MapOpTemplate *mapOpTempl = new MapOpTemplate(srcTempl, udfTempl);
        m_pipeTempl->addOpTemplate(mapOpTempl);
        //split?
        return mapOpTempl;
    }

    Template *getReduceOpTemplate(Op *op) {
        ReduceOp *redOp = (ReduceOp *) op;
        Template *srcTempl = getOpTemplate(redOp->source());
        UdfTemplate *udfTempl = getUdfTemplate(redOp->udf());
        string valType = udfTempl->udf()->returnType();
        addUdfTemplate(udfTempl);
        m_jobTempl->addUdfTemplate(udfTempl);

        UdfTemplate *keyTempl = NULL;
        DataTemplate *outTempl;
        if (redOp->key() != NULL) {
            keyTempl = getUdfTemplate(redOp->key());
            string keyType = keyTempl->udf()->returnType();
            addUdfTemplate(keyTempl);
            m_jobTempl->addUdfTemplate(keyTempl);
            outTempl = new CollectorTemplate(keyType, valType);
            m_stageTempls->back()->setOutputTemplate(outTempl);
            m_pipeTempl->setOutputTemplate(outTempl);
            //m_reqTempl->addDataTemplate(outTempl);
        }
        else {
            outTempl = new ScalarTemplate(valType);
            m_stageTempls->back()->setOutputTemplate(outTempl);
            m_pipeTempl->setOutputTemplate(outTempl);
            //m_reqTempl->addDataTemplate(outTempl);
        }

        ReduceOpTemplate *redOpTempl = new ReduceOpTemplate(srcTempl, udfTempl,
                                                   keyTempl, outTempl);
        m_pipeTempl->addOpTemplate(redOpTempl);

        DataTemplate *dataTempl = new ScalarTemplate(valType);
        DataTemplate *testTempl = new ScalarTemplate(valType);
        Request::Type type = Request::Type::ANY;
        string filename = "TEST_red";
        uint64_t blkId = 0;
        StageTemplate *stageTempl = new StageTemplate(type, filename, blkId);
        stageTempl->setInputTemplate(dataTempl);
        stageTempl->setOutputTemplate(testTempl);
        ReduceOpTemplate *redOpTempl2 = new ReduceOpTemplate(dataTempl, udfTempl,
                                                   keyTempl, testTempl);
        PipelineTemplate *pipeTempl = new PipelineTemplate();
        pipeTempl->setInputTemplate(dataTempl);
        pipeTempl->setOutputTemplate(testTempl);
        pipeTempl->addOpTemplate(redOpTempl2);
        stageTempl->addPipelineTemplate(pipeTempl);
        m_jobTempl->addBodyTemplate(stageTempl);

        //addUdfTemplate(udfTempl);
        //addUdfTemplate(keyTempl);
        //m_pipeTempl->addOpTemplate(redTempl);
        //addPipelineTemplate();
        //addStageTemplate();
        //addTaskTemplate();

        return redOpTempl;
    }

    Template *getSaveOpTemplate(Op *op) {
        SaveOp *saveOp = (SaveOp *) op;
        Template *srcTempl = getOpTemplate(saveOp->source());
        SaveOpTemplate *saveOpTempl = new SaveOpTemplate(srcTempl);
        //addSaveTemplate(saveTempl);
        return saveOpTempl;
    }

    void addTaskTemplate() {
        m_jobTempl->addTaskTemplate(m_taskTempl);
        m_taskTempl = new TaskTemplate(0);
    }

    void addUdfTemplate(UdfTemplate *udfTempl) {
        m_taskTempl->addUdfTemplate(udfTempl);
    }

    void addPipelineTemplate() {
        m_pipeTempl = new PipelineTemplate();
        m_stageTempls->back()->addPipelineTemplate(m_pipeTempl);
    }
};
}
#endif
