#include "../tpch_q4_util.h"

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
    unordered_set<int> *exists = new unordered_set<int>();
    unordered_map<char,int> *vals = new unordered_map<char,int>();

    test(vals);
    gettimeofday(&start, NULL);
    for (int i = 0; i < Q4_LINEITEM; i++)
        if (l_commitdate[i] < l_receiptdate[i])
            exists->insert(l_orderkey[i]);
    for (int i = 0; i < Q4_ORDERS; i++)
        if (o_orderdate[i] >= Q4_DATE1 && o_orderdate[i] < Q4_DATE2)
            if (exists->find(o_orderkey[i]) != exists->end())
                (*vals)[o_orderpriority[i]]++;
    gettimeofday(&stop, NULL);
    test(vals);
    printTime("tpch_q4", start, stop);

    delete[] o_orderkey;
    delete[] o_orderdate;
    delete[] o_orderpriority;
    delete[] l_orderkey;
    delete[] l_commitdate;
    delete[] l_receiptdate;
    delete exists;
    delete vals;

    return 0;
}
