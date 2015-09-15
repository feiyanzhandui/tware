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
            float *data = new float[size];
            //initNeg(data, size, 5);
            init(data, size);

            gettimeofday(&start, NULL);
            for (int k = 0; k < size; k++) {
                data[k] = sqrtf(data[k]);
                if (data[k] > 1)
                    data[k] *= -1;
                //data[k] = sqrtf(data[k]);
            }
            gettimeofday(&stop, NULL);
            times[i - DMIN].push_back(getTime(start, stop));

            *test += data[0];
            delete[] data;
        }
    }
    delete test;

    writeTimes("fused", times);
    return 0;
}
