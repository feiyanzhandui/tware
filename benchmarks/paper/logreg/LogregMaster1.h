#ifndef LOGREG_MASTER1_H
#define LOGREG_MASTER1_H

#include <iostream>
#include <string>
#include <vector>

#include "../Master.h"
#include "../Socket.h"
#include "LogregUtil.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

class LogregMaster1 : public Master {
public:
    LogregMaster1(int numWorkers, Socket *in, Socket *out, vector<string> files)
            : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tITER: " << LOGREG_ITER
             << "\n\tDATA: " << LOGREG_DATA
             << "\n\tTOTAL: " << (LOGREG_DATA * m_numWorkers)
             << endl;
    }

    virtual void init() {
        m_w = new float[LOGREG_ATTR];
        m_g = new float[LOGREG_ATTR];

        srand((unsigned) time(NULL));
        for (int i = 0; i < LOGREG_ATTR; i++)
            m_w[i] = 0.5f;
    }

    virtual void execute() {
        for (int i = 0; i < LOGREG_ITER; i++) {
            m_out->send((char *) m_w, LOGREG_SEND);
            for (int j = 0; j < m_numWorkers; j++) {
                m_in->receive((char *) m_g);
                for (int k = 0; k < LOGREG_ATTR; k++)
                    m_w[k] -= m_g[k];
            }
        }
        m_out->send(NULL, 0);
    }

    virtual void cleanup() {
        cout << "\nw:\n";
        for (int i = 0; i < LOGREG_ATTR; i++)
            cout << m_w[i] << " ";
        cout << "\n" << endl;

        delete[] m_w;
        delete[] m_g;
    }
private:
    float *m_w;
    float *m_g;
};
#endif
