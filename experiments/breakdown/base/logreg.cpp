#include "../logreg_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *d = new float[DATA * (ATTR + 1)];
    float *w = new float[ATTR];
    float *g = new float[ATTR];
    init(file, d, w, g);

    test(g);
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA; i++) {
        int dOff = i * (ATTR + 1);

        //train
        float dot = 0.0f;
        for (int j = 0; j < ATTR; j++)
            dot += d[dOff + j] * w[j];
        float label = d[dOff + ATTR];
        float s = (1.0f / (1.0f + exp(-label * dot)) - 1.0f) * label;
        for (int j = 0; j < ATTR; j++)
            g[j] += s * d[dOff + j];
    }
    gettimeofday(&stop, NULL);
    test(g);
    printTime("logreg", start, stop);

    delete[] d;
    delete[] w;
    delete[] g;

    return 0;
}
