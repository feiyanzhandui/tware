#include "../../../const.h"
#include "../../../util.h"

int main(int argc, char *argv[]) {
    int *data1 = new int[DATA];
    initData(data1, DATA);

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    vector<int *> res1;
    res1.push_back(new int[DATA]);
    int pos = 0;
    for (int i = 0; i < DATA; i++) {
        res1.back()[pos] = data1[i];
        pos += (data1[i] < PVAR);
    }
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, PVAR, res1.size());
    printTime(start, stop);

    delete[] data1;
    for (int i = 0; i < res1.size(); i++)
        delete[] res1[i];
    return 0;
}
