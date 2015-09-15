#include "../bayes_util.h"

__attribute__((noinline)) void train(int *data, int *counts) {
    int label = data[ATTR];
    counts[label]++;
    int countOff = VALUE + label * ATTR * VALUE;
    for (int i = 0; i < ATTR; i++)
        counts[countOff + i * VALUE + data[i]]++;
}

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    int *data = new int[DATA * (ATTR + 1)];
    init(file, data);
    int *counts = new int[VALUE + VALUE * ATTR * VALUE];
    init(counts);

    test(counts);
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA; i++) {
        int dataOff = i * (ATTR + 1);
        train(data + dataOff, counts);
    }
    gettimeofday(&stop, NULL);
    test(counts);
    printTime("bayes", start, stop);

    delete[] data;
    delete[] counts;

    return 0;
}
