#ifndef BASIC_SCHEDULER_H
#define BASIC_SCHEDULER_H

#include "job/Job.h"
#include "message/ManagementMessage.pb.h"
#include "message/WorkerMessage.pb.h"
#include "scheduler/Scheduler.h"
#include "util/Logging.h"

namespace tupleware {
class BasicScheduler : public Scheduler {
public:
    BasicScheduler(ManagementMessage *mgmtMsg) : Scheduler(mgmtMsg) {}

    virtual void schedule(Job *job) {
        /*int numWorkers = m_workers->size();
        if (numWorkers == 0)
            Logger::fatal(__FILE__, __LINE__, "no workers");

        int numParallel = 0, numNonParallel = 0;
        int totalThreads = 0;
        for (int i = 0; i < m_workers->size(); i++) {
            totalThreads += m_workers->at(i).number_of_cores();
        }

        for (Task *task : phase->tasks()) {
            if (task->isParallel())
                numParallel++;
        }

        numNonParallel = phase->tasks().size() - numParallel;

        if (totalThreads < numParallel + numNonParallel)
            Logger::warn __FILE__, __LINE__, "num tasks exceeds num threads");

        vector<Task *> taskArray;

        int numThreadsPerTask = (totalThreads - numNonParallel) / numParallel;

        for (int t = 0; t < phase->tasks().size(); t++) {
            Task *task = phase->tasks()[t];
            if (!task->isParallel()) {
                taskArray.push_back(task);
            } else {
                for (int i = 0; i < numThreadsPerTask; i++)
                    taskArray.push_back(task);
            }
        }

        int pos = 0;
        for (int w = 0; w < numWorkers; w++) {
            unordered_map<Task *,int> counts;
            for (int i = 0; i < m_workers->at(w).number_of_cores(); i++) {
                if (pos < totalThreads) {
                    counts[taskArray[pos]] = counts[taskArray[pos]] + 1;
                    pos++;
                }
            }

            for (auto it = counts.begin(); it != counts.end(); ++it) {
                it->first->addWorker(w, it->second);
            }
        }*/
    }
};
}
#endif
