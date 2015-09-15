#ifndef KMEANS_MASTER_H
#define KMEANS_MASTER_H

#include <iostream>
#include <string>
#include <unistd.h>

#include "job/Task.h"
#include "net/Socket.h"

using std::cout;
using std::endl;
using std::string;

#include "util/Config.h"

namespace tupleware {
class KmeansMaster : public Task {
public:
    KmeansMaster(int numWorkers, int numIter, int numAttr, int numCent,
            Socket *in, Socket *out) : Task(NULL) {
        m_numWorkers = numWorkers;
        m_numIter = numIter;
        m_numAttr = numAttr;
        m_numCent = numCent;
        m_in = in;
        m_out = out;
    }

    virtual void run() {
        m_in->bind();
        m_out->bind();
usleep(100000);

        size_t centSize = m_numAttr * m_numCent * sizeof(TYPE);
        size_t tempSize = (m_numAttr + 1) * m_numCent * sizeof(TYPE);
        TYPE *cent = (TYPE *) new char[centSize];
        TYPE *temp = (TYPE *) new char[tempSize];
        TYPE sum[centSize];
        TYPE count[m_numCent];

        srand((unsigned) time(NULL));
        for (int i = 0; i < m_numCent; i++) {
            int centOff = i * m_numAttr;
            for (int j = 0; j < m_numAttr; j++)
                cent[centOff + j] = (TYPE) (rand() % 10);
        }

        Logging::info("Initial Means:");
        for (int i = 0; i < m_numCent; i++) {
            int centOff = i * m_numAttr;
            cout << "\t( ";
            for (int j = 0; j < m_numAttr; j++)
                cout << cent[centOff + j] << " ";
            cout << ")" << endl;
        }

        for (int i = 0; i < m_numIter; i++) {
            Logging::info("Running iter #" + to_string(i) + "...");
            m_out->send((char *) cent, centSize);
            for (int j = 0; j < m_numCent; j++) {
                int centOff = j * m_numAttr;
                for (int k = 0; k < m_numAttr; k++)
                    sum[centOff + k] = 0.0f;
                count[j] = 0.0f;
            }
            for (int j = 0; j < m_numWorkers; j++) {
                m_in->receive((char *) temp, tempSize);
                for (int k = 0; k < m_numCent; k++) {
                    int centOff = k * m_numAttr;
                    int tempOff = centOff + k;
                    for (int l = 0; l < m_numAttr; l++)
                        sum[centOff + l] += temp[tempOff + l];
                    count[k] += temp[tempOff + m_numAttr];
                }
            }
            for (int j = 0; j < m_numCent; j++)
                if (count[j] > 0) {
                    int centOff = j * m_numAttr;
                    for (int k = 0; k < m_numAttr; k++)
                        cent[centOff + k] = sum[centOff + k] / count[j];
                }
        }
        m_out->send(NULL, 0);

        Logging::info("Final Means:");
        for (int i = 0; i < m_numCent; i++) {
            int centOff = i * m_numAttr;
            cout << "\t( ";
            for (int j = 0; j < m_numAttr; j++)
                cout << cent[centOff + j] << " ";
            cout << ")" << endl;
        }

        m_in->close();
        m_out->close();

        delete[] cent;
        delete[] temp;
    }
private:
    int m_numWorkers;
    int m_numIter;
    int m_numAttr;
    int m_numCent;
    Socket *m_in;
    Socket *m_out;
};
}
#endif
