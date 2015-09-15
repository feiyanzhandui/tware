#ifndef UTIL_H
#define UTIL_H

#include <sys/time.h>

#define USEC 0.000001
#define TILE 1024
#define SOCKS 8192
#define PORT1 3998
#define PORT2 3999
#define PORT3 4000

class Util {
public:
    static double getTime(struct timeval start, struct timeval stop) {
        return (double) stop.tv_sec + (double) stop.tv_usec * USEC
             - (double) start.tv_sec - (double) start.tv_usec * USEC;
    }
};
#endif
