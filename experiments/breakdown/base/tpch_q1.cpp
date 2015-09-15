#include "../tpch_q1_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    char *l_returnflag = new char[Q1_LINEITEM];
    char *l_linestatus = new char[Q1_LINEITEM];
    float *l_quantity = new float[Q1_LINEITEM];
    float *l_extendedprice = new float[Q1_LINEITEM];
    float *l_discount = new float[Q1_LINEITEM];
    float *l_tax = new float[Q1_LINEITEM];
    long *l_shipdate = new long[Q1_LINEITEM];
    init(file, l_returnflag, l_linestatus, l_quantity, l_extendedprice,
            l_discount, l_tax, l_shipdate);
    unordered_map<short,char *> *vals = new unordered_map<short,char *>();

    test(vals);
    gettimeofday(&start, NULL);
    for (int i = 0; i < Q1_LINEITEM; i++)
        if (l_shipdate[i] <= Q1_DATE1) {
            short key = l_returnflag[i] << 8;
            key |= l_linestatus[i];
            auto itr = vals->find(key);
            char *val;
            if (itr == vals->end()) {
                val = new char[Q1_VALSIZE];
                val[0] = l_returnflag[i];
                val[1] = l_linestatus[i];
                *((int *) (val + 2)) = 0;
                *((float *) (val + 6)) = 0.0f;
                *((float *) (val + 10)) = 0.0f;
                *((float *) (val + 14)) = 0.0f;
                *((float *) (val + 18)) = 0.0f;
                *((float *) (val + 22)) = 0.0f;
                (*vals)[key] = val;
            }
            else
                val = itr->second;
            *((int *) (val + 2)) += 1;
            *((float *) (val + 6)) += l_quantity[i];
            *((float *) (val + 10)) += l_extendedprice[i];
            *((float *) (val + 14)) += l_extendedprice[i] * (1 - l_discount[i]);
            *((float *) (val + 18)) += l_extendedprice[i] * (1 - l_discount[i])
                                     * (1 + l_tax[i]);
            *((float *) (val + 22)) += l_discount[i];
        }
    gettimeofday(&stop, NULL);
    test(vals);
    printTime("tpch_q1", start, stop);

    delete[] l_returnflag;
    delete[] l_linestatus;
    delete[] l_quantity;
    delete[] l_extendedprice;
    delete[] l_discount;
    delete[] l_tax;
    delete[] l_shipdate;
    delete vals;

    return 0;
}
