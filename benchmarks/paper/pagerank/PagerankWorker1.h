#ifndef PAGERANK_WORKER1_H
#define PAGERANK_WORKER1_H

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../Socket.h"
#include "../Util.h"
#include "../Worker.h"
#include "PagerankUtil.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

class PagerankWorker1 : public Worker {
public:
    PagerankWorker1(int id, int numWorkers, Socket *in, Socket *out,
            vector<string> files) : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        m_links = new int[PAGERANK_LINK * 2];
        m_counts = new int[PAGERANK_PAGE];
        m_ranks = new float[PAGERANK_PAGE];
        m_send = new float[PAGERANK_PAGE];

        //links
        ifstream f1(m_files[0]);
        if (f1.is_open()) {
            string line;
            char delim;
            for (int i = 0; i < PAGERANK_LINK && getline(f1, line); i++) {
                int linkOff = i * 2;
                istringstream buff(line);
                buff >> m_links[linkOff] >> delim >> m_links[linkOff + 1];
            }
            f1.close();
        }

        //counts
        ifstream f2(m_files[1]);
        if (f2.is_open()) {
            for (int i = 0; i < PAGERANK_PAGE; i++)
                m_counts[i] = 0.0f;

            string line;
            char delim;
            float temp;
            int id;
            for (int i = 0; i < PAGERANK_PAGE && getline(f2, line); i++) {
                istringstream buff(line);
                buff >> id >> delim;
                buff >> m_counts[id];
            }
            f2.close();
        }
    }

    virtual void execute() {
        while (m_in->receive((char *) m_ranks)) {
            for (int i = 0; i < PAGERANK_PAGE; i++)
                m_send[i] = 0.0f;
            for (int i = 0; i < PAGERANK_LINK; i++) {
                int linkOff = i * 2;

                //train
                int src = m_links[linkOff];
                int dst = m_links[linkOff + 1];
                m_send[dst] += m_ranks[src] / m_counts[src];
            }
            m_out->send((char *) m_send, PAGERANK_SEND);
        }
    }

    virtual void cleanup() {
        delete[] m_links;
        delete[] m_counts;
        delete[] m_ranks;
        delete[] m_send;
    }
private:
    int *m_links;
    int *m_counts;
    float *m_ranks;
    float *m_send;
};
#endif
