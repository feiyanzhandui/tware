#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <sstream>
#include <sys/time.h>

using std::ifstream;
using std::istringstream;

#define USEC 0.000001
#define TILE 1024
#define SOCKS 8192
#define PORT1 6000
#define PORT2 6001
#define SECONDS 86400
#define LINE_SIZE 256
#define TPCH_DELIM '|'

class Util {
public:
    static double getTime(struct timeval start, struct timeval stop) {
        return (double) stop.tv_sec + (double) stop.tv_usec * USEC
             - (double) start.tv_sec - (double) start.tv_usec * USEC;
    }
};
#endif
