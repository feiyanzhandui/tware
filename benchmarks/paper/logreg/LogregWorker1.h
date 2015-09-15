#ifndef LOGREG_WORKER1_H
#define LOGREG_WORKER1_H

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../Socket.h"
#include "../Util.h"
#include "../Worker.h"
#include "LogregUtil.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

class LogregWorker1 : public Worker {
public:
    LogregWorker1(int id, int numWorkers, Socket *in, Socket *out,
            vector<string> files) : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        m_d = new float[LOGREG_DATA * (LOGREG_ATTR + 1)];
        m_w = new float[LOGREG_ATTR];
        m_g = new float[LOGREG_ATTR];
        init_logreg_data(m_files[0], m_d);
    }

    virtual void execute() {
        while (m_in->receive((char *) m_w)) {
            for (int i = 0; i < LOGREG_ATTR; i++)
                m_g[i] = 0.0f;
            for (int i = 0; i < LOGREG_DATA; i++) {
                int dOff = i * (LOGREG_ATTR + 1);

                //train
                float dot = 0.0f;
                for (int j = 0; j < LOGREG_ATTR; j++)
                    dot += m_d[dOff + j] * m_w[j];
                float label = m_d[dOff + LOGREG_ATTR];
                float s = (1.0f / (1.0f + exp(-label * dot)) - 1.0f) * label;
                for (int j = 0; j < LOGREG_ATTR; j++)
                    m_g[j] += s * m_d[dOff + j];
            }
            m_out->send((char *) m_g, LOGREG_SEND);
        }
    }

    virtual void cleanup() {
        delete[] m_d;
        delete[] m_w;
        delete[] m_g;
    }
private:
    float *m_d;
    float *m_w;
    float *m_g;
};
#endif
