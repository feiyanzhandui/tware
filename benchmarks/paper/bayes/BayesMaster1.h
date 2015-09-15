#ifndef BAYES_MASTER1_H
#define BAYES_MASTER1_H

#include <string>
#include <vector>

#include "../Master.h"
#include "../Socket.h"
#include "BayesUtil.h"

using std::string;
using std::vector;

class BayesMaster1 : public Master {
public:
    BayesMaster1(int numWorkers, Socket *in, Socket *out, vector<string> files)
            : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tDATA:  " << BAYES_DATA
             << "\n\tTOTAL: " << (BAYES_DATA * m_numWorkers)
             << endl;
    }

    virtual void init() {
        m_counts = new int[BAYES_COUNT];
        m_recv = new int[BAYES_COUNT];

        for (int i = 0; i < BAYES_COUNT; i++)
            m_counts[i] = 0;
    }

    virtual void execute() {
        m_out->send(NULL, 0);
        for (int i = 0; i < m_numWorkers; i++) {
            m_in->receive((char *) m_recv);
            for (int j = 0; j < BAYES_COUNT; j++)
                m_counts[j] += m_recv[j];
        }
    }

    virtual void cleanup() {
        int numCounts = BAYES_COUNT < 10 ? BAYES_COUNT : 10;
        cout << "First " << numCounts << " counts:\n";
        for (int i = 0; i < numCounts; i++)
            cout << "\tcounts[" << i << "]:" << m_counts[i] << "\n";
        cout << endl;
        delete[] m_counts;
        delete[] m_recv;
    }
private:
    int *m_counts;
    int *m_recv;
};
#endif
