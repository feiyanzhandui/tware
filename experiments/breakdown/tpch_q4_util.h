#include "util.h"

#define TILE 1024
#define Q4_LINE_SIZE 256
#define Q4_DELIM '|'
#define Q4_ORDERS 15000000
#define Q4_LINEITEM 59986052
#define Q4_DATE1 741484800
#define Q4_DATE2 749433600
#define Q4_VALSIZE 5
#define Q4_PRIORITY 5

inline void init(string file1, string file2, int *o_orderkey, long *o_orderdate,
        char *o_orderpriority, int *l_orderkey, long *l_commitdate,
        long *l_receiptdate) {
    //orders
    ifstream f1(file1);
    if (f1.is_open()) {
        char line[Q4_LINE_SIZE];
        struct tm tm = {0};
        f1.getline(line, Q4_LINE_SIZE);
        for (int i = 0; i < Q4_ORDERS && f1.good(); i++) {
            //o_orderkey
            char *pos = line;
            o_orderkey[i] = strtol(pos, &pos, 0);

            //skip
            pos = strchr(++pos, Q4_DELIM);
            pos += 3;
            pos = strchr(pos, Q4_DELIM);

            //o_orderdate
            pos += 11;
            *pos = '\0';
            strptime(pos - 10, "%Y-%m-%d", &tm);
            o_orderdate[i] = (long) mktime(&tm);

            //o_orderpriority
            o_orderpriority[i] = *(++pos) - '1';

            //next
            f1.getline(line, Q4_LINE_SIZE);
        }
        f1.close();
    }

    //lineitem
    ifstream f2(file2);
    if (f2.is_open()) {
        char line[Q4_LINE_SIZE];
        struct tm tm = {0};
        f2.getline(line, Q4_LINE_SIZE);
        for (int i = 0; i < Q4_LINEITEM && f2.good(); i++) {
            //l_orderkey
            char *pos = line;
            l_orderkey[i] = strtol(pos, &pos, 0);

            //skip
            pos = strchr(++pos, Q4_DELIM);
            pos = strchr(++pos, Q4_DELIM);
            pos += 3;
            pos = strchr(pos, Q4_DELIM);
            pos = strchr(++pos, Q4_DELIM);
            pos = strchr(++pos, Q4_DELIM);
            pos = strchr(++pos, Q4_DELIM);

            //l_commitdate
            pos += 26;
            *pos = '\0';
            strptime(pos - 10, "%Y-%m-%d", &tm);
            l_commitdate[i] = (long) mktime(&tm);

            //l_receiptdate
            pos += 11;
            *pos = '\0';
            strptime(pos - 10, "%Y-%m-%d", &tm);
            l_receiptdate[i] = (long) mktime(&tm);

            //next
            f2.getline(line, Q4_LINE_SIZE);
        }
        f2.close();
    }
}

inline void test(unordered_map<char,int> *vals) {
    cout << "vals:\n";
    for (auto itr = vals->begin(); itr != vals->end(); ++itr)
        cout << "\tvals[" << itr->first << "]=" << itr->second << "\n";
    cout << endl;
}

inline void test(int *vals) {
}
/*
inline void test(unordered_map<int,float> *temp) {
    cout << "temp:\n";
    //for (int i = 0; i < 20; i++)
    //    cout << "\ttemp[" << i << "]=" << temp[i] << "\n";
    cout << endl;
}*/
