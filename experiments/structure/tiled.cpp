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
            initNeg(data1, data2, size, 10);

            int batch = size / CACHE;
            float *temp1 = new float[CACHE];
            float *temp2 = new float[CACHE];

            gettimeofday(&start, NULL);
            for (int k = 0; k < batch; k++) {
                int curr = k * CACHE;
                for (int l = 0; l < CACHE; l++) {
                    int pos = curr + l;
                    temp1[l] = data1[pos];
                    if (temp1[l] < 0)
                        temp1[l] *= -1;
                    temp2[l] = data2[pos];
                    if (temp2[l] < 0)
                        temp2[l] *= -1;
                }
                for (int l = 0; l < CACHE; l++) {
                    int pos = curr + l;
                    result[pos] = temp1[l] + temp2[l];
                }
            }
            gettimeofday(&stop, NULL);
            times[i - DMIN].push_back(getTime(start, stop));

            *test += result[0];
            delete[] data1;
            delete[] data2;
            delete[] result;
            delete[] temp1;
            delete[] temp2;
        }
    }
    delete test;

    writeTimes("tiled", times);
    return 0;
}
