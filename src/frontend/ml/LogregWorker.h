#ifndef LOGREG_WORKER_H
#define LOGREG_WORKER_H

#include <cfloat>
#include <unistd.h>
#include <vector>

#include "job/Task.h"
#include "job/TaskController.h"
#include "net/Socket.h"

using std::vector;

namespace tupleware {
class LogregWorker : public Task {
public:
    LogregWorker(Socket *lreqSock, int numAttr, Socket *in, Socket *out)
            : Task(lreqSock) {
        m_numAttr = numAttr;
        m_in = in;
        m_out = out;
    }

    virtual void run() {
usleep(10000);
        m_reqSock->connect();
        m_in->connect();
        m_out->connect();
usleep(10000);

        size_t wSize = m_numAttr * sizeof(float);
        float *w = (float *) new char[wSize];
        float *g = (float *) new char[wSize];

        while (m_in->receive((char *) w, wSize)) {
            for (int i = 0; i < m_numAttr; i++)
                g[i] = 0.0f;
            Block *blk = getBlock(0);
            float *data = (float *) blk->data();
            size_t size = blk->size() / (m_numAttr * sizeof(float));
            for (int i = 0; i < size; i++) {
                int dataOff = i * m_numAttr;
                float dot = 0.0f;
                for (int j = 0; j < m_numAttr; j++)
                    dot += data[dataOff + j] * w[j];
                dot *= -data[dataOff + m_numAttr];
                for (int j = 0; j < m_numAttr; j++)
                    g[j] += dot * data[dataOff + j];
            }
            m_out->send((char *) g, wSize);
        }

        m_in->close();
        m_out->close();

        delete[] w;
        delete[] g;
    }
private:
    int m_numAttr;
    Socket *m_in;
    Socket *m_out;
};
}
#endif
