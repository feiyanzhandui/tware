#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdint.h>

#define ITER 10000000
#define CENT 4
#define MAXR 1000

using std::cout;
using std::endl;

inline void distance(float x, float y, float *cx, float *cy, float *d) {
    for (int i = 0; i < CENT; i++) {
        float dx = cx[i] - x;
        float dy = cy[i] - y;
        d[i] = sqrtf(dx * dx + dy * dy);
    }
}

inline void minimum(float *d, int *a) {
    float m = d[0];
    *a = 0;
    for (int i = 1; i < CENT; i++) {
        float n = d[i];
        if (n < m) {
            m = n;
            *a = i;
        }
    }
}

inline void reassign(int a, float x, float y, float *tx, float *ty, int *tc) {
    tx[a] += x;
    ty[a] += y;
    ++tc[a];
}

inline void recompute(float *cx, float *cy, float *xs, float *ys, int *ct) {
    for (int i = 0; i < CENT; i++)
        if (ct[i] > 0) {
            cx[i] = xs[i] / ct[i];
            cy[i] = ys[i] / ct[i];
        }
}
