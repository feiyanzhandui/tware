#include "../bayes_util.h"

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

        //train
        int label = data[dataOff + ATTR];
        counts[label]++;
        int countOff = VALUE + label * ATTR * VALUE;
        for (int j = 0; j < ATTR; j++)
            counts[countOff + j * VALUE + data[dataOff + j]]++;
    }
    gettimeofday(&stop, NULL);
    test(counts);
    printTime("bayes", start, stop);

    delete[] data;
    delete[] counts;

    return 0;
}
