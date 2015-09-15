#include "../bayes_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    int *data = new int[DATA * (ATTR + 1)];
    init(file, data);
    unordered_map<int,int> *counts = new unordered_map<int,int>();

    test(counts);
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA; i++) {
        int dataOff = i * (ATTR + 1);

        //train
        int label = data[dataOff + ATTR];
        (*counts)[getKey(label, ATTR, VALUE)]++;
        for (int j = 0; j < ATTR; j++)
            (*counts)[getKey(label, j, data[dataOff + j])]++;
    }
    gettimeofday(&stop, NULL);
    test(counts);
    printTime("bayes", start, stop);

    delete[] data;
    delete counts;

    return 0;
}
