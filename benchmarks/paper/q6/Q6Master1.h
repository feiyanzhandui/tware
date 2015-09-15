#ifndef Q6_MASTER1_H
#define Q6_MASTER1_H

#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>

#include "../Master.h"
#include "../ZmqSocket.h"
#include "Q6Util.h"

using std::string;
using std::vector;
using std::unordered_map;

class Q6Master1 : public Master {
public:
    Q6Master1(int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Master(numWorkers, in, out, files) {}

    virtual void printParams() {
        cout << "\n\tDATA: " << Q6_LINEITEM
             << endl;
    }

    virtual void init() {}

    virtual void execute() {
        r_revenue = new float();
        float *val = new float();
        for (int i = 0; i < m_numWorkers; i++) {
            m_in->receive((char *) val);
            *r_revenue += *val;
        }
        delete val;
    }

    virtual void cleanup() {
        cout << "Q6 RESULTS\n";
        cout << "\tr_revenue=" << *r_revenue << endl;

        delete[] r_revenue;
    }
private:
    float *r_revenue;
};
#endif
