#include "../bayes_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    int *data = new int[DATA * ATTR];
    int *labels = new int[DATA];
    init(file, data, labels);
    int *counts = new int[VALUE + VALUE * ATTR * VALUE];
    init(counts);
    int offs[TILE];

    test(counts);
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA; i += TILE) {
        //train
        for (int j = 0; j < TILE; j++)
            offs[j] = VALUE + labels[i + j] * ATTR * VALUE;
        for (int j = 0; j < TILE; j++)
            counts[labels[i + j]]++;
        for (int j = 0; j < TILE; j++) {
            int dataOff = (i + j) * ATTR;
            for (int k = 0; k < ATTR; k++)
                counts[offs[j] + k * VALUE + data[dataOff + k]]++;
        }
    }
    gettimeofday(&stop, NULL);
    test(counts);
    printTime("bayes", start, stop);

    delete[] data;
    delete[] labels;
    delete[] counts;

    return 0;
}
