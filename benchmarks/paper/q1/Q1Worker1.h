#ifndef Q1_WORKER1_H
#define Q1_WORKER1_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "../Util.h"
#include "../Worker.h"
#include "../ZmqSocket.h"
#include "Q1Util.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

class Q1Worker1 : public Worker {
public:
    Q1Worker1(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        l_returnflag = new char[Q1_LINEITEM];
        l_linestatus = new char[Q1_LINEITEM];
        l_quantity = new float[Q1_LINEITEM];
        l_extendedprice = new float[Q1_LINEITEM];
        l_discount = new float[Q1_LINEITEM];
        l_tax = new float[Q1_LINEITEM];
        l_shipdate = new long[Q1_LINEITEM];

        ifstream f(m_files[0]);
        if (f.is_open()) {
            char line[LINE_SIZE];
            struct tm tm = {0};
            f.getline(line, LINE_SIZE);
            for (int i = 0; i < Q1_LINEITEM && f.good(); i++) {
                //skip
                char *pos = line;
                pos = strchr(pos, TPCH_DELIM);
                pos = strchr(++pos, TPCH_DELIM);
                pos = strchr(++pos, TPCH_DELIM);
                pos = strchr(++pos, TPCH_DELIM);

                //l_quantity
                l_quantity[i] = strtof(++pos, &pos);

                //l_extendedprice
                l_extendedprice[i] = strtof(++pos, &pos);

                //l_discount
                l_discount[i] = strtof(++pos, &pos);

                //l_tax
                l_tax[i] = strtof(++pos, &pos);

                //l_returnflag
                l_returnflag[i] = *(++pos);
                ++pos;

                //l_linestatus
                l_linestatus[i] = *(++pos);

                //l_shipdate
                pos += 11;
                *pos = '\0';
                strptime(pos - 10, "%Y-%m-%d", &tm);
                l_shipdate[i] = (long) mktime(&tm);

                //next
                f.getline(line, LINE_SIZE);
            }
            f.close();
        }
    }

    virtual void execute() {/*
        for (int i = 0; i < TPCH_LINEITEM; i++) {
            if (l_shipdate[i] <= Q1_DELTA) {
                short key = l_returnflag[i] << 8;
                key |= l_linestatus[i];
                auto itr = m_vals.find(key);
                char *val;
                if (itr == m_vals.end()) {
                    val = new char[Q1_VALSIZE];
                    val[0] = l_returnflag[i];
                    val[1] = l_linestatus[i];
                    *((int *) (val + 2)) = 0;
                    *((float *) (val + 6)) = 0.0f;
                    *((float *) (val + 10)) = 0.0f;
                    *((float *) (val + 14)) = 0.0f;
                    *((float *) (val + 18)) = 0.0f;
                    *((float *) (val + 22)) = 0.0f;
                    m_vals[key] = val;
                }
                else
                    val = itr->second;
                *((int *) (val + 2)) += 1;
                *((float *) (val + 6)) += l_quantity[i];
                *((float *) (val + 10)) += l_extendedprice[i];
                *((float *) (val + 14)) += l_extendedprice[i]
                                         * (1 - l_discount[i]);
                *((float *) (val + 18)) += l_extendedprice[i]
                                         * (1 - l_discount[i]) * (1 + l_tax[i]);
                *((float *) (val + 22)) += l_discount[i];
            }
        }
        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr)
            m_out->send(itr->second, Q1_VALSIZE);
        m_out->send(NULL, 0);*/
    }

    virtual void cleanup() {
        delete[] l_returnflag;
        delete[] l_linestatus;
        delete[] l_quantity;
        delete[] l_extendedprice;
        delete[] l_discount;
        delete[] l_tax;
        delete[] l_shipdate;
        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr)
            delete[] itr->second;
    }
private:
    char *l_returnflag;
    char *l_linestatus;
    float *l_quantity;
    float *l_extendedprice;
    float *l_discount;
    float *l_tax;
    long *l_shipdate;
    unordered_map<short,char *> m_vals;
};
#endif
