#ifndef Q1_WORKER3_H
#define Q1_WORKER3_H

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

class Q1Worker3 : public Worker {
public:
    Q1Worker3(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
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
            string line;
            int tmpi;
            string date;
            char delim;
            srand((unsigned) time(NULL));
            for (int i = 0; i < Q1_LINEITEM && getline(f, line); i++) {
                istringstream buff(line);
                buff >> tmpi >> delim
                     >> tmpi >> delim
                     >> tmpi >> delim
                     >> tmpi >> delim
                     >> l_quantity[i] >> delim
                     >> l_extendedprice[i] >> delim
                     >> l_discount[i] >> delim
                     >> l_tax[i] >> delim
                     >> l_returnflag[i] >> delim
                     >> l_linestatus[i] >> delim
                     >> date;
                struct tm tm = {0};
                strptime(date.substr(0, 10).c_str(), "%Y-%m-%d", &tm);
                time_t t;
                t = mktime(&tm);
                l_shipdate[i] = (long) t;

                if (l_returnflag[i] == 'R')
                    l_returnflag[i] = 2;
                else if (l_returnflag[i] == 'A')
                    l_returnflag[i] = 4;
                else
                    l_returnflag[i] = 6;

                if (l_linestatus[i] == 'O')
                    l_linestatus[i] = 0;
                else
                    l_linestatus[i] = 1;
            }
            f.close();
        }

        m_vals = new char*[8];
        for (int i = 0; i < 8; i++) {
            char *val = new char[Q1_VALSIZE];
            val[0] = (i / 2) * 2;
            val[1] = i % 2;
            *((int *) (val + 2)) = 0;
            *((float *) (val + 6)) = 0.0f;
            *((float *) (val + 10)) = 0.0f;
            *((float *) (val + 14)) = 0.0f;
            *((float *) (val + 18)) = 0.0f;
            *((float *) (val + 22)) = 0.0f;
            m_vals[i] = val;
        }
    }

    virtual void execute() {
        for (int i = 0; i < Q1_LINEITEM; i++) {
            if (l_shipdate[i] <= Q1_DELTA) {
                char key = l_returnflag[i] | l_linestatus[i];
                char *val = m_vals[key];
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
        for (int i = 0; i < 8; i++)
            m_out->send(m_vals[i], Q1_VALSIZE);
        m_out->send(NULL, 0);
    }

    virtual void cleanup() {
        delete[] l_returnflag;
        delete[] l_linestatus;
        delete[] l_quantity;
        delete[] l_extendedprice;
        delete[] l_discount;
        delete[] l_tax;
        delete[] l_shipdate;
        for (int i = 0; i < 8; i++)
            delete[] m_vals[i];
        delete[] m_vals;
    }
private:
    char *l_returnflag;
    char *l_linestatus;
    float *l_quantity;
    float *l_extendedprice;
    float *l_discount;
    float *l_tax;
    long *l_shipdate;
    char **m_vals;
};
#endif
