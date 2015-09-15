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
using std::string;
using std::vector;

inline void initKeys(int *keys, int size, int distinct) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        keys[i] = rand() % distinct;
}

inline void initVals(int *vals, int size) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        vals[i] = rand() % 10;
}

inline void printAlgo(string algo, int distinct, int res) {
    cout << algo << "(" << distinct << "): " << res << endl;
}

inline void printTime(struct timeval start, struct timeval stop) {
    double time = (double) stop.tv_sec + (double) stop.tv_usec * USEC
                - (double) start.tv_sec - (double) start.tv_usec * USEC;
    cerr << time << endl;
}
