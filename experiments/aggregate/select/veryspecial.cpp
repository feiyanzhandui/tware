#include "../util.h"

int main(int argc, char *argv[]) {
    int *keys = new int[DATA];
    int *vals = new int[DATA];
    initKeys(keys, DATA, KEYS);
    initVals(vals, DATA);

    int *result = new int[KEYS + 1];
    for (int i = 0; i < KEYS; i++)
        result[i] = 0;
int pvar = (KEYS / 2);
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    int *keyTile = keys;
    int *valTile = vals;
    int hashes[TILE];
    int buff[KEYS][TILE];
    int pos[KEYS + 1];
    for (int i = 0; i < KEYS; i++)
        pos[i] = 0;
    for (int i = 0; i < TILES; i++) {
        for (int j = 0; j < TILE; j++) {
            int f = keyTile[j] < pvar;
            hashes[j] = (keyTile[j] % KEYS) * f + f;
        }
        for (int j = 0; j < TILE; j++) {
            int h = hashes[j];
            //buff[h][pos[h]++] = valTile[j];
            //if (pos[h] == TILE) {
                //for (int k = 0; k < TILE; k++)
                //    result[h] += buff[h][k];
            //    pos[h] = 0;
            //}
        }
        keyTile += TILE;
        valTile += TILE;
    }
    gettimeofday(&stop, NULL);
/*
    printAlgo(__FILE__, KEYS, result[0]);
    printTime(start, stop);

    delete[] keys;
    delete[] vals;
    delete[] result;*/
    return 0;
}
