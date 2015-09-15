#include "../logreg_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *d = new float[DATA * ATTR];
    float *l = new float[DATA];
    float *w = new float[ATTR];
    float *g = new float[ATTR];
    init(file, d, w, g, l);
    float s[TILE];
    float dots[TILE];

    test(g);
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA; i += TILE) {
        for (int j = 0; j < TILE; j++) {
            int dOff = (i + j) * ATTR;
            dots[j] = 0.0f;
            for (int k = 0; k < ATTR; k++)
                dots[j] += d[dOff + k] * w[k];
        }
        for (int j = 0; j < TILE; j++) {
            int label = l[i + j];
            s[j] = (1.0f / (1.0f + exp(-label * dots[j])) - 1.0f) * label;
        }
        for (int j = 0; j < TILE; j++) {
            for (int k = 0; k < ATTR; k++) {
                g[k] += s[j] * d[(i + j) * ATTR + k];
            }
        }
    }
    gettimeofday(&stop, NULL);
    test(g);
    printTime("logreg", start, stop);

    delete[] d;
    delete[] l;
    delete[] w;
    delete[] g;

    return 0;
}
