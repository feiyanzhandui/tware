#include "pipe.h"
#include "tware.h"
#include "util.h"

void clear(vector<float *> out) {
    for (int i = 0; i < out.size(); i++)
        delete[] out[i];
}

int main(int argc, char *argv[]) {
    double **times = new double*[5];
    for (int i = 0; i < 5; i++) {
        times[i] = new double[21];
        for (int j = 0; j < 21; j++)
            times[i][j] = 0;
    }

    int batch = DATA / CACHE;
    int extra = DATA % CACHE;

    float *in = new float[DATA];
    float *t1 = new float[CACHE];
    float *t2 = new float[CACHE * 2];
    vector<float *> out;

    for (int i = 0; i < 21; i++) {
        loadData(in, i * 5);
        for (int j = 0; j < ITER; j++) {
            startTimer();
            pipe(in, out);
            stopTimer();
            times[0][i] += getTime();
            clear(out);
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 21; j++)
            cout << (times[i][j] / ITER) << ",";
        cout << "\n";
    }

    delete[] in;
    delete[] t1;
    delete[] t2;

    for (int i = 0; i < 5; i++)
        delete[] times[i];
    delete[] times;

    return 0;
}
