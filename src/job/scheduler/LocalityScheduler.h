#ifndef LOCALITY_SCHEDULER_H
#define LOCALITY_SCHEDULER_H

#include "job/Job.h"
#include "job/Task.h"
#include "message/ClusterMessage.pb.h"
#include "message/WorkerMessage.pb.h"
#include "scheduler/Scheduler.h"
#include "util/Logger.h"
#include "util/LogLevel.h"

namespace tupleware {
class LocalityScheduler : public Scheduler {
public:
    LocalityScheduler(ClusterMessage *clusterMsg) : Scheduler(clusterMsg) {}

    virtual void schedule(Job *job) {
    }
};
}
#endif
