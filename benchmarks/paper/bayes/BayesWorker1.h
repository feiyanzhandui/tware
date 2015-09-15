#ifndef BAYES_WORKER1_H
#define BAYES_WORKER1_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../Socket.h"
#include "../Util.h"
#include "../Worker.h"
#include "BayesUtil.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

class BayesWorker1 : public Worker {
public:
    BayesWorker1(int id, int numWorkers, Socket *in, Socket *out,
            vector<string> files) : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        m_data = new int[BAYES_DATA * (BAYES_ATTR + 1)];
        m_send = new int[BAYES_COUNT];

        //data
        ifstream f(m_files[0]);
        if (f.is_open()) {
            string line;
            char delim;
            for (int i = 0; i < BAYES_DATA && getline(f, line); i++) {
                istringstream buff(line);
                int dataOff = i * (BAYES_ATTR + 1);
                for (int j = 0; j <= BAYES_ATTR; j++)
                    buff >> m_data[dataOff + j] >> delim;
            }
            f.close();
        }

        //send
        for (int i = 0; i < BAYES_COUNT; i++)
            m_send[i] = 0;
    }

    virtual void execute() {
        m_in->receive(NULL);
        for (int i = 0; i < BAYES_DATA; i++) {
            int dataOff = i * (BAYES_ATTR + 1);

            //train
            int label = m_data[dataOff + BAYES_ATTR];
            m_send[label]++;
            int sendOff = BAYES_VALUE + label * BAYES_ATTR * BAYES_VALUE;
            for (int j = 0; j < BAYES_ATTR; j++)
                m_send[sendOff + j * BAYES_VALUE + m_data[dataOff + j]]++;
        }
        m_out->send((char *) m_send, BAYES_SEND);
    }

    virtual void cleanup() {
        delete[] m_data;
        delete[] m_send;
    }
private:
    int *m_data;
    int *m_send;
};
#endif
