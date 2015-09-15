#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job/Job.h"
#include "message/ManagementMessage.pb.h"
#include "message/WorkerMessage.pb.h"

namespace tupleware {
class Scheduler {
public:
    virtual void schedule(Job *job) = 0;
protected:
    ManagementMessage *m_mgmtMsg;
    int m_nextTaskId;

    Scheduler(ManagementMessage *mgmtMsg) {
        m_mgmtMsg = mgmtMsg;
        m_nextTaskId = -1;
    }
};
}
#endif
