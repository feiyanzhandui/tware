#include "../../const.h"
#include "../../util.h"

int main(int argc, char *argv[]) {
    string algo = "nobranch_block1";

    int *data1 = new int[DATA];
    initData(data1, DATA);

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    vector<int *> res;
    res.push_back(new int[TILE]);
    int pos = 0;
    for (int i = 0; i < DATA; i++) {
        res.back()[pos] = data1[i];
        pos += (data1[i] < PVAR);
        if (pos == TILE) {
            res.push_back(new int[TILE]);
            pos = 0;
        }
    }
    gettimeofday(&stop, NULL);

    printAlgo(algo, PVAR, res.size());
    printTime(start, stop);

    delete[] data1;
    for (int i = 0; i < res.size(); i++)
        delete[] res[i];
    return 0;
}
