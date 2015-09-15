#include "const.h"
#include "util.h"

int main(int argc, char *argv[]) {
    int *keys = new int[DATA];
    int *vals = new int[DATA];
    initKeys(keys, DATA, KEYS);
    initVals(vals, DATA);

    int *result = new int[KEYS];
    for (int i = 0; i < KEYS; i++)
        result[i] = 0;

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    int *keyTile = keys;
    int *valTile = vals;
    int hashes[TILE];
    for (int i = 0; i < TILES; i++) {
        for (int j = 0; j < TILE; j++)
            hashes[j] = keyTile[j] % KEYS;
        for (int j = 0; j < TILE; j++)
            result[hashes[j]] += valTile[j];
        keyTile += TILE;
        valTile += TILE;
    }
    gettimeofday(&stop, NULL);

    printAlgo(__FILE__, KEYS, result[0]);
    printTime(start, stop);

    delete[] keys;
    delete[] vals;
    delete[] result;
    return 0;
}
