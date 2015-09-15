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
    res1.push_back(new int[DATA]);
    res2.push_back(new int[DATA]);
    int pos = 0;
    int *tile1 = data1;
    int *tile2 = data2;
    int bitmap[TILE];
    for (int i = 0; i < TILES; i++) {
        for (int j = 0; j < TILE; j++)
            bitmap[j] = (tile1[j] < PVAR);
        for (int j = 0; j < TILE; j++) {
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
