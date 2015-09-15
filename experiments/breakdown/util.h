#ifndef UTIL_H
#define UTIL_H

#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

#define USEC 0.000001
#define ITER 20
#define EPS 0.01f
#define MINDATE 662764776

template <typename T>
class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual T *next() = 0;
};

inline void toDays(long *secs, short *days, int size) {
    for (int i = 0; i < size; i++)
        days[i] = (short) ((secs[i] - MINDATE) / 24 / 60 / 60);
}

inline void printTime(string algo, struct timeval start, struct timeval stop) {
    double time = (double) stop.tv_sec + (double) stop.tv_usec * USEC
                - (double) start.tv_sec - (double) start.tv_usec * USEC;
    cerr << algo << ": " << time << endl;
}
#endif
