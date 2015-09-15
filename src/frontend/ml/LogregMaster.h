#ifndef LOGREG_MASTER_H
#define LOGREG_MASTER_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#include "job/Task.h"
#include "net/Socket.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace tupleware {
class LogregMaster : public Task {
public:
    LogregMaster(int numWorkers, int numIter, int numAttr, Socket *in,
            Socket *out) : Task(NULL) {
        m_numWorkers = numWorkers;
        m_numIter = numIter;
        m_numAttr = numAttr;
        m_in = in;
        m_out = out;
    }

    virtual void run() {
usleep(10000);
        m_in->bind();
        m_out->bind();
usleep(100000);

        size_t wSize = m_numAttr * sizeof(float);
        float *w = (float *) new char[wSize];
        float *g = (float *) new char[wSize];

        srand((unsigned) time(NULL));
        for (int i = 0; i < m_numAttr; i++)
            w[i] = 0.5f;

        for (int i = 0; i < m_numIter; i++) {
            m_out->send((char *) w, wSize);
            for (int j = 0; j < m_numWorkers; j++) {
                m_in->receive((char *) g, wSize);
                for (int k = 0; k < m_numAttr; k++)
                    w[k] -= g[k] / m_numWorkers;
            }
        }
        m_out->send(NULL, 0);

        cout << "\nw:\n";
        for (int i = 0; i < m_numAttr; i++)
            cout << w[i] << " ";
        cout << "\n" << endl;

        delete[] w;
        delete[] g;
    }
private:
    int m_numWorkers;
    int m_numIter;
    int m_numAttr;
    Socket *m_in;
    Socket *m_out;
};
}
#endif
