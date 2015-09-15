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
            int *data = new int[size];
            unordered_map<int,int> *counts = new unordered_map<int,int>();
            init(data, size);

            gettimeofday(&start, NULL);
            int total = 0;
            for (int k = 0; k < size; k++)
                (*counts)[data[k]]++;
            gettimeofday(&stop, NULL);
            times[i - DMIN].push_back(getTime(start, stop));

            for (int k = 0; k < KEYS; k++)
                *test += (*counts)[k];
            delete[] data;
            delete counts;
        }
    }
    delete test;

    writeTimes(argv[1], times);
    return 0;
}
