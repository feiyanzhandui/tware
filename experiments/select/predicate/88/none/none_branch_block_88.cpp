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
    res1.push_back(new int[TILE]);
    res2.push_back(new int[TILE]);
    res3.push_back(new int[TILE]);
    res4.push_back(new int[TILE]);
    res5.push_back(new int[TILE]);
    res6.push_back(new int[TILE]);
    res7.push_back(new int[TILE]);
    res8.push_back(new int[TILE]);
    int pos = 0;
    for (int i = 0; i < DATA; i++)
        if (data1[i] < PVAR
         && data2[i] < PMAX
         && data3[i] < PMAX
         && data4[i] < PMAX
         && data5[i] < PMAX
         && data6[i] < PMAX
         && data7[i] < PMAX
         && data8[i] < PMAX) {
            res1.back()[pos] = data1[i];
            res2.back()[pos] = data2[i];
            res3.back()[pos] = data3[i];
            res4.back()[pos] = data4[i];
            res5.back()[pos] = data5[i];
            res6.back()[pos] = data6[i];
            res7.back()[pos] = data7[i];
            res8.back()[pos++] = data8[i];
            if (pos == TILE) {
                res1.push_back(new int[TILE]);
                res2.push_back(new int[TILE]);
                res3.push_back(new int[TILE]);
                res4.push_back(new int[TILE]);
                res5.push_back(new int[TILE]);
                res6.push_back(new int[TILE]);
                res7.push_back(new int[TILE]);
                res8.push_back(new int[TILE]);
                pos = 0;
            }
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
