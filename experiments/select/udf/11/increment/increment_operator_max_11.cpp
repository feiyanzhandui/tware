#include "../../../const.h"
#include "../../../util.h"

int main(int argc, char *argv[]) {
    float *data1 = new float[DATA];
    initData(data1, DATA);

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    int pos = 0;
    float *buff1 = new float[DATA];
    for (int i = 0; i < DATA; i++) {
        buff1[pos] = data1[i];
        pos += (data1[i] < PVAR);
    }
    float *res1 = new float[pos];
    for (int i = 0; i < pos; i++)
        res1[i] = buff1[i] + 1;
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, PVAR, pos);
    printTime(start, stop);

    delete[] data1;
    delete[] buff1;
    delete[] res1;
    return 0;
}
