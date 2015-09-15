#ifndef WORKER_H
#define WORKER_H

#include "Socket.h"
#include "Util.h"

class Worker {
public:
    static void *run(void *arg) {
        Worker *worker = (Worker *) arg;
        worker->connect();
        worker->ready();

        worker->wait();
        worker->init();
        worker->ready();

        worker->wait();
        worker->execute();
        worker->ready();

        worker->wait();
        worker->cleanup();
        worker->ready();

        worker->wait();
        worker->close();
        return NULL;
    }

    Worker(Socket *in, Socket *out) {
        m_in = in;
        m_out = out;
    }

    virtual ~Worker() {
        delete m_in;
        delete m_out;
    }

    virtual void init() = 0;
    virtual void execute() = 0;
    virtual void cleanup() = 0;
protected:
    Socket *m_in;
    Socket *m_out;

    void connect() {
        m_in->connect();
        m_out->connect();
    }

    void ready() {
        m_out->send(NULL, 0);
    }

    void wait() {
        m_in->receive(NULL);
    }

    void close() {
        m_in->close();
        m_out->close();
    }
};
#endif
