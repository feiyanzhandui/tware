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
    float *tile1 = data1;
    int bitmap[TILE];
    for (int i = 0; i < TILES; i++) {
        int count = 0;
        for (int j = 0; j < TILE; j++) {
            bitmap[j] = (tile1[j] < PVAR);
            count += bitmap[j];
        }
        int end = count + pos;
        if (end > TILE) {
            int j = 0;
            for (; pos < TILE; j++) {
                res1.back()[pos] = tile1[j];
                pos += bitmap[j];
            }
            for (int k = 0; k < TILE; k++)
                res1.back()[k] = sqrtf(res1.back()[k]);
            res1.push_back(new float[TILE]);
            pos = 0;
            for (; j < count; j++) {
                res1.back()[pos] = tile1[j];
                pos += bitmap[j];
            }
        }
        else
            for (int j = 0; pos < end; j++) {
                res1.back()[pos] = tile1[j];
                pos += bitmap[j];
            }
    }
    for (int i = 0; i < pos; i++)
        res1.back()[i] = sqrtf(res1.back()[i]);
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, PVAR, res1.size());
    printTime(start, stop);

    delete[] data1;
    for (int i = 0; i < res1.size(); i++)
        delete[] res1[i];
    return 0;
}
