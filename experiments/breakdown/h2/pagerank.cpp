#include "../pagerank_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file1 = argv[1];
    string file2 = argv[2];

    int *links = new int[LINK * 2];
    int *counts = new int[PAGE];
    float *ranks = new float[PAGE];
    unordered_map<int,float> *temp = new unordered_map<int,float>();
    init(file1, file2, links, counts, ranks);

    test(temp);
    gettimeofday(&start, NULL);
    for (int i = 0; i < LINK; i++) {
        int linkOff = i * 2;

        //train
        int src = links[linkOff];
        int dst = links[linkOff + 1];
        (*temp)[dst] += ranks[src] / counts[src];
    }
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("pagerank", start, stop);

    delete[] links;
    delete[] counts;
    delete[] ranks;
    delete temp;

    return 0;
}
