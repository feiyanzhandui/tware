#ifndef JOIN_WORKER_H
#define JOIN_WORKER_H

#include <fstream>
#include <string>
#include <vector>

#include "JoinUtil.h"
#include "Socket.h"
#include "Util.h"
#include "Worker.h"

using std::ofstream;
using std::string;
using std::vector;

#include <iostream>
using namespace std;

class JoinWorker : public Worker {
public:
    JoinWorker(Socket *in, Socket *out, string ordersFile, string lineitemFile,
            string outFile, Socket *readSock, vector<Socket *> writeSocks)
            : Worker(in, out) {
        m_ordersFile = ordersFile;
        m_lineitemFile = lineitemFile;
        m_outFile = outFile;
        m_readSock = readSock;
        m_writeSocks = writeSocks;
        m_numWorkers = m_writeSocks.size();
    }

    ~JoinWorker() {}

    virtual void init() {
        //bind
        m_readSock->bind();

        //orders
        o_orderkey = new vector<int>();
        o_orderdate = new vector<long>();
        o_orderpriority = new vector<char>();
        o_orderkeyParts = new vector<int>*[m_numWorkers];
        o_orderdateParts = new vector<long>*[m_numWorkers];
        o_orderpriorityParts = new vector<char>*[m_numWorkers];
        o_orderkeyLoc = new vector<int *>();
        o_orderdateLoc = new vector<long *>();
        o_orderpriorityLoc = new vector<char *>();
        ordersLocSize = new vector<size_t>();
        for (int i = 0; i < m_numWorkers; i++) {
            o_orderkeyParts[i] = new vector<int>();
            o_orderdateParts[i] = new vector<long>();
            o_orderpriorityParts[i] = new vector<char>();
        }

        //lineitem
        l_orderkey = new vector<int>();
        l_commitdate = new vector<long>();
        l_receiptdate = new vector<long>();
        l_orderkeyParts = new vector<int>*[m_numWorkers];
        l_commitdateParts = new vector<long>*[m_numWorkers];
        l_receiptdateParts = new vector<long>*[m_numWorkers];
        l_orderkeyLoc = new vector<int *>();
        l_commitdateLoc = new vector<long *>();
        l_receiptdateLoc = new vector<long *>();
        lineitemLocSize = new vector<size_t>();
        for (int i = 0; i < m_numWorkers; i++) {
            l_orderkeyParts[i] = new vector<int>();
            l_commitdateParts[i] = new vector<long>();
            l_receiptdateParts[i] = new vector<long>();
        }

        //result
        r_orderkey = new vector<int>();
        r_orderdate = new vector<long>();
        r_orderpriority = new vector<char>();
        r_commitdate = new vector<long>();
        r_receiptdate = new vector<long>();

        //load
        loadOrders(m_ordersFile, o_orderkey, o_orderdate, o_orderpriority);
        loadLineitem(m_lineitemFile, l_orderkey, l_commitdate, l_receiptdate);

        //connect
        for (Socket *writeSock : m_writeSocks)
            writeSock->connect();
    }

    virtual void execute() {
        //gettimeofday(&m_start1, NULL);
        //partition orders
        int numOrders = o_orderkey->size();
        for (int i = 0; i < numOrders; i++) {
            int part = (*o_orderkey)[i] % m_numWorkers;
            o_orderkeyParts[part]->push_back((*o_orderkey)[i]);
            o_orderdateParts[part]->push_back((*o_orderdate)[i]);
            o_orderpriorityParts[part]->push_back((*o_orderpriority)[i]);
        }
//cout << "here1" << endl;
        //gettimeofday(&m_stop1, NULL);
        //gettimeofday(&m_start2, NULL);
        //send orderParts
        for (int i = 0; i < m_numWorkers; i++) {
            int size = o_orderkeyParts[i]->size();

if (size != o_orderdateParts[i]->size()) cout << "orderdate!!!!" << endl;
if (size != o_orderpriorityParts[i]->size()) cout << "orderpriority!!!!" << endl;

            if (size > 0) {
                int *o_orderkeySort = new int[size];
                long *o_orderdateSort = new long[size];
                char *o_orderprioritySort = new char[size];
                sortOrders(&(*o_orderkeyParts[i])[0],
                        &(*o_orderdateParts[i])[0],
                        &(*o_orderpriorityParts[i])[0], o_orderkeySort,
                        o_orderdateSort, o_orderprioritySort, size);
                m_writeSocks[i]->sendOrders(o_orderkeySort, o_orderdateSort,
                        o_orderprioritySort, size);
            }
            else
                m_writeSocks[i]->sendOrders(NULL, NULL, NULL, 0);
        }
//cout << "here2" << endl;
        //gettimeofday(&m_stop2, NULL);
        //gettimeofday(&m_start3, NULL);
        //receive orderParts
        for (int i = 0; i < m_numWorkers; i++) {
            int *o_orderkeyRecv;
            long *o_orderdateRecv;
            char *o_orderpriorityRecv;
            int recv = m_readSock->receiveOrders(&o_orderkeyRecv,
                               &o_orderdateRecv, &o_orderpriorityRecv);
            if (recv > 0) {
                ordersLocSize->push_back(recv);
                o_orderkeyLoc->push_back(o_orderkeyRecv);
                o_orderdateLoc->push_back(o_orderdateRecv);
                o_orderpriorityLoc->push_back(o_orderpriorityRecv);
            }
        }
//cout << "here3" << endl;
        //gettimeofday(&m_stop3, NULL);
        //gettimeofday(&m_start4, NULL);
        //partition lineitem
        int numLineitem = l_orderkey->size();
        for (int i = 0; i < numLineitem; i++) {
            int part = (*l_orderkey)[i] % m_numWorkers;
            l_orderkeyParts[part]->push_back((*l_orderkey)[i]);
            l_commitdateParts[part]->push_back((*l_commitdate)[i]);
            l_receiptdateParts[part]->push_back((*l_receiptdate)[i]);
        }
//cout << "here4" << endl;
        //gettimeofday(&m_stop4, NULL);
        //gettimeofday(&m_start5, NULL);
        //send lineitemParts
        for (int i = 0; i < m_numWorkers; i++) {
            int size = l_orderkeyParts[i]->size();
            if (size > 0) {
                int *l_orderkeySort = new int[size];
                long *l_commitdateSort = new long[size];
                long *l_receiptdateSort = new long[size];
                sortLineitem(&(*l_orderkeyParts[i])[0],
                        &(*l_commitdateParts[i])[0],
                        &(*l_receiptdateParts[i])[0], l_orderkeySort,
                        l_commitdateSort, l_receiptdateSort, size);
                m_writeSocks[i]->sendLineitem(l_orderkeySort, l_commitdateSort,
                        l_receiptdateSort, size);
            }
            else
                m_writeSocks[i]->sendLineitem(NULL, NULL, NULL, 0);
        }
//cout << "here5" << endl;
        //gettimeofday(&m_stop5, NULL);
        //gettimeofday(&m_start6, NULL);
        //receive lineitemParts
        for (int i = 0; i < m_numWorkers; i++) {
            int *l_orderkeyRecv;
            long *l_commitdateRecv;
            long *l_receiptdateRecv;
            int recv = m_readSock->receiveLineitem(&l_orderkeyRecv,
                               &l_commitdateRecv, &l_receiptdateRecv);
            if (recv > 0) {
                lineitemLocSize->push_back(recv);
                l_orderkeyLoc->push_back(l_orderkeyRecv);
                l_commitdateLoc->push_back(l_commitdateRecv);
                l_receiptdateLoc->push_back(l_receiptdateRecv);
            }
        }
//cout << "here6" << endl;
        //gettimeofday(&m_stop6, NULL);
        //gettimeofday(&m_start3, NULL);
        //merge
        int numOrdersBlks = o_orderkeyLoc->size();
        for (int i = 0; i < numOrdersBlks; i++) {
            int numLineitemBlks = l_orderkeyLoc->size();
            for (int j = 0; j < numLineitemBlks; j++)
                merge((*o_orderkeyLoc)[i], (*o_orderdateLoc)[i],
                        (*o_orderpriorityLoc)[i], (*ordersLocSize)[i],
                        (*l_orderkeyLoc)[j], (*l_commitdateLoc)[j],
                        (*l_receiptdateLoc)[j], (*lineitemLocSize)[j],
                        r_orderkey, r_orderdate, r_orderpriority, r_commitdate,
                        r_receiptdate);
        }
//gettimeofday(&m_sort1, NULL);
//gettimeofday(&m_sort2, NULL);
//cout << "lsort:  " + to_string(Util::getTime(m_sort1, m_sort2)) << endl;
    }

    virtual void cleanup() {
        //write output
        ofstream f(m_outFile);
        if (f.is_open()) {
            for (int i = 0; i < r_orderkey->size(); i++)
                f << (*r_orderkey)[i] << DELIM
                  << (*r_orderdate)[i] << DELIM
                  << (*r_orderpriority)[i] << DELIM
                  << (*r_commitdate)[i] << DELIM
                  << (*r_receiptdate)[i] << '\n';
            f.close();
        }

        //orders
        for (int i = 0; i < m_numWorkers; i++) {
            delete o_orderkeyParts[i];
            delete o_orderdateParts[i];
            delete o_orderpriorityParts[i];
        }
        for (int i = 0; i < ordersLocSize->size(); i++) {
            delete[] (*o_orderkeyLoc)[i];
            delete[] (*o_orderdateLoc)[i];
            delete[] (*o_orderpriorityLoc)[i];
        }
        delete o_orderkey;
        delete o_orderdate;
        delete o_orderpriority;
        delete o_orderkeyParts;
        delete o_orderpriorityParts;
        delete o_orderkeyLoc;
        delete o_orderdateLoc;
        delete o_orderpriorityLoc;
        delete ordersLocSize;

        //lineitem
        for (int i = 0; i < m_numWorkers; i++) {
            delete l_orderkeyParts[i];
            delete l_commitdateParts[i];
            delete l_receiptdateParts[i];
        }
        for (int i = 0; i < lineitemLocSize->size(); i++) {
            delete[] (*l_orderkeyLoc)[i];
            delete[] (*l_commitdateLoc)[i];
            delete[] (*l_receiptdateLoc)[i];
        }
        delete l_orderkey;
        delete l_commitdate;
        delete l_receiptdate;
        delete l_orderkeyParts;
        delete l_commitdateParts;
        delete l_receiptdateParts;
        delete l_orderkeyLoc;
        delete l_commitdateLoc;
        delete l_receiptdateLoc;
        delete lineitemLocSize;

        //result
        delete r_orderkey;
        delete r_orderdate;
        delete r_orderpriority;
        delete r_commitdate;
        delete r_receiptdate;

        //socks
        for (Socket *writeSock : m_writeSocks)
            writeSock->close();
        m_readSock->close();

        //cout << "\n*** RUN INFO ***"
        //     << "\n\ttime1: " << Util::getTime(m_start1, m_stop1)
        //     << "\n\ttime2: " << Util::getTime(m_start2, m_stop2)
        //     << "\n\ttime3: " << Util::getTime(m_start3, m_stop3)
        //     << endl;
    }
private:
    string m_ordersFile;
    string m_lineitemFile;
    string m_outFile;
    Socket *m_readSock;
    vector<Socket *> m_writeSocks;
    int m_numWorkers;

    //orders
    vector<int> *o_orderkey;
    vector<long> *o_orderdate;
    vector<char> *o_orderpriority;
    vector<int> **o_orderkeyParts;
    vector<long> **o_orderdateParts;
    vector<char> **o_orderpriorityParts;
    vector<int *> *o_orderkeyLoc;
    vector<long *> *o_orderdateLoc;
    vector<char *> *o_orderpriorityLoc;
    vector<size_t> *ordersLocSize;

    //lineitem
    vector<int> *l_orderkey;
    vector<long> *l_commitdate;
    vector<long> *l_receiptdate;
    vector<int> **l_orderkeyParts;
    vector<long> **l_commitdateParts;
    vector<long> **l_receiptdateParts;
    vector<int *> *l_orderkeyLoc;
    vector<long *> *l_commitdateLoc;
    vector<long *> *l_receiptdateLoc;
    vector<size_t> *lineitemLocSize;

    //result
    vector<int> *r_orderkey;
    vector<long> *r_orderdate;
    vector<char> *r_orderpriority;
    vector<long> *r_commitdate;
    vector<long> *r_receiptdate;
/*    struct timeval m_start1;
    struct timeval m_stop1;
    struct timeval m_start2;
    struct timeval m_stop2;
    struct timeval m_start3;
    struct timeval m_stop3;
    struct timeval m_start4;
    struct timeval m_stop4;
    struct timeval m_start5;
    struct timeval m_stop5;
    struct timeval m_start6;
    struct timeval m_stop6;
    struct timeval m_start7;
    struct timeval m_stop7;*/
};
#endif
