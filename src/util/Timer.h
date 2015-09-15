#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

namespace tupleware {
class Timer {
public:
    Timer() {
        m_time = 0.0;
        m_start = new timeval();
    }

    ~Timer() {
        delete m_start;
    }

    double time() {
        return m_time;
    }

    void start() {
        gettimeofday(m_start, NULL);
    }

    void stop() {
        struct timeval stop;
        gettimeofday(&stop, NULL);
        double t1 = (double) (m_start->tv_sec + m_start->tv_usec * USEC);
        double t2 = (double) (stop.tv_sec + stop.tv_usec * USEC);
        m_time += t2 - t1;
    }

    void reset() {
        m_time = 0.0;
        start();
    }
private:
    static const double USEC;

    double m_time;
    struct timeval *m_start;
};

const double Timer::USEC = 0.000001;
}
#endif
