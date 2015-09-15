#ifndef PAGERANK_MASTER1_H
#define PAGERANK_MASTER1_H

#include <string>
#include <vector>

#include "../Master.h"
#include "../Socket.h"
#include "PagerankUtil.h"

using std::string;
using std::vector;

class PagerankMaster1 : public Master {
public:
    PagerankMaster1(int numWorkers, Socket *in, Socket *out,
            vector<string> files) : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tITER: " << PAGERANK_ITER
             << "\n\tPAGE: " << PAGERANK_PAGE
             << "\n\tLINK: " << PAGERANK_LINK
             << "\n\tTOTAL: " << (PAGERANK_LINK * m_numWorkers)
             << endl;
    }

    virtual void init() {
        m_ranks = new float[PAGERANK_PAGE];
        m_temp = new float[PAGERANK_PAGE];
        m_recv = new float[PAGERANK_PAGE];

        for (int i = 0; i < PAGERANK_PAGE; i++)
            m_ranks[i] = 1.0f;
    }

    virtual void execute() {
        for (int i = 0; i < PAGERANK_ITER; i++) {
            cout << "iter: " << i << endl;
            m_out->send((char *) m_ranks, PAGERANK_SEND);
            for (int j = 0; j < PAGERANK_PAGE; j++)
                m_temp[j] = 0.0f;
            for (int j = 0; j < m_numWorkers; j++) {
                m_in->receive((char *) m_recv);
                for (int k = 0; k < PAGERANK_PAGE; k++)
                    m_temp[k] += m_recv[k];
            }
            for (int j = 0; j < PAGERANK_PAGE; j++)
                if (m_temp[j] > 0.0f)
                    m_ranks[j] = 0.15f + 0.85f * m_temp[j];
        }
        m_out->send(NULL, 0);
    }

    virtual void cleanup() {
        cout << "\n*** RANKS ***" << endl;
        for (int i = 0; i < 20; i++)
            cout << "\tranks[" << i << "]=" << m_ranks[i] << endl;

        delete[] m_ranks;
        delete[] m_recv;
    }
private:
    float *m_ranks;
    float *m_temp;
    float *m_recv;
};
#endif
