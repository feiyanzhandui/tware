#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#include "util/Runnable.h"

namespace tupleware {
class Thread {
public:
    Thread(Runnable *run) {
        m_run = run;
    }

    ~Thread() {}

    void start() {
        pthread_create(&m_pt, NULL, Thread::start, (void *) m_run);
    }

    void join() {
        pthread_join(m_pt, NULL);
    }

    void stop() {
        m_run->kill();
    }
private:
    static void *start(void *arg) {
        ((Runnable *) arg)->run();
    }

    pthread_t m_pt;
    Runnable *m_run;
};
}
#endif
