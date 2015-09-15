#ifndef PAGERANK_WORKER1_H
#define PAGERANK_WORKER1_H

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../Util.h"
#include "../Worker.h"
#include "../ZmqSocket.h"
#include "PagerankUtil.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

class PagerankWorker1 : public Worker {
public:
    PagerankWorker1(int id, ZmqSocket *in, ZmqSocket *out, vector<string> files)
            : Worker(id, in, out, files) {}

    virtual void init() {
        m_links = new int[PAGERANK_LINK * 2];
        m_nextLinks = new int[PAGERANK_LINK * 2];
        m_counts = new int[PAGERANK_PAGE];
        m_ranks = new float[PAGERANK_PAGE];
        m_prevRanks = new float[PAGERANK_PAGE];
        m_sendRanks1 = new float[PAGERANK_PAGE];
        m_sendRanks2 = new float[PAGERANK_PAGE];
        m_numLinks = 0;
        m_numNextLinks = 0;

        string fname = m_files[0];
        if (m_id < 10)
            fname += "0";
        ifstream f0(fname + to_string(m_id));
        if (f0.is_open()) {
            string line;
            char delim;
            for (int i = 0; i < PAGERANK_LINK && getline(f0, line); i++) {
                int linksOff = i * 2;
                istringstream buff(line);
                buff >> m_links[linksOff] >> delim >> m_links[linksOff + 1];
                m_numLinks++;
            }
            f0.close();
        }

        ifstream f1(m_files[1]);
        if (f1.is_open()) {
            string line;
            char delim;
            int idx;
            float temp;
            for (int i = 0; i < PAGERANK_PAGE && getline(f1, line); i++) {
                istringstream buff(line);
                buff >> m_counts[i];
                m_prevRanks[i] = 0.0f;
            }
            f1.close();
        }
    }

    virtual void execute() {
        while (m_in->receive((char *) m_ranks)) {
            for (int i = 0; i < m_numLinks; i++) {
                int linksOff = i * 2;
                int dst = m_links[linksOff + 1];
                if (fabs(m_ranks[dst] - m_prevRanks[dst]) > PAGERANK_EPS) {
                    int src = m_links[linksOff];
                    int nextLinksOff = m_numNextLinks * 2;
                    m_numNextLinks++;
                    m_nextLinks[nextLinksOff] = src;
                    m_nextLinks[nextLinksOff + 1] = dst;
                    m_sendRanks1[dst] += m_ranks[src] / m_counts[src];
                }
            }
            m_out->send((char *) m_sendRanks1, PAGERANK_SEND);

            cout << "num=" << m_numLinks << endl;

            float *tempSendRanks = m_sendRanks1;
            m_sendRanks1 = m_sendRanks2;
            m_sendRanks2 = tempSendRanks;
            for (int i = 0; i < PAGERANK_PAGE; i++)
                m_sendRanks1[i] = 0.0f;

            int *tempLinks = m_links;
            m_links = m_nextLinks;
            m_nextLinks = tempLinks;
            m_numLinks = m_numNextLinks;
            m_numNextLinks = 0;

            float *tempRanks = m_prevRanks;
            m_prevRanks = m_ranks;
            m_ranks = tempRanks;
        }
    }

    virtual void cleanup() {
        delete[] m_links;
        delete[] m_nextLinks;
        delete[] m_counts;
        delete[] m_ranks;
        delete[] m_prevRanks;
        delete[] m_sendRanks1;
        delete[] m_sendRanks2;
    }
private:
    int *m_links;
    int *m_nextLinks;
    int *m_counts;
    float *m_ranks;
    float *m_prevRanks;
    float *m_sendRanks1;
    float *m_sendRanks2;
    int m_numLinks;
    int m_numNextLinks;
};
#endif
