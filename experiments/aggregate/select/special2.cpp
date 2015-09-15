#include "../util.h"

int main(int argc, char *argv[]) {
    int *keys = new int[DATA];
    int *vals = new int[DATA];
    initKeys(keys, DATA, KEYS);
    initVals(vals, DATA);

    int *result = new int[KEYS + 1];
    for (int i = 0; i < KEYS; i++)
        result[i] = 0;

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA; i++) {
        int f = keys[i] < (KEYS / 2);
        int hash = (keys[i] % KEYS) * f + f;
        result[hash] += vals[i];
    }
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, KEYS, result[0]);
    printTime(start, stop);

    delete[] keys;
    delete[] vals;
    delete[] result;
    return 0;
}
