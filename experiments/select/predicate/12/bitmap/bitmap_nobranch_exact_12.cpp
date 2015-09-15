#include "../../../const.h"
#include "../../../util.h"

int main(int argc, char *argv[]) {
    int *data1 = new int[DATA];
    int *data2 = new int[DATA];
    initData(data1, DATA);
    initData(data2, DATA);

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    vector<int *> res1;
    vector<int *> res2;
    res1.push_back(new int[TILE]);
    res2.push_back(new int[TILE]);
    int pos = 0;
    int *tile1 = data1;
    int *tile2 = data2;
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
                res2.back()[pos] = tile2[j];
                pos += bitmap[j];
            }
            res1.push_back(new int[TILE]);
            res2.push_back(new int[TILE]);
            pos = 0;
            for (; j < count; j++) {
                res1.back()[pos] = tile1[j];
                res2.back()[pos] = tile2[j];
                pos += bitmap[j];
            }
        }
        else
            for (int j = 0; pos < end; j++) {
                res1.back()[pos] = tile1[j];
                res2.back()[pos] = tile2[j];
                pos += bitmap[j];
            }
        tile1 += TILE;
        tile2 += TILE;
    }
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, PVAR, res1.size());
    printTime(start, stop);

    delete[] data1;
    delete[] data2;
    for (int i = 0; i < res1.size(); i++) {
        delete[] res1[i];
        delete[] res2[i];
    }
    return 0;
}
