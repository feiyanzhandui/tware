#include "util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    vector<vector<double> > times;
    for (int i = 0; i < DLEN; i++)
        times.push_back(vector<double>());

    int *test = new int();
    for (int i = DMIN; i < DMAX; i++) {
        int size = 1 << i;
        for (int j = 0; j < ITER; j++) {
            float *data1 = new float[size];
            float *data2 = new float[size];
            float *result = new float[size];
            init(data1, data2, size);

            gettimeofday(&start, NULL);
            for (int k = 0; k < size; k++)
                result[k] = data1[k] + data2[k];
            gettimeofday(&stop, NULL);
            times[i - DMIN].push_back(getTime(start, stop));

            *test += result[0];
            delete[] data1;
            delete[] data2;
            delete[] result;
        }
    }
    delete test;

    writeTimes(argv[1], times);
    return 0;
}
