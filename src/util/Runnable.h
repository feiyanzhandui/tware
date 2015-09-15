#ifndef RUNNABLE_H
#define RUNNABLE_H

namespace tupleware {
class Runnable {
public:
    void kill() {
        m_kill = true;
    }

    virtual void run() = 0;
protected:
    bool volatile m_kill;

    Runnable() {
        m_kill = false;
    }
};
}
#endif
