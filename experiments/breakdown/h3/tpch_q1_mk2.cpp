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
    init(l_returnflag, l_linestatus);
    int *keys = new int[TILE];
    char *vals = new char[Q1_VALSIZE * 14];
    init(vals);
    int bitmap[TILE];

    test(vals);
    gettimeofday(&start, NULL);
    for (int i = 0; i < Q1_LINEITEM; i += TILE) {
        for (int j = 0; j < TILE; j++)
            bitmap[j] = l_shipdate[i] <= Q1_DATE1;
        for (int j = 0; j < TILE; j++)
            if (bitmap[j]) {
                int key = (int) (((l_returnflag[i] + 1) << 2) | l_linestatus[i]);
                int valOff = key * Q1_VALSIZE;
                *((int *) (vals + valOff + 2)) += bitmap[j];
                *((float *) (vals + valOff + 6)) += l_quantity[i];
                *((float *) (vals + valOff + 10)) += l_extendedprice[i];
                *((float *) (vals + valOff + 14)) += l_extendedprice[i]
                                                   * (1 - l_discount[i]);
                *((float *) (vals + valOff + 18)) += l_extendedprice[i]
                                                   * (1 - l_discount[i])
                                                   * (1 + l_tax[i]);
                *((float *) (vals + valOff + 22)) += l_discount[i];
            }
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
    delete[] keys;
    delete[] vals;

    return 0;
}
