#ifndef DEFAULTS_H
#define DEFAULTS_H

namespace tupleware {
class Defaults {
public:
    static const int LCAT_PORT     = 4000;
    static const int BCAST_PORT    = 4001;
    static const int CONN_PORT     = 4002;
    static const int FILE_PORT     = 4003;
    static const int JOB_PORT      = 4004;
    static const int RESULT_PORT   = 4005;
    static const int TASK_PORT     = 4009;
    static const int WORKER_PORT   = 4010;
    static const int BLOCK_PORT    = 4011;
    static const int MGMT_PORT     = 4012;
    static const int GCAT_PORT     = 4013;
    static const int SORT_PORT     = 9000;
};
}
#endif
