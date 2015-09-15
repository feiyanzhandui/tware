#include "../../../const.h"
#include "../../../util.h"

int main(int argc, char *argv[]) {
    int *data1 = new int[DATA];
    initData(data1, DATA);

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    vector<int *> res1;
    res1.push_back(new int[TILE]);
    int pos = 0;
    int *tile1 = data1;
    int bitmap[TILE];
    for (int i = 0; i < TILES; i++) {
        for (int j = 0; j < TILE; j++)
            bitmap[j] = (tile1[j] < PVAR);
        for (int j = 0; j < TILE; j++)
            if (bitmap[j]) {
                res1.back()[pos++] = tile1[j];
                if (pos == TILE) {
                    res1.push_back(new int[TILE]);
                    pos = 0;
                }
            }
        tile1 += TILE;
    }
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, PVAR, res1.size());
    printTime(start, stop);

    delete[] data1;
    for (int i = 0; i < res1.size(); i++)
        delete[] res1[i];
    return 0;
}
