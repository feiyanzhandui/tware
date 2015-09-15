#ifndef Q4_MASTER2_H
#define Q4_MASTER2_H

#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../Master.h"
#include "../ZmqSocket.h"
#include "Q4Util.h"

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

class Q4Master2 : public Master {
public:
    Q4Master2(int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tORDERS:   " << Q4_ORDERS
             << "\n\tLINEITEM: " << Q4_LINEITEM
             << endl;
    }

    virtual void init() {}

    virtual void execute() {
        m_exists = new unordered_set<int>*[m_numWorkers];
        for (int i = 0; i < m_numWorkers; i++)
            m_exists[i] = new unordered_set<int>();
        m_out->send((char *) &m_exists, sizeof(&m_exists));
        for (int i = 0; i < m_numWorkers; i++) {
            m_in->receive(NULL, 0);
        }
        m_out->send(NULL, 0);
        int *vals = new int[?];
        memset() to 0
        int *recv = new int[?];
        for (int i = 0; i < m_numWorkers; i++) {
            m_in->receive((char *) recv, Q4_VALSIZE);
            for (int j = 0; j < ?; j++)
                vals[j] += recv[j];
        }
        r_orderpriority = new char[?];
        r_order_count = new int[?];
        for (int i = 0; i < ?; i++) {
            r_orderpriority[i] = i + '0';
            r_order_count[i] = vals[i];
        }
    }

    virtual void cleanup() {
        cout << "Q4 RESULTS\n";
        for (int i = 0; i < m_size; i++)
            cout << "\t"
                 << r_orderpriority[i] << ","
                 << r_order_count[i] << "\n";

        for (int i = 0; i < m_numWorkers; i++)
            delete m_exists[i];
        delete[] m_exists;
        delete[] r_orderpriority;
        delete[] r_order_count;
    }
private:
    unordered_set<int> **m_exists;
    int m_size;
    char *r_orderpriority;
    int *r_order_count;
};
#endif
