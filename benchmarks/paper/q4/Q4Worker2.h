#ifndef Q4_WORKER2_H
#define Q4_WORKER2_H

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

class Q4Worker2 : public Worker {
public:
    Q4Worker2(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        o_orderkey = new int[Q4_ORDERS];
        o_orderdate = new short[Q4_ORDERS];
        o_orderpriority = new char[Q4_ORDERS];
        l_orderkey = new int[Q4_LINEITEM];
        l_commitdate = new short[Q4_LINEITEM];
        l_receiptdate = new short[Q4_LINEITEM];

        ifstream f1(m_files[0]);
        if (f1.is_open()) {
            int size = 256;
            char buff[size];
            struct tm tm = {0};
            time_t t;
            long secs;
            f1.getline(buff, size);
            for (int i = 0; i < Q4_ORDERS && f1.good(); i++) {
                char *pos = strchr(buff, '|');
                while (pos != NULL) {
                    *pos = '\0';
                    pos = strchr(++pos, '|');
                }
                o_orderkey[i] = strtol(buff, &pos, 0);
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                strptime(++pos, "%Y-%m-%d", &tm);
                t = mktime(&tm);
                secs = (long) t;
                o_orderdate[i] = (short) ((secs - Q4_MINDATE) / SECONDS);
                pos = strchr(pos, '\0');
                o_orderpriority[i] = *(++pos) - '0';
                f1.getline(buff, size);
            }
            f1.close();
        }

        ifstream f2(m_files[1]);
        if (f2.is_open()) {
            int size = 256;
            char buff[size];
            struct tm tm = {0};
            time_t t;
            long secs;
            f2.getline(buff, size);
            for (int i = 0; i < Q4_LINEITEM && f2.good(); i++) {
                char *pos = strchr(buff, '|');
                while (pos != NULL) {
                    *pos = '\0';
                    pos = strchr(++pos, '|');
                }
                l_orderkey[i] = strtol(buff, &pos, 0);
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                strptime(++pos, "%Y-%m-%d", &tm);
                t = mktime(&tm);
                secs = (long) t;
                l_commitdate[i] = (short) ((secs - Q4_MINDATE) / SECONDS);
                pos = strchr(pos, '\0');
                strptime(++pos, "%Y-%m-%d", &tm);
                t = mktime(&tm);
                secs = (long) t;
                l_receiptdate[i] = (short) ((secs - Q4_MINDATE) / SECONDS);
                f2.getline(buff, size);
            }
            f2.close();
        }
    }

    virtual void execute() {
        unordered_set<int> **exists;
        m_in->receive((char *) &exists, sizeof(&exists));
        unordered_set<int> *curr = exists[m_id];
        for (int i = 0; i < Q4_LINEITEM; i++)
            if (l_commitdate[i] < l_receiptdate[i])
                curr->insert(l_orderkey[i]);
        m_out->send(NULL, 0);
        m_in->receive(NULL, 0);
        for (int i = 0; i < Q4_ORDERS; i++)
            if (o_orderdate[i] >= Q4_SDATE1 && o_orderdate[i] < Q4_SDATE2)
                for (int j = 0; j < m_numWorkers; j++) {
                    curr = exists[j];
                    if (curr->find(o_orderkey[i]) != curr->end()) {
                        char key = o_orderpriority[i];
                        m_vals[o_orderpriority[i]]++;
                        break;
                    }
                }

        m_out->send(m_vals, ?);
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
    short *o_orderdate;
    char *o_orderpriority;
    int *l_orderkey;
    short *l_commitdate;
    short *l_receiptdate;
    int *m_vals;
};
#endif
