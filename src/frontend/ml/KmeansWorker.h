#ifndef KMEANS_WORKER_H
#define KMEANS_WORKER_H

#include <cstdint>
#include <cfloat>
#include <unistd.h>

#include "file/Block.h"
#include "job/Task.h"
#include "net/Socket.h"

#include "util/Config.h"

namespace tupleware {
class KmeansWorker : public Task {
public:
    KmeansWorker(Socket *reqSock, uint64_t fileId, int numAttr, int numCent,
            Socket *in, Socket *out) : Task(reqSock) {
        m_fileId = fileId;
        m_numAttr = numAttr;
        m_numCent = numCent;
        m_in = in;
        m_out = out;
    }

    virtual void run() {
usleep(10000);
        m_reqSock->connect();
        m_in->connect();
        m_out->connect();
usleep(1000);

        size_t centSize = m_numAttr * m_numCent * sizeof(TYPE);
        size_t tempSize = (m_numAttr + 1) * m_numCent * sizeof(TYPE);
        TYPE *cent = (TYPE *) new char[centSize];
        TYPE *temp = (TYPE *) new char[tempSize];

int iter = 0;
        while (m_in->receive((char *) cent, centSize)) {
            for (int i = 0; i < (m_numAttr + 1) * m_numCent; i++)
                temp[i] = 0.0f;

            Block *blk = getBlock(m_fileId + iter);
            while (blk != NULL) {
                TYPE *data = (TYPE *) blk->data();
                size_t size = blk->size() / (m_numAttr * sizeof(TYPE));
                for (int i = 0; i < size; i++) {
                    int dataOff = i * m_numAttr;
                    #if TYPE == float
                        TYPE min = FLT_MAX;
                    #elif TYPE == double
                        TYPE min = DBL_MAX;
                    #endif
                    int assign = 0;
                    for (int j = 0; j < m_numCent; j++) {
                        TYPE dist = 0.0f;
                        int centOff = j * m_numAttr;
                        for (int k = 0; k < m_numAttr; k++) {
                            TYPE d = cent[centOff + k] - data[dataOff + k];
                            dist += d * d;
                        }
                        if (dist < min) {
                            min = dist;
                            assign = j;
                        }
                    }
                    int tempOff = assign * m_numAttr + assign;
                    for (int j = 0; j < m_numAttr; j++)
                        temp[tempOff + j] += data[dataOff + j];
                    ++temp[tempOff + m_numAttr];
                }
                blk = getBlock(m_fileId + iter);
            }
            m_out->send((char *) temp, tempSize);
iter++;
        }

        m_reqSock->close();
        m_in->close();
        m_out->close();

        delete[] cent;
        delete[] temp;
    }
private:
    uint64_t m_fileId;
    int m_numAttr;
    int m_numCent;
    Socket *m_in;
    Socket *m_out;
};
}
#endif
