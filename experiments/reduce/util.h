#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <vector>

#define USEC 0.000001
#define DMIN 15
#define DMAX 28
#define DLEN 13
#define ITER 9
#define CACHE 4096

using std::ofstream;
using std::sqrt;
using std::string;
using std::vector;

inline void init(float *data, int size) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        data[i] = rand() % 10;
}

inline void initNeg(float *data, int size, int neg) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++) {
        int val = rand() % 100;
        data[i] = rand() % 100 < neg ? -val : val;
    }
}

inline void init(float *data1, float *data2, int size) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        data1[i] = data2[i] = rand() % 100;
}

inline void initNeg(float *data1, float *data2, int size, int neg) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++) {
        int val = rand() % 100;
        data1[i] = data2[i] = rand() % 100 < neg ? -val : val;
    }
}

inline double getTime(struct timeval start, struct timeval stop) {
    return (double) stop.tv_sec + (double) stop.tv_usec * USEC
         - (double) start.tv_sec - (double) start.tv_usec * USEC;
}

inline void writeTimes(string algo, vector<vector<double> > times) {
    for (int i = 0; i < DLEN; i++)
        sort(times[i].begin(), times[i].end());
    int median = ITER / 2;
    ofstream f("results.csv", ofstream::app);
    f << algo;
    for (int i = 0; i < DLEN; i++)
        f << "," << times[i][median];
    f << "\n";
}
#endif
