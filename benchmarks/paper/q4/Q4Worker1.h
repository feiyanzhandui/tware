#ifndef Q4_WORKER1_H
#define Q4_WORKER1_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../Util.h"
#include "../Worker.h"
#include "../ZmqSocket.h"
#include "Q4Util.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

class Q4Worker1 : public Worker {
public:
    Q4Worker1(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        o_orderkey = new int[Q4_ORDERS];
        o_orderdate = new long[Q4_ORDERS];
        o_orderpriority = new char[Q4_ORDERS];
        l_orderkey = new int[Q4_LINEITEM];
        l_commitdate = new long[Q4_LINEITEM];
        l_receiptdate = new long[Q4_LINEITEM];

        ifstream f1(m_files[0]);
        if (f1.is_open()) {
            char line[LINE_SIZE];
            struct tm tm = {0};
            f1.getline(line, LINE_SIZE);
            for (int i = 0; i < Q4_ORDERS && f1.good(); i++) {
                //o_orderkey
                char *pos = line;
                char *end = strchr(pos, TPCH_DELIM);
                *end = '\0';
                o_orderkey[i] = strtol(pos, &pos, 0);

                //skip
                pos = strchr(++pos, TPCH_DELIM);
                pos += 3;
                pos = strchr(pos, TPCH_DELIM);

                //o_orderdate
                pos += 11;
                *pos = '\0';
                strptime(pos - 10, "%Y-%m-%d", &tm);
                o_orderdate[i] = (long) mktime(&tm);

                //o_orderpriority
                o_orderpriority[i] = *(++pos);

                //next
                f1.getline(line, LINE_SIZE);
            }
            f1.close();
        }

        ifstream f2(m_files[1]);
        if (f2.is_open()) {
            char line[LINE_SIZE];
            struct tm tm = {0};
            f2.getline(line, LINE_SIZE);
            for (int i = 0; i < Q4_LINEITEM && f2.good(); i++) {
                //l_orderkey
                char *pos = line;
                char *end = strchr(pos, TPCH_DELIM);
                *end = '\0';
                l_orderkey[i] = strtol(pos, &pos, 0);

                //skip
                pos = strchr(++pos, TPCH_DELIM);
                pos = strchr(++pos, TPCH_DELIM);
                pos += 3;
                pos = strchr(pos, TPCH_DELIM);
                pos = strchr(++pos, TPCH_DELIM);
                pos = strchr(++pos, TPCH_DELIM);
                pos = strchr(++pos, TPCH_DELIM);

                //l_commitdate
                pos += 15;
                *pos = '\0';
                strptime(pos - 10, "%Y-%m-%d", &tm);
                l_commitdate[i] = (long) mktime(&tm);

                //l_receiptdate
                pos += 11;
                *pos = '\0';
                strptime(pos - 10, "%Y-%m-%d", &tm);
                l_receiptdate[i] = (long) mktime(&tm);

                //next
                f2.getline(line, LINE_SIZE);
            }
            f2.close();
        }
    }

    virtual void execute() {
        unordered_set<int> **exists;
        m_in->receive((char *) &exists);
        unordered_set<int> *curr = exists[m_id];
        for (int i = 0; i < Q4_LINEITEM; i++)
            if (l_commitdate[i] < l_receiptdate[i])
                curr->insert(l_orderkey[i]);
        m_out->send(NULL, 0);
        m_in->receive(NULL);
        for (int i = 0; i < Q4_ORDERS; i++)
            if (o_orderdate[i] >= Q4_DATE1 && o_orderdate[i] < Q4_DATE2)
                for (int j = 0; j < m_numWorkers; j++) {
                    curr = exists[j];
                    if (curr->find(o_orderkey[i]) != curr->end()) {
                        char key = o_orderpriority[i];
                        auto itr = m_vals.find(key);
                        char *val;
                        if (itr == m_vals.end()) {
                            val = new char[Q4_VALSIZE];
                            val[0] = key;
                            *((int *) (val + 1)) = 0;
                            m_vals[key] = val;
                        }
                        else
                            val = itr->second;
                        *((int *) (val + 1)) += 1;
                        break;
                    }
                }
        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr)
            m_out->send(itr->second, Q4_VALSIZE);
        m_out->send(NULL, 0);
    }

    virtual void cleanup() {
        delete[] o_orderkey;
        delete[] o_orderdate;
        delete[] o_orderpriority;
        delete[] l_orderkey;
        delete[] l_commitdate;
        delete[] l_receiptdate;
        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr)
            delete itr->second;
    }
private:
    int *o_orderkey;
    long *o_orderdate;
    char *o_orderpriority;
    int *l_orderkey;
    long *l_commitdate;
    long *l_receiptdate;
    unordered_map<char,char *> m_vals;
};
#endif
