#include "../tpch_q4_util.h"

#define KEYS (TILE * 128)

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file1 = argv[1];
    string file2 = argv[2];

    int *o_orderkey = new int[Q4_ORDERS];
    long *o_orderdate = new long[Q4_ORDERS];
    char *o_orderpriority = new char[Q4_ORDERS];
    int *l_orderkey = new int[Q4_LINEITEM];
    long *l_commitdate = new long[Q4_LINEITEM];
    long *l_receiptdate = new long[Q4_LINEITEM];
    init(file1, file2, o_orderkey, o_orderdate, o_orderpriority, l_orderkey,
            l_commitdate, l_receiptdate);
    vector<int> *exists = new vector<int>[KEYS + 1];
    int *keys = new int[TILE];
    unordered_map<char,int> *vals = new unordered_map<char,int>();

    test(vals);
    gettimeofday(&start, NULL);
    for (int i = 0; i < Q4_LINEITEM; i += TILE) {
        for (int j = 0; j < TILE; j++) {
            int cmp = l_commitdate[i + j] < l_receiptdate[i + j];
            keys[j] = (l_orderkey[i + j] % KEYS) * cmp + cmp;
        }
        for (int j = 0; j < TILE; j++)
            exists[keys[j]].push_back(l_orderkey[i + j]);
    }
    for (int i = 0; i < Q4_ORDERS; i++)
        if (o_orderdate[i] >= Q4_DATE1 && o_orderdate[i] < Q4_DATE2) {
            int key = o_orderkey[i] % KEYS + 1;
            for (int j = 0; j < exists[key].size(); j++)
                if (o_orderkey[i] == exists[key][j]) {
                    (*vals)[o_orderpriority[i]]++;
                    break;
                }
        }
    gettimeofday(&stop, NULL);
    test(vals);
    printTime("tpch_q4", start, stop);

    delete[] o_orderkey;
    delete[] o_orderdate;
    delete[] o_orderpriority;
    delete[] l_orderkey;
    delete[] l_commitdate;
    delete[] l_receiptdate;
    delete[] exists;
    delete[] keys;
    delete vals;

    return 0;
}
