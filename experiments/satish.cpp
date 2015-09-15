//#define VALARRAY 1

#define ITER 10000000
#define SIZE 100
#define USEC 0.000001

#include <array>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <valarray>

using namespace std;

int main() {
    struct timeval start, stop;

#ifndef VALARRAY
    float *val1 = new float[SIZE];
    float *val2 = new float[SIZE];
    float *res = new float(0.0f);
    srand((unsigned) time(NULL));
    for (int i = 0; i < SIZE; i++) {
        val1[i] = (float) rand();
        val2[i] = (float) rand();
    }
#else
    valarray<float> val1(SIZE);
    valarray<float> val2(SIZE);
    float *res = new float(0.0f);
    srand((unsigned) time(NULL));
    val1 = (float) rand();
    val2 = (float) rand();
#endif

    gettimeofday(&start, NULL);
#ifndef VALARRAY
    for (int i = 0; i < ITER; i++) {
        float dot = 0.0f;
        for (int j = 0; j < SIZE; j++)
            dot += val1[j] * val2[j];
        *res += dot;
    }

#else
    for (int i = 0; i < ITER; i++) {
        float dot = (val1 * val2).sum();
        *res += dot;
    }
#endif
    gettimeofday(&stop, NULL);
    cout << "res=" << *res << endl;

    double time = (double) stop.tv_sec + (double) stop.tv_usec * USEC
                - (double) start.tv_sec - (double) start.tv_usec * USEC;
    cout << "time=" << time << endl;

#ifndef VALARRAY
    delete val1;
    delete val2;
#endif
    delete res;
}
