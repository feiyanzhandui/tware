#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <unordered_map>
#include <vector>

#define USEC 0.000001
#define DMIN 15
#define DMAX 28
#define DLEN 13
#define ITER 9
#define KEYS 10
#define CACHE 4096

using std::ofstream;
using std::sqrt;
using std::string;
using std::unordered_map;
using std::vector;

inline void init(int *data, int size) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        data[i] = (int) (rand() % KEYS);
}

inline double getTime(struct timeval start, struct timeval stop) {
    return (double) stop.tv_sec + (double) stop.tv_usec * USEC
         - (double) start.tv_sec - (double) start.tv_usec * USEC;
}

inline void writeTimes(string algo, vector<vector<double> > times) {
    for (int i = 0; i < DLEN; i++)
        sort(times[i].begin(), times[i].end());
    int median = ITER / 2;
    ofstream f(algo, ofstream::app);
    for (int i = 0; i < DLEN; i++)
        f << (1<<(i+DMIN)) << "," << times[i][median] << "\n";
}
#endif
