#ifndef JOIN_MASTER_H
#define JOIN_MASTER_H

#include "Master.h"
#include "Socket.h"

class JoinMaster : public Master {
public:
    JoinMaster(int numWorkers, Socket *in, Socket *out)
            : Master(numWorkers, in, out) {}

    virtual void printParams() {}
    virtual void init() {}
    virtual void execute() {}
    virtual void cleanup() {}
};
#endif
