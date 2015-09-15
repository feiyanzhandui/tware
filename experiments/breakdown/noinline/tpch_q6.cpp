#include "../tpch_q6_util.h"

__attribute__((noinline)) bool pred1(long l_shipdate) {
    return l_shipdate >= Q6_DATE1;
}

__attribute__((noinline)) bool pred2(long l_shipdate) {
    return l_shipdate < Q6_DATE2;
}

__attribute__((noinline)) bool pred3(float l_discount) {
    return l_discount >= 0.05f;
}

__attribute__((noinline)) bool pred4(float l_discount) {
    return l_discount <= 0.07f;
}

__attribute__((noinline)) bool pred5(float l_quantity) {
    return l_quantity < 24.0f;
}

__attribute__((noinline)) float getRevenue(float l_extendedprice,
        float l_discount) {
    return l_extendedprice * l_discount;
}

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *l_quantity = new float[Q6_LINEITEM];
    float *l_extendedprice = new float[Q6_LINEITEM];
    float *l_discount = new float[Q6_LINEITEM];
    long *l_shipdate = new long[Q6_LINEITEM];
    float revenue = 0.0f;

    test(revenue);
    init(file, l_quantity, l_extendedprice, l_discount, l_shipdate);
    gettimeofday(&start, NULL);
    for (int i = 0; i < Q6_LINEITEM; i++)
        if (pred1(l_shipdate[i])
         && pred2(l_shipdate[i])
         && pred3(l_discount[i])
         && pred4(l_discount[i])
         && pred5(l_quantity[i]))
            revenue += getRevenue(l_extendedprice[i], l_discount[i]);
    gettimeofday(&stop, NULL);
    test(revenue);
    printTime("tpch_q6", start, stop);

    delete[] l_quantity;
    delete[] l_extendedprice;
    delete[] l_discount;
    delete[] l_shipdate;

    return 0;
}
