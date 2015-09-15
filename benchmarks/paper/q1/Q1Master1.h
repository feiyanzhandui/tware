#ifndef Q1_MASTER1_H
#define Q1_MASTER1_H

#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>

#include "../Master.h"
#include "../ZmqSocket.h"
#include "Q1Util.h"

using std::string;
using std::vector;
using std::unordered_map;

class Q1Master1 : public Master {
public:
    Q1Master1(int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tLINEITEM: " << Q1_LINEITEM
             << endl;
    }

    virtual void init() {}

    virtual void execute() {
        char recv[Q1_VALSIZE];
        for (int i = 0; i < m_numWorkers;) {
            if (m_in->receive(recv)) {
                short key = *((short *) recv);
                auto itr = m_vals.find(key);
                if (itr == m_vals.end()) {
                    char *val = new char[Q1_VALSIZE];
                    memcpy(val, recv, Q1_VALSIZE);
                    m_vals[key] = val;
                }
                else {
                    char *val = itr->second;
                    *((int *) (val + 2)) += *((int *) (recv + 2));
                    *((float *) (val + 6)) += *((float *) (recv + 6));
                    *((float *) (val + 10)) += *((float *) (recv + 10));
                    *((float *) (val + 14)) += *((float *) (recv + 14));
                    *((float *) (val + 18)) += *((float *) (recv + 18));
                    *((float *) (val + 22)) += *((float *) (recv + 22));
                }
            }
            else
                i++;
        }

        int size = m_vals.size();
        r_returnflag = new char[size];
        r_linestatus = new char[size];
        r_sum_qty = new float[size];
        r_sum_base_price = new float[size];
        r_sum_disc_price = new float[size];
        r_sum_charge = new float[size];
        r_avg_qty = new float[size];
        r_avg_price = new float[size];
        r_avg_disc = new float[size];
        r_count_order = new int[size];
        int i = 0;
        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr) {
            char *val = itr->second;
            r_returnflag[i] = val[0];
            r_linestatus[i] = val[1];
            r_count_order[i] = *((int *) (val + 2));
            r_sum_qty[i] = *((float *) (val + 6));
            r_sum_base_price[i] = *((float *) (val + 10));
            r_sum_disc_price[i] = *((float *) (val + 14));
            r_sum_charge[i] = *((float *) (val + 18));
            r_avg_qty[i] = r_sum_qty[i] / r_count_order[i];
            r_avg_price[i] = r_sum_base_price[i] / r_count_order[i];
            r_avg_disc[i] = *((float *) (val + 22)) / r_count_order[i];
            i++;
        }
    }

    virtual void cleanup() {
        cout << "Q1 RESULTS\n";
        for (int i = 0; i < m_vals.size(); i++)
            cout << "\t"
                 << r_returnflag[i] << ","
                 << r_linestatus[i] << ","
                 << r_sum_qty[i] << ","
                 << r_sum_base_price[i] << ","
                 << r_sum_disc_price[i] << ","
                 << r_sum_charge[i] << ","
                 << r_avg_qty[i] << ","
                 << r_avg_price[i] << ","
                 << r_avg_disc[i] << ","
                 << r_count_order[i]
                 << "\n";
        cout << endl;

        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr)
            delete[] itr->second;
        delete[] r_returnflag;
        delete[] r_linestatus;
        delete[] r_sum_qty;
        delete[] r_sum_base_price;
        delete[] r_sum_disc_price;
        delete[] r_sum_charge;
        delete[] r_avg_qty;
        delete[] r_avg_price;
        delete[] r_avg_disc;
        delete[] r_count_order;
    }
private:
    unordered_map<short,char *> m_vals;
    char *r_returnflag;
    char *r_linestatus;
    float *r_sum_qty;
    float *r_sum_base_price;
    float *r_sum_disc_price;
    float *r_sum_charge;
    float *r_avg_qty;
    float *r_avg_price;
    float *r_avg_disc;
    int *r_count_order;
};
#endif
