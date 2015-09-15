#include "../../../const.h"
#include "../../../util.h"

int main(int argc, char *argv[]) {
    float *data1 = new float[DATA];
    initData(data1, DATA);

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    vector<float *> res1;
    res1.push_back(new float[TILE]);
    int pos = 0;
    for (int i = 0; i < DATA; i++)
        if (data1[i] < PVAR) {
            res1.back()[pos++] = sqrtf(data1[i]);
            if (pos == TILE) {
                res1.push_back(new float[TILE]);
                pos = 0;
            }
        }
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, PVAR, res1.size());
    printTime(start, stop);

    delete[] data1;
    for (int i = 0; i < res1.size(); i++)
        delete[] res1[i];
    return 0;
}
