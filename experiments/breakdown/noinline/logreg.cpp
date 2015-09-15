#include "../logreg_util.h"

__attribute__((noinline)) void train(float *d, float *w, float *g) {
    float dot = 0.0f;
    for (int i = 0; i < ATTR; i++)
        dot += d[i] * w[i];
    float label = d[ATTR];
    float scale = (1.0f / (1.0f + exp(-label * dot)) - 1.0f) * label;
    for (int i = 0; i < ATTR; i++)
        g[i] += scale * d[i];
}

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
        train(d + dOff, w, g);
    }
    gettimeofday(&stop, NULL);
    test(g);
    printTime("logreg", start, stop);

    delete[] d;
    delete[] w;
    delete[] g;

    return 0;
}
