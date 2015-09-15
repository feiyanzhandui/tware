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
        cout << "\n\tLINEITEM: " << Q4_LINEITEM
             << endl;
    }

    virtual void init() {}

    virtual void execute() {
        unordered_map<short,char *> vals;
        char *val = new char[Q1_VALSIZE];
        for (int i = 0; i < m_numWorkers;) {
            if (m_in->receive(val)) {
                short key = *((short *) val);
                auto itr = vals.find(key);
                if (itr == vals.end()) {
                    vals[key] = val;
                    val = new char[Q1_VALSIZE];
                }
                else {
                    char *res = itr->second;
                    *((int *) (res + 2)) += *((int *) (val + 2));
                    *((float *) (res + 6)) += *((float *) (val + 6));
                    *((float *) (res + 10)) += *((float *) (val + 10));
                    *((float *) (res + 14)) += *((float *) (val + 14));
                    *((float *) (res + 18)) += *((float *) (val + 18));
                    *((float *) (res + 22)) += *((float *) (val + 22));
                }
            }
            else
                i++;
        }

        m_size = vals.size();
        r_returnflag = new char[m_size];
        r_linestatus = new char[m_size];
        r_sum_qty = new float[m_size];
        r_sum_base_price = new float[m_size];
        r_sum_disc_price = new float[m_size];
        r_sum_charge = new float[m_size];
        r_avg_qty = new float[m_size];
        r_avg_price = new float[m_size];
        r_avg_disc = new float[m_size];
        r_count_order = new int[m_size];
        int i = 0;
        for (auto itr = vals.begin(); itr != vals.end(); ++itr) {
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
        for (auto itr = vals.begin(); itr != vals.end(); ++itr)
            delete[] itr->second;
    }

    virtual void cleanup() {
        cout << "Q1 RESULTS\n";
        for (int i = 0; i < m_size; i++) {
            cout << "\t";
            if (r_returnflag[i] == 2)
                cout << "R,";
            else if (r_returnflag[i] == 4)
                cout << "A,";
            else
                cout << "N,";
            if (r_linestatus[i] == 0)
                cout << "O,";
            else
                cout << "F,";
                 //<< r_returnflag[i] << ","
                 //<< r_linestatus[i] << ","
            cout     << r_sum_qty[i] << ","
                 << r_sum_base_price[i] << ","
                 << r_sum_disc_price[i] << ","
                 << r_sum_charge[i] << ","
                 << r_avg_qty[i] << ","
                 << r_avg_price[i] << ","
                 << r_avg_disc[i] << ","
                 << r_count_order[i]
                 << "\n";}
        cout << endl;

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
    int m_size;
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
