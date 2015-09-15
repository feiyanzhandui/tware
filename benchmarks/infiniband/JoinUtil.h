#ifndef JOIN_UTIL_H
#define JOIN_UTIL_H

#include <algorithm>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "Util.h"

using std::ifstream;
using std::iota;
using std::sort;
using std::string;
using std::vector;

#define DELIM '|'
#define LINE_SIZE 256
#define O_ORDERKEY sizeof(int)
#define O_ORDERDATE sizeof(long)
#define O_ORDERPRIORITY sizeof(char)
#define L_ORDERKEY sizeof(int)
#define L_COMMITDATE sizeof(long)
#define L_RECEIPTDATE sizeof(long)

inline void loadOrders(string filename, vector<int> *o_orderkey,
        vector<long> *o_orderdate, vector<char> *o_orderpriority) {
    ifstream f(filename);
    if (f.is_open()) {
        char buff[LINE_SIZE];
        struct tm tm = {0};
        f.getline(buff, LINE_SIZE);
        while (f.good()) {
            //o_orderkey
            char *pos = buff;
            char *end = strchr(pos, DELIM);
            *end = '\0';
            o_orderkey->push_back(strtol(pos, &pos, 0));

            //skip
            pos = strchr(++pos, DELIM);
            pos += 3;
            pos = strchr(pos, DELIM);

            //o_orderdate
            pos += 11;
            *pos = '\0';
            //strptime(pos - 10, "%Y-%m-%d", &tm);
            //o_orderdate->push_back((long) mktime(&tm));
            o_orderdate->push_back(123456789);

            //o_orderpriority
            o_orderpriority->push_back(*(++pos));

            //next
            f.getline(buff, LINE_SIZE);
        }
        f.close();
    }
}

inline void loadLineitem(string filename, vector<int> *l_orderkey,
        vector<long> *l_commitdate, vector<long> *l_receiptdate) {
    ifstream f(filename);
    if (f.is_open()) {
        char buff[LINE_SIZE];
        struct tm tm = {0};
        f.getline(buff, LINE_SIZE);
        while (f.good()) {
            //l_orderkey
            char *pos = buff;
            char *end = strchr(pos, DELIM);
            *end = '\0';
            l_orderkey->push_back(strtol(pos, &pos, 0));

            //skip
            pos = strchr(++pos, DELIM);
            pos = strchr(++pos, DELIM);
            pos += 3;
            pos = strchr(pos, DELIM);
            pos = strchr(++pos, DELIM);
            pos = strchr(++pos, DELIM);
            pos = strchr(++pos, DELIM);

            //l_commitdate
            pos += 15;
            //*pos = '\0';
            //strptime(pos - 10, "%Y-%m-%d", &tm);
            //l_commitdate->push_back((long) mktime(&tm));
            l_commitdate->push_back(123456789);

            //l_receiptdate
            pos += 11;
            //*pos = '\0';
            //strptime(pos - 10, "%Y-%m-%d", &tm);
            //l_receiptdate->push_back((long) mktime(&tm));
            l_receiptdate->push_back(123456789);

            //next
            f.getline(buff, LINE_SIZE);
        }
        f.close();
    }
}

inline void sortOrders(int *o_orderkey, long *o_orderdate,
        char *o_orderpriority, int *o_orderkeySort, long *o_orderdateSort,
        char *o_orderprioritySort, int size) {
    vector<int> idx(size);
    iota(idx.begin(), idx.end(), 0);

    sort(idx.begin(), idx.end(),
            [o_orderkey](int idx1, int idx2) {
                return *(o_orderkey + idx1) < *(o_orderkey + idx2);
            }
    );

    for (int i = 0; i < size; i++) {
        int pos = idx[i];
        o_orderkeySort[pos] = o_orderkey[i];
        o_orderdateSort[pos] = o_orderdate[i];
        o_orderprioritySort[pos] = o_orderpriority[i];
    }
}

inline void sortLineitem(int *l_orderkey, long *l_commitdate,
        long *l_receiptdate, int *l_orderkeySort, long *l_commitdateSort,
        long *l_receiptdateSort, int size) {
    vector<int> idx(size);
    iota(idx.begin(), idx.end(), 0);

    sort(idx.begin(), idx.end(),
            [l_orderkey](int idx1, int idx2) {
                return *(l_orderkey + idx1) < *(l_orderkey + idx2);
            }
    );

    for (int i = 0; i < size; i++) {
        int pos = idx[i];
        l_orderkeySort[pos] = l_orderkey[i];
        l_commitdateSort[pos] = l_commitdate[i];
        l_receiptdateSort[pos] = l_receiptdate[i];
    }
}

inline void merge(int *o_orderkey, long *o_orderdate, char *o_orderpriority,
        size_t ordersSize, int *l_orderkey, long *l_commitdate,
        long *l_receiptdate, size_t lineitemSize, vector<int> *r_orderkey,
        vector<long> *r_orderdate, vector<char> *r_orderpriority,
        vector<long> *r_commitdate, vector<long> *r_receiptdate) {
    if (o_orderkey[0] <= l_orderkey[lineitemSize - 1]
            && l_orderkey[0] <= o_orderkey[ordersSize - 1]) {
        for (int i = 0, j = 0; i < ordersSize && j < lineitemSize;)
            if (o_orderkey[i] < l_orderkey[j])
                i++;
            else {
                if (o_orderkey[i] == l_orderkey[j]) {
                    r_orderkey->push_back(o_orderkey[i]);
                    r_orderdate->push_back(o_orderdate[i]);
                    r_orderpriority->push_back(o_orderpriority[i]);
                    r_commitdate->push_back(l_commitdate[j]);
                    r_receiptdate->push_back(l_receiptdate[j]);
                }
                j++;
            }
    }
}

inline void merge2(int *o_orderkey, long *o_orderdate, char *o_orderpriority,
        size_t ordersSize, int *l_orderkey, long *l_commitdate,
        long *l_receiptdate, size_t lineitemSize, vector<int> *r_orderkey,
        vector<long> *r_orderdate, vector<char> *r_orderpriority,
        vector<long> *r_commitdate, vector<long> *r_receiptdate) {
    if (o_orderkey[0] <= l_orderkey[lineitemSize - 1]
            && l_orderkey[0] <= o_orderkey[ordersSize - 1]) {
        for (int i = 0, j = 0; i < ordersSize && j < lineitemSize;)
            if (o_orderkey[i] < l_orderkey[j])
                i++;
            else if (o_orderkey[i] > l_orderkey[j])
                j++;
            else {
                r_orderkey->push_back(o_orderkey[i]);
                r_orderdate->push_back(o_orderdate[i]);
                r_orderpriority->push_back(o_orderpriority[i]);
                r_commitdate->push_back(l_commitdate[j]);
                r_receiptdate->push_back(l_receiptdate[j]);
                j++;
            }
    }
}

inline void merge3(int *o_orderkey, long *o_orderdate, char *o_orderpriority,
        size_t ordersSize, int *l_orderkey, long *l_commitdate,
        long *l_receiptdate, size_t lineitemSize, vector<int> *r_orderkey,
        vector<long> *r_orderdate, vector<char> *r_orderpriority,
        vector<long> *r_commitdate, vector<long> *r_receiptdate) {
    if (o_orderkey[0] > l_orderkey[lineitemSize - 1]
            || l_orderkey[0] > o_orderkey[ordersSize - 1])
        return;

    for (int i = 0, j = 0; i < ordersSize && j < lineitemSize;) {
        int comp = o_orderkey[i] - l_orderkey[j];
        if (comp == 0) {
        //if (!comp) {
            r_orderkey->push_back(o_orderkey[i]);
            r_orderdate->push_back(o_orderdate[i]);
            r_orderpriority->push_back(o_orderpriority[i]);
            r_commitdate->push_back(l_commitdate[j]);
            r_receiptdate->push_back(l_receiptdate[j]);
        }
        i += comp < 0;
        j += comp >= 0;
    }
}
#endif
