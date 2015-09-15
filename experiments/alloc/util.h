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

inline void printTime(int tile, struct timeval start, struct timeval stop) {
    double time = (double) stop.tv_sec + (double) stop.tv_usec * USEC
                - (double) start.tv_sec - (double) start.tv_usec * USEC;
    cout << tile << "," << time << endl;
}
