#include "util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    vector<vector<double> > times;
    for (int i = 0; i < DLEN; i++)
        times.push_back(vector<double>());

    float *test = new float();
    for (int i = DMIN; i < DMAX; i++) {
        int size = 1 << i;
        for (int j = 0; j < ITER; j++) {
            float *data = new float[size];
            init(data, size);

            gettimeofday(&start, NULL);
            float total = 0;
            for (int k = 0; k < size; k++)
                total += data[k];
            gettimeofday(&stop, NULL);
            times[i - DMIN].push_back(getTime(start, stop));

            *test += total;
            delete[] data;
        }
    }
    delete test;

    writeTimes(argv[1], times);
    return 0;
}
