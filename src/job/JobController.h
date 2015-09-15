#ifndef JOB_CONTROLLER_H
#define JOB_CONTROLLER_H

#include <cstdint>
#include <string>
#include <vector>

#include "codegen/Generator.h"
#include "codegen/Printer.h"
#include "codegen/strategy/AdaptiveStrategy.h"
#include "codegen/strategy/OperatorStrategy.h"
#include "codegen/strategy/PipelineStrategy.h"
#include "codegen/template/JobTemplate.h"
#include "codegen/template/TaskTemplate.h"
#include "compiler/Compiler.h"
#include "compiler/Executable.h"
#include "job/Job.h"
#include "job/Result.h"
#include "job/Task.h"
#include "net/InprocSocket.h"
//#include "scheduler/Scheduler.h"
#include "server/GlobalManager.h"
#include "util/Logging.h"
#include "util/Runnable.h"
#include "util/Timer.h"

using std::string;
using std::vector;

namespace tupleware {
class JobController : public Runnable {
public:
    JobController(uint64_t jobId, Job *job) : Runnable() {
        m_jobId = jobId;
        m_job = job;
        m_jobSock = new InprocSocket("job" + to_string(m_jobId), ZMQ_PULL);
        m_jobExecSock = new InprocSocket("job" + to_string(m_jobId) + "_exec", ZMQ_REP);
        m_bcastSock = new InprocSocket("bcast", ZMQ_PUSH);
        //m_workers = workers;
        //m_scheduler = scheduler;
    }

    ~JobController() {
        delete m_job;
        delete m_bcastSock;
    }

    virtual void run() {
        m_jobSock->bind();
        m_jobExecSock->bind();
        m_bcastSock->connect();

        Timer *t1 = new Timer();
        Timer *t2 = new Timer();

        Strategy *strat = new PipelineStrategy();
        Generator *gen = new Generator(m_jobId, m_job->workflow(), strat);
        JobTemplate *jobTempl = gen->generate();

        Printer *jobp = new Printer();
        jobTempl->generate(*jobp);
        cerr << "\n\n" << jobp->str() << "\n\n" << endl;
        string llvm = Compiler::getLlvm(jobp->str());
        if (llvm.length() < 1000)
            cerr << "\n\n" << llvm << "\n\n" << endl;
        for (UdfTemplate *udfTempl : jobTempl->udfTemplates()) {
            //cerr << udfTempl->udf()->llvm() << endl;
            llvm = Compiler::link(llvm, udfTempl->udf()->llvm());
            //cerr << udfTempl->udf()->name() << endl;
        }

        Logging::info("Compiling job executable...");
        Executable *exec = Compiler::compile(llvm);

        Logging::info("Launching job executor...");
        Context *ctx;//= m_job->context();
        //Timer *t = new Timer();
        //t->start();
        InprocSocket *sock = new InprocSocket("job" + to_string(m_jobId) + "_exec", ZMQ_REQ);
        sock->connect();
        Thread *t = new Thread(exec->getRunnable(ctx, sock));
        t->start();
        Serializable *msg = m_jobExecSock->receive();
        m_jobExecSock->send(msg);

        for (TaskTemplate *taskTempl : jobTempl->taskTemplates()) {
            t1->start();
            Printer *taskp = new Printer();
            taskTempl->generate(*taskp);
            cerr << "\n\n" << taskp->str() << "\n\n" << endl;

            string llvm = Compiler::getLlvm(taskp->str());
            if (llvm.length() < 1000)
                cerr << "\n\n" << llvm << "\n\n" << endl;
            for (UdfTemplate *udfTempl : taskTempl->udfTemplates()) {
                if (llvm.length() < 200) {
                    cerr << "\n\n" << udfTempl->udf()->llvm() << "\n\n" << endl;
                    cerr << udfTempl->udf()->name() << endl;
                }
                llvm = Compiler::link(llvm, udfTempl->udf()->llvm());
            }
            if (llvm.length() < 1000)
                cerr << "\n\n" << llvm << "\n\n" << endl;
            t1->stop();
            t2->start();
cout << "Before: " << llvm.length() << endl;
            llvm = Compiler::optimize(llvm);
cout << "After:  " << llvm.length() << endl << endl;
            t2->stop();

            Task *task = new Task(llvm, taskTempl->filenames(), 1, m_job->context());
            m_bcastSock->send(task);
            //taskMsgs.push_back(taskMsg);
            delete taskp;

            //for (int j = 0; j < task->parallelism(); j++)
            //receive result message
        }
        Logging::info("Compiled in " + to_string(t1->time()) + "s");
        Logging::info("Optimized in " + to_string(t2->time()) + "s");

        //m_resSock->close();
        //delete job;
        delete t1;
        delete t2;
    }
private:
    uint64_t m_jobId;
    Job *m_job;
    InprocSocket *m_jobSock;
    InprocSocket *m_jobExecSock;
    InprocSocket *m_fmgrSock;
    InprocSocket *m_bcastSock;
    //vector<WorkerMessage *> *m_workers;
    //Scheduler *m_scheduler;
};
}
#endif
