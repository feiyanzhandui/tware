#ifndef KMEANS_MASTER1_H
#define KMEANS_MASTER1_H

#include <iostream>
#include <string>
#include <vector>

#include "../Master.h"
#include "../Socket.h"
#include "KmeansUtil.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

class KmeansMaster1 : public Master {
public:
    KmeansMaster1(int numWorkers, Socket *in, Socket *out, vector<string> files)
            : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tITER: " << KMEANS_ITER
             << "\n\tDATA: " << KMEANS_DATA
             << "\n\tCENT: " << KMEANS_CENT
             << "\n\tTOTAL: " << (KMEANS_DATA * m_numWorkers)
             << "\n" << endl;
    }

    virtual void init() {
        m_cent = new float[KMEANS_CENT * KMEANS_ATTR];
        m_temp = new float[KMEANS_CENT * (KMEANS_ATTR + 1)];
        m_recv = new float[KMEANS_CENT * (KMEANS_ATTR + 1)];
        kmeans_init_cent(m_files[0], m_cent);
        cout << "INIT:\n";
        for (int i = 0; i < KMEANS_CENT; i++) {
            int centOff = i * KMEANS_ATTR;
            for (int j = 0; j < KMEANS_ATTR; j++)
                cout << m_cent[centOff + j] << " ";
            cout << "\n";
        }
        cout << "\n" << endl;
    }

    virtual void execute() {
        for (int i = 0; i < KMEANS_ITER; i++) {
            m_out->send((char *) m_cent, KMEANS_SEND1);
            for (int j = 0; j < KMEANS_CENT * (KMEANS_ATTR + 1); j++)
                m_temp[j] = 0.0f;
            for (int j = 0; j < m_numWorkers; j++) {
                m_in->receive((char *) m_recv);
                for (int k = 0; k < KMEANS_CENT * (KMEANS_ATTR + 1); k++)
                    m_temp[k] += m_recv[k];
            }
            for (int j = 0; j < KMEANS_CENT; j++) {
                int centOff = j * KMEANS_ATTR;
                int tempOff = j * (KMEANS_ATTR + 1);
                int count = m_temp[tempOff + KMEANS_ATTR];
                if (count > 0)
                    for (int k = 0; k < KMEANS_ATTR; k++)
                        m_cent[centOff + k] = m_temp[tempOff + k] / count;
            }
        }
        m_out->send(NULL, 0);
    }

    virtual void cleanup() {
        cout << "\nMEANS:\n";
        for (int i = 0; i < KMEANS_CENT; i++) {
            int centOff = i * KMEANS_ATTR;
            for (int j = 0; j < KMEANS_ATTR; j++)
                cout << m_cent[centOff + j] << " ";
            cout << "\n";
        }
        cout << "\n" << endl;

        delete[] m_cent;
        delete[] m_temp;
        delete[] m_recv;
    }
private:
    float *m_cent;
    float *m_temp;
    float *m_recv;
};
#endif
