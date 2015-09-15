#ifndef Q6_WORKER2_H
#define Q6_WORKER2_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "../Util.h"
#include "../Worker.h"
#include "../ZmqSocket.h"
#include "Q6Util.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

class Q6Worker2 : public Worker {
public:
    Q6Worker2(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        l_quantity = new float[Q6_LINEITEM];
        l_extendedprice = new float[Q6_LINEITEM];
        l_discount = new float[Q6_LINEITEM];
        l_shipdate = new long[Q6_LINEITEM];

        init(m_files[0], l_quantity, l_extendedprice, l_discount, l_shipdate);
    }

    virtual void execute() {
        m_val = new char[Q6_VALSIZE];
        float *t_quantity = l_quantity;
        float *t_extendedprice = l_extendedprice;
        float *t_discount = l_discount;
        long *t_shipdate = l_shipdate;
        float bitmap[TILE];
        float revenue = 0;
        for (int i = 0; i < Q6_LINEITEM / TILE; i++) {
            for (int j = 0; j < TILE; j++)
                bitmap[j] = (float) (t_shipdate[j] >= Q6_DATE1
                          & t_shipdate[j] < Q6_DATE2
                          & t_discount[j] >= 0.05f
                          & t_discount[j] <= 0.07f
                          & t_quantity[j] < 24.0f);
            for (int j = 0; j < TILE; j++)
                revenue += t_extendedprice[j] * t_discount[j] * bitmap[j];
            t_quantity += TILE;
            t_extendedprice += TILE;
            t_discount += TILE;
            t_shipdate += TILE;
        }
        for (int i = Q6_LINEITEM - Q6_EXTRA; i < Q6_LINEITEM; i++)
            if (l_shipdate[i] >= Q6_DATE1
             && l_shipdate[i] < Q6_DATE2
             && l_discount[i] >= 0.05f
             && l_discount[i] <= 0.07f
             && l_quantity[i] < 24.0f)
                revenue += l_extendedprice[i] * l_discount[i];
        *((float *) m_val) = revenue;
        m_out->send(m_val, Q6_VALSIZE);
    }

    virtual void cleanup() {
        delete[] m_val;
        delete[] l_quantity;
        delete[] l_extendedprice;
        delete[] l_discount;
        delete[] l_shipdate;
    }
private:
    char *m_val;
    float *l_quantity;
    float *l_extendedprice;
    float *l_discount;
    long *l_shipdate;
};
#endif
