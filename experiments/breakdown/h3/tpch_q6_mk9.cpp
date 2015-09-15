#include "../tpch_q6_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *l_quantity = new float[Q6_LINEITEM];
    float *l_extendedprice = new float[Q6_LINEITEM];
    float *l_discount = new float[Q6_LINEITEM];
    long *l_shipdate = new long[Q6_LINEITEM];
    init(file, l_quantity, l_extendedprice, l_discount, l_shipdate);
    float revenue = 0.0f;

    test(revenue);
    gettimeofday(&start, NULL);
    for (int i = 0; i < Q6_LINEITEM; i++)
        revenue += l_extendedprice[i] * l_discount[i]
                 * (l_shipdate[i] >= Q6_DATE1)
                 * (l_shipdate[i] < Q6_DATE2)
                 * (l_discount[i] >= 0.05f)
                 * (l_discount[i] <= 0.07f)
                 * (l_quantity[i] < 24.0f);
    gettimeofday(&stop, NULL);
    test(revenue);
    printTime("tpch_q6_mk9", start, stop);

    delete[] l_quantity;
    delete[] l_extendedprice;
    delete[] l_discount;
    delete[] l_shipdate;

    return 0;
}
