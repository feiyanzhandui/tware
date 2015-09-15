#include "const.h"
#include "util.h"

int main(int argc, char *argv[]) {
    vector<int *> result;
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    for (int i = 0; i < (DATA / TILE); i++)
        result.push_back(new int[TILE]);
    gettimeofday(&stop, NULL);

    printTime(TILE, start, stop);

    for (int i = 0; i < result.size(); i++)
        delete[] result[i];
    return 0;
}
