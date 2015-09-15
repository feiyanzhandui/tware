#ifndef PAGERANK_WORKER_H
#define PAGERANK_WORKER_H

#include <cfloat>
#include <unistd.h>
#include <vector>

#include "job/Task.h"
#include "net/Socket.h"

using std::vector;

namespace tupleware {
class PagerankWorker : public Task {
public:
    PagerankWorker(Socket *reqSock, uint64_t fileId, int numPages, Socket *in,
            Socket *out) : Task(reqSock) {
        m_fileId = fileId;
        m_numPages = numPages;
        m_in = in;
        m_out = out;
    }

    virtual void run() {
usleep(10000);
        m_reqSock->connect();
        m_in->connect();
        m_out->connect();
usleep(1000);

        size_t countSize = m_numPages * sizeof(int);
        size_t rankSize = m_numPages * sizeof(float);
        size_t tempSize = rankSize;
        int *counts = (int *) new char[countSize];
        float *ranks = (float *) new char[rankSize];
        float *temp = (float *) new char[tempSize];

        for (int i = 0; i < m_numPages; i++)
            counts [i] = 1;
            //counts[i] = (int) (rand() / 1000 + 1);

int iter = 0;
        while (m_in->receive((char *) ranks, rankSize)) {
            for (int i = 0; i < m_numPages; i++)
                temp[i] = 0.0f;

            Block *blk = getBlock(m_fileId + iter);
            while (blk != NULL) {
                int *data = (int *) blk->data();
                size_t size = blk->size() / (2 * sizeof(int));
                for (int i = 0; i < size; i++) {
                    int dataOff = 2 * i;
                    int src = data[dataOff];
                    int dst = data[dataOff + 1];
                    temp[dst] += ranks[src] / counts[src];
                }
                blk = getBlock(m_fileId + iter);
            }
            m_out->send((char *) temp, tempSize);
iter++;
        }

        m_in->close();
        m_out->close();

        delete[] counts;
        delete[] ranks;
    }
private:
    uint64_t m_fileId;
    int m_numPages;
    Socket *m_in;
    Socket *m_out;
};
}
#endif
