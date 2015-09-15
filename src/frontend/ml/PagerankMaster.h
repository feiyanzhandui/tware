#ifndef PAGERANK_MASTER_H
#define PAGERANK_MASTER_H

#include <iostream>
#include <string>
#include <unistd.h>

#include "job/Task.h"
#include "net/Socket.h"
#include "util/Logging.h"

using std::cout;
using std::endl;
using std::string;

namespace tupleware {
class PagerankMaster : public Task {
public:
    PagerankMaster(int numWorkers, int numIter, int numPages, float jump,
            Socket *in, Socket *out) : Task(NULL) {
        m_numWorkers = numWorkers;
        m_numIter = numIter;
        m_numPages = numPages;
        m_jump = jump;
        m_in = in;
        m_out = out;
    }

    virtual void run() {
        m_in->bind();
        m_out->bind();
usleep(100000);

        size_t countSize = m_numPages * sizeof(int);
        size_t rankSize = m_numPages * sizeof(float);
        size_t tempSize = rankSize;
        float *counts = (float *) new char[countSize];
        float *ranks = (float *) new char[rankSize];
        float *temp = (float *) new char[tempSize];

        for (int i = 0; i < m_numPages; i++)
            ranks[i] = 1.0f;

        for (int i = 0; i < m_numIter; i++) {
            Logging::info("Running iter #" + to_string(i) + "...");
            m_out->send((char *) ranks, rankSize);
            for (int j = 0; j < m_numPages; j++)
                ranks[j] = 0.0f;
            for (int j = 0; j < m_numWorkers; j++) {
                m_in->receive((char *) temp, tempSize);
                for (int k = 0; k < m_numPages; k++)
                    ranks[k] += temp[k];
            }
            for (int j = 0; j < m_numPages; j++)
                ranks[j] = m_jump + (1.0f - m_jump) * ranks[j];
        }
        m_out->send(NULL, 0);

        m_in->close();
        m_out->close();
    }
private:
    int m_numWorkers;
    int m_numIter;
    int m_numPages;
    int m_jump;
    Socket *m_in;
    Socket *m_out;
};
}
#endif
