#include "functions.h"

inline float randf(int max) {
    return (float) (rand() % max);
}

inline int randi(int max) {
    return rand() % max;
}

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    long long start, stop;

    float x = 0;
    float y = 0;
    float *d = new float[CENT];
    int *a = new int();
    float *tx = new float[CENT];
    float *ty = new float[CENT];
    int *tc = new int[CENT];
    float *cx = new float[CENT];
    float *cy = new float[CENT];
    float *xs = new float[CENT];
    float *ys = new float[CENT];
    int *ct = new int[CENT];

    //distance
    long long total = 0;
    for (int i = 0; i < ITER; ++i) {
        x = randf(MAXR);
        y = randf(MAXR);
        for (int j = 0; j < CENT; j++) {
            cx[j] = randf(MAXR);
            cy[j] = randf(MAXR);
            d[j] = 0.0f;
        }
        __asm__ __volatile__ ("rdtsc" : "=A" (start));
        distance(x, y, cx, cy, d);
        __asm__ __volatile__ ("rdtsc" : "=A" (stop));
        long long cycles = stop - start;
        total += cycles;
    }
    cout << "distance: " << (total / ITER) << " cycles" << endl;

    //minimum
    total = 0;
    for (int i = 0; i < ITER; i++) {
        for (int j = 0; j < CENT; j++)
            d[j] = randf(MAXR);
        *a = 0;
        __asm__ __volatile__ ("rdtsc" : "=A" (start));
        minimum(d, a);
        __asm__ __volatile__ ("rdtsc" : "=A" (stop));
        long long cycles = stop - start;
        total += cycles;
    }
    cout << "minimum:  " << (total / ITER) << " cycles" << endl;

    //reassign
    total = 0;
    for (int i = 0; i < ITER; ++i) {
        *a = randi(CENT);
        x = randf(MAXR);
        y = randf(MAXR);
        for (int j = 0; j < CENT; j++) {
            tx[j] = 0.0f;
            ty[j] = 0.0f;
            tc[j] = 0;
        }
        __asm__ __volatile__ ("rdtsc" : "=A" (start));
        reassign(*a, x, y, tx, ty, tc);
        __asm__ __volatile__ ("rdtsc" : "=A" (stop));
        long long cycles = stop - start;
        total += cycles;
    }
    cout << "reassign: " << (total / ITER) << " cycles" << endl;

    //recompute
    total = 0;
    for (int i = 0; i < ITER; ++i) {
        for (int j = 0; j < CENT; j++) {
            cx[j] = randf(MAXR);
            cy[j] = randf(MAXR);
            xs[j] = randf(MAXR);
            ys[j] = randf(MAXR);
            ct[j] = randi(MAXR);
        }
        __asm__ __volatile__ ("rdtsc" : "=A" (start));
        recompute(cx, cy, xs, ys, ct);
        __asm__ __volatile__ ("rdtsc" : "=A" (stop));
        long long cycles = stop - start;
        total += cycles;
    }
    cout << "recompute: " << (total / ITER) << " cycles" << endl;

    delete d;
    delete a;
    delete cx;
    delete cy;
    delete tx;
    delete ty;
    delete tc;
    delete xs;
    delete ys;
    delete ct;

    return 0;
}
