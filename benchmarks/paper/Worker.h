#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <vector>

#include "Socket.h"
#include "Util.h"

using std::string;
using std::vector;

class Worker {
public:
    static void *run(void *arg) {
        Worker *worker = (Worker *) arg;
        worker->connect();
        worker->wait();
        worker->init();
        worker->ready();

        worker->wait();
        worker->execute();

        worker->wait();
        worker->cleanup();
        worker->ready();
        worker->close();

        return NULL;
    }

    Worker(int id, int numWorkers, Socket *in, Socket *out,
            vector<string> files) {
        m_id = id;
        m_numWorkers = numWorkers;
        m_in = in;
        m_out = out;
        m_files = files;
    }

    virtual ~Worker() {
        delete m_in;
        delete m_out;
    }

    virtual void init() = 0;
    virtual void execute() = 0;
    virtual void cleanup() = 0;
protected:
    int m_id;
    int m_numWorkers;
    Socket *m_in;
    Socket *m_out;
    vector<string> m_files;

    void connect() {
usleep(10000);
        m_in->connect();
        m_out->connect();
        //char c;
        //while (!m_in->receive(&c));
        m_out->send(NULL, 0);
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
