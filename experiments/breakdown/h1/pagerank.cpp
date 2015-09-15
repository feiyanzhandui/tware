#include "../pagerank_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file1 = argv[1];
    string file2 = argv[2];

    int *srcs = new int[LINK];
    int *dsts = new int[LINK];
    int *counts = new int[PAGE];
    float *ranks = new float[PAGE];
    float *temp = new float[PAGE];
    init(file1, file2, srcs, dsts, counts, ranks, temp);
    float rank[TILE];
    int count[TILE];
    float div[TILE];

    test(temp);
    gettimeofday(&start, NULL);
    for (int i = 0; i < LINK - TILE; i += TILE) {
        for (int j = 0; j < TILE; j++) {
            int src = srcs[i + j];
            rank[j] = ranks[src];
            count[j] = counts[src];
        }
        for (int j = 0; j < TILE; j++)
            div[j] = rank[j] / count[j];
        for (int j = 0; j < TILE; j++) {
            int dst = dsts[i + j];
            temp[dst] += div[j];
        }
    }
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("pagerank", start, stop);

    delete[] srcs;
    delete[] dsts;
    delete[] counts;
    delete[] ranks;
    delete[] temp;

    return 0;
}
