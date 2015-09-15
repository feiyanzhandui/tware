#ifndef Q1_WORKER2_H
#define Q1_WORKER2_H

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

class Q1Worker2 : public Worker {
public:
    Q1Worker2(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        l_returnflag = new char[Q1_LINEITEM];
        l_linestatus = new char[Q1_LINEITEM];
        l_quantity = new float[Q1_LINEITEM];
        l_extendedprice = new float[Q1_LINEITEM];
        l_discount = new float[Q1_LINEITEM];
        l_tax = new float[Q1_LINEITEM];
        l_shipdate = new short[Q1_LINEITEM];

        ifstream f(m_files[0]);
        if (f.is_open()) {
            int size = 256;
            char buff[size];
            struct tm tm = {0};
            time_t t;
            f.getline(buff, size);
            for (int i = 0; i < Q1_LINEITEM && f.good(); i++) {
                char *pos = strchr(buff, '|');
                while (pos != NULL) {
                    *pos = '\0';
                    pos = strchr(++pos, '|');
                }
                pos = strchr(buff, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                l_quantity[i] = strtof(++pos, &pos);
                l_extendedprice[i] = strtof(++pos, &pos);
                l_discount[i] = strtof(++pos, &pos);
                l_tax[i] = strtof(++pos, &pos);
                l_returnflag[i] = *(++pos);
                pos = strchr(++pos, '\0');
                l_linestatus[i] = *(++pos);
                pos = strchr(++pos, '\0');
                pos = strchr(++pos, '\0');
                strptime(++pos, "%Y-%m-%d", &tm);
                t = mktime(&tm);
                secs = (long) t;
                l_shiptdate[i] = (short) ((secs - Q4_MINDATE) / SECONDS);
                f.getline(buff, size);
            }
            f.close();
        }
    }

    virtual void execute() {
        char *t_returnflag = l_returnflag;
        char *t_linestatus = l_linestatus;
        float *t_quantity = l_quantity;
        float *t_extendedprice = l_extendedprice;
        float *t_discount = l_discount;
        float *t_tax = l_tax;
        long *t_shipdate = l_shipdate;
        int bitmap[TILE];
        for (int i = 0; i < (Q1_LINEITEM / TILE); i++) {
            for (int j = 0; j < TILE; j++)
                bitmap[j] = t_shipdate[j] <= Q1_DELTA;
            for (int j = 0; j < TILE; j++)
                if (bitmap[j]) {
                    short key = t_returnflag[j] << 8;
                    key |= t_linestatus[j];
                    auto itr = m_vals.find(key);
                    char *val;
                    if (itr == m_vals.end()) {
                        val = new char[Q1_VALSIZE];
                        val[0] = t_returnflag[j];
                        val[1] = t_linestatus[j];
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
                    *((float *) (val + 6)) += t_quantity[j];
                    *((float *) (val + 10)) += t_extendedprice[j];
                    *((float *) (val + 14)) += t_extendedprice[j]
                                             * (1 - t_discount[j]);
                    *((float *) (val + 18)) += t_extendedprice[j]
                                             * (1 - t_discount[j])
                                             * (1 + t_tax[j]);
                    *((float *) (val + 22)) += t_discount[j];
                }
            t_returnflag += TILE;
            t_linestatus += TILE;
            t_quantity += TILE;
            t_extendedprice += TILE;
            t_discount += TILE;
            t_tax += TILE;
            t_shipdate += TILE;
        }
        for (int i = Q1_LINEITEM - (Q1_LINEITEM % TILE); i < Q1_LINEITEM; i++)
            if (l_shipdate[i] <= Q1_DELTA) {
cout << "i=" << i << endl;
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
        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr)
            m_out->send(itr->second, Q1_VALSIZE);
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
