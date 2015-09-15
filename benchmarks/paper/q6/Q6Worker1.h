#ifndef Q6_WORKER1_H
#define Q6_WORKER1_H

#include <string>
#include <vector>

#include "../Util.h"
#include "../Worker.h"
#include "../ZmqSocket.h"
#include "Q6Util.h"

using std::string;
using std::vector;

class Q6Worker1 : public Worker {
public:
    Q6Worker1(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        l_quantity = new float[Q6_LINEITEM];
        l_extendedprice = new float[Q6_LINEITEM];
        l_discount = new float[Q6_LINEITEM];
        l_shipdate = new long[Q6_LINEITEM];

        load(m_files[0], l_quantity, l_extendedprice, l_discount, l_shipdate);
    }

    virtual void execute() {
        m_val = new char[Q6_VALSIZE];
        float revenue = 0.0f;
        for (int i = 0; i < Q6_LINEITEM; i++)
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
