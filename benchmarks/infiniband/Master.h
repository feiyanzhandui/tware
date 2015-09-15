#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <string>
#include <sys/time.h>
#include <unistd.h>

#include "Socket.h"
#include "Util.h"

using std::cout;
using std::endl;
using std::string;

class Master {
public:
    static void *run(void *arg) {
        cout << "***** STARTING MASTER *****" << endl;
        Master *master = (Master *) arg;

        cout << "\n*** CONNECT ***" << endl;
        master->printParams();
        master->bind();
        master->wait();

        cout << "\n*** INIT ***" << endl;
        gettimeofday(&(master->m_init1), NULL);
        master->ready();
        master->init();
        master->wait();
        gettimeofday(&(master->m_init2), NULL);

        cout << "\n*** EXECUTE ***" << endl;
        gettimeofday(&(master->m_exec1), NULL);
        master->ready();
        master->execute();
        master->wait();
        gettimeofday(&(master->m_exec2), NULL);

        cout << "\n*** CLEANUP ***" << endl;
        gettimeofday(&(master->m_clean1), NULL);
        master->ready();
        master->cleanup();
        master->wait();
        gettimeofday(&(master->m_clean2), NULL);

        master->ready();
        master->close();
        master->printInfo();
        return NULL;
    }

    Master(int numWorkers, Socket *in, Socket *out) {
        m_numWorkers = numWorkers;
        m_in = in;
        m_out = out;
    }

    virtual ~Master() {
        delete m_in;
        delete m_out;
    }

    virtual void printParams() = 0;
    virtual void init() = 0;
    virtual void execute() = 0;
    virtual void cleanup() = 0;
protected:
    int m_numWorkers;
    Socket *m_in;
    Socket *m_out;
    struct timeval m_init1;
    struct timeval m_init2;
    struct timeval m_exec1;
    struct timeval m_exec2;
    struct timeval m_clean1;
    struct timeval m_clean2;

    void bind() {
        m_out->bind();
        m_in->bind();
    }

    void ready() {
        m_out->send(NULL, 0);
    }

    void wait() {
        cout << "Waiting for " << m_numWorkers << " workers..." << endl;
        for (int i = 1; i <= m_numWorkers; i++) {
            m_in->receive(NULL);
            cout << "\t" << i << " workers ready." << endl;
        }
        cout << "All " << m_numWorkers << " workers ready." << endl;
    }

    void close() {
        m_out->close();
        m_in->close();
    }

    void printInfo() {
        cout << "\n*** RUN INFO ***"
             << "\n\tinit:    " << Util::getTime(m_init1, m_init2)
             << "\n\trun:     " << Util::getTime(m_exec1, m_exec2)
             << "\n\tcleanup: " << Util::getTime(m_clean1, m_clean2)
             << endl;
    }
};
#endif
