#include "../pagerank_util.h"

__attribute__((noinline)) void train(int *links, int *counts, float *ranks,
        float *temp) {
    int src = links[0];
    int dst = links[1];
    temp[dst] += ranks[src] / counts[src];
}

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file1 = argv[1];
    string file2 = argv[2];

    int *links = new int[LINK * 2];
    int *counts = new int[PAGE];
    float *ranks = new float[PAGE];
    float *temp = new float[PAGE];
    init(file1, file2, links, counts, ranks, temp);

    test(temp);
    gettimeofday(&start, NULL);
    for (int i = 0; i < LINK * 2; i += 2)
        train(links + i, counts, ranks, temp);
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("pagerank", start, stop);

    delete[] links;
    delete[] counts;
    delete[] ranks;
    delete[] temp;

    return 0;
}
