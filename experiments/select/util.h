#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <vector>
#include <sys/time.h>

#define USEC 0.000001

using std::cerr;
using std::cout;
using std::endl;
using std::sqrt;
using std::string;
using std::vector;

inline void initData(int *data, int size) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        data[i] = rand() % 100;
}

inline void initData(float *data, int size) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        data[i] = (float) (rand() % 100);
}

inline void printAlgo(string algo, int pvar, int res) {
    cout << algo << "(" << pvar << "): " << res << endl;
}

inline void printTime(struct timeval start, struct timeval stop) {
    double time = (double) stop.tv_sec + (double) stop.tv_usec * USEC
                - (double) start.tv_sec - (double) start.tv_usec * USEC;
    cerr << time << endl;
}

inline int factorial(int n) {
    int f = n;
    while (n--)
        f *= n;
    return f;
}
