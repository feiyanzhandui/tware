#ifndef Q4_MASTER1_H
#define Q4_MASTER1_H

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

class Q4Master1 : public Master {
public:
    Q4Master1(int numWorkers, ZmqSocket *in, ZmqSocket *out,
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
        for (int i = 0; i < m_numWorkers; i++)
            m_in->receive(NULL);
        m_out->send(NULL, 0);
        unordered_map<char,char *> vals;
        char *val = new char[Q4_VALSIZE];
        for (int i = 0; i < m_numWorkers;) {
            if (m_in->receive(val)) {
                char key = val[0];
                auto itr = vals.find(key);
                if (itr == vals.end()) {
                    vals[key] = val;
                    val = new char[Q4_VALSIZE];
                }
                else {
                    char *res = itr->second;
                    *((int *) (res + 1)) += *((int *) (val + 1));
                }
            }
            else
                i++;
        }

        m_size = vals.size();
        r_orderpriority = new char[m_size];
        r_order_count = new int[m_size];
        int i = 0;
        for (auto itr = vals.begin(); itr != vals.end(); ++itr) {
            char *val = itr->second;
            r_orderpriority[i] = val[0];
            r_order_count[i] = *((int*) (val + 1));
            i++;
        }
        for (auto itr = vals.begin(); itr != vals.end(); ++itr)
            delete[] itr->second;
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
