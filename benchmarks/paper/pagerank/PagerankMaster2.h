#ifndef PAGERANK_MASTER2_H
#define PAGERANK_MASTER2_H

#include <string>
#include <vector>

#include "../Master.h"
#include "../ZmqSocket.h"
#include "PagerankUtil.h"

using std::string;
using std::vector;

class PagerankMaster2 : public Master {
public:
    PagerankMaster2(int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tITER: " << PAGERANK_ITER
             << "\n\tPAGE: " << PAGERANK_PAGE
             << "\n\tLINK: " << PAGERANK_LINK
             << endl;
    }

    virtual void init() {
        m_ranks = new float[PAGERANK_PAGE];
        m_tempRanks = new float[PAGERANK_PAGE];
        m_recvRanks = new float[PAGERANK_PAGE * 2];

        for (int i = 0; i < PAGERANK_PAGE; i++)
            m_ranks[i] = 1.0f;
    }

    virtual void execute() {
        for (int i = 0; i < PAGERANK_ITER; i++) {
            cout << "iter: " << i << endl;
            m_out->send((char *) m_ranks, PAGERANK_SEND);
            for (int j = 0; j < PAGERANK_PAGE; j++)
                m_tempRanks[j] = 0.0f;
            for (int j = 0; j < m_numWorkers; j++) {
                int recv = m_in->receive((char *) m_recvRanks);
                for (int k = 0; k < recv / sizeof(float); k += 2) {
                    int page = *((int *) (m_recvRanks + k));
                    m_tempRanks[page] += m_recvRanks[k + 1];
                }
            }
            for (int j = 0; j < PAGERANK_PAGE; j++)
                if (m_tempRanks[j] > 0.0f)
                    m_ranks[j] = 0.15f + 0.85f * m_tempRanks[j];
        }
        m_out->send(NULL, 0);
    }

    virtual void cleanup() {
        cout << "\n*** RANKS ***" << endl;
        for (int i = 0; i < 20; i++)
            cout << "\trank[" << i << "]=" << m_ranks[i] << endl;

        delete[] m_ranks;
        delete[] m_tempRanks;
        delete[] m_recvRanks;
    }
private:
    float *m_ranks;
    float *m_tempRanks;
    float *m_recvRanks;
};
#endif
