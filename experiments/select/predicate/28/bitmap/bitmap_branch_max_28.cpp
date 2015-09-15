#include "../../../const.h"
#include "../../../util.h"

int main(int argc, char *argv[]) {
    int *data1 = new int[DATA];
    int *data2 = new int[DATA];
    int *data3 = new int[DATA];
    int *data4 = new int[DATA];
    int *data5 = new int[DATA];
    int *data6 = new int[DATA];
    int *data7 = new int[DATA];
    int *data8 = new int[DATA];
    initData(data1, DATA);
    initData(data2, DATA);
    initData(data3, DATA);
    initData(data4, DATA);
    initData(data5, DATA);
    initData(data6, DATA);
    initData(data7, DATA);
    initData(data8, DATA);

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    vector<int *> res1;
    vector<int *> res2;
    vector<int *> res3;
    vector<int *> res4;
    vector<int *> res5;
    vector<int *> res6;
    vector<int *> res7;
    vector<int *> res8;
    res1.push_back(new int[DATA]);
    res2.push_back(new int[DATA]);
    res3.push_back(new int[DATA]);
    res4.push_back(new int[DATA]);
    res5.push_back(new int[DATA]);
    res6.push_back(new int[DATA]);
    res7.push_back(new int[DATA]);
    res8.push_back(new int[DATA]);
    int pos = 0;
    int *tile1 = data1;
    int *tile2 = data2;
    int *tile3 = data3;
    int *tile4 = data4;
    int *tile5 = data5;
    int *tile6 = data6;
    int *tile7 = data7;
    int *tile8 = data8;
    int bitmap[TILE];
    for (int i = 0; i < TILES; i++) {
        for (int j = 0; j < TILE; j++)
            bitmap[j] = (tile1[j] < PVAR)
                      & (tile2[j] < PMAX);
        for (int j = 0; j < TILE; j++)
            if (bitmap[j]) {
                res1.back()[pos] = tile1[j];
                res2.back()[pos] = tile2[j];
                res3.back()[pos] = tile3[j];
                res4.back()[pos] = tile4[j];
                res5.back()[pos] = tile5[j];
                res6.back()[pos] = tile6[j];
                res7.back()[pos] = tile7[j];
                res8.back()[pos++] = tile8[j];
            }
        tile1 += TILE;
        tile2 += TILE;
        tile3 += TILE;
        tile4 += TILE;
        tile5 += TILE;
        tile6 += TILE;
        tile7 += TILE;
        tile8 += TILE;
    }
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, PVAR, res1.size());
    printTime(start, stop);

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete[] data6;
    delete[] data7;
    delete[] data8;
    for (int i = 0; i < res1.size(); i++) {
        delete[] res1[i];
        delete[] res2[i];
        delete[] res3[i];
        delete[] res4[i];
        delete[] res5[i];
        delete[] res6[i];
        delete[] res7[i];
        delete[] res8[i];
    }
    return 0;
}
