#include <cmath>
#include <emmintrin.h>
#include <iostream>
#include <smmintrin.h>
#include <sys/time.h>
#include <unordered_map>
#include <xmmintrin.h>

#define USEC 0.000001
#define DATA 10000000
#define KEYS 10000000
#define VALS 100

using namespace std;

double getTime(struct timeval start, struct timeval stop) {
    return (double) stop.tv_sec + (double) stop.tv_usec * USEC
         - (double) start.tv_sec - (double) start.tv_usec * USEC;
}

inline void stream(unordered_map<int,int> *map, int *keys, int *vals) {
    int k = 0;
    int v = 0;
    for (int i = 0; i < DATA; i++) {
        _mm_stream_si32(&k, keys[i]);
        _mm_stream_si32(&v, vals[i]);
        (*map)[k] += v;
    }
}

inline void nostream(unordered_map<int,int> *map, int *keys, int *vals) {
    for (int i = 0; i < DATA; i++)
        (*map)[keys[i]] += vals[i];
}

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    unordered_map<int,int> *map = new unordered_map<int,int>();
    int *keys = new int[DATA];
    int *vals = new int[DATA];

    srand((unsigned) time(NULL));
    for (int i = 0; i < DATA; i++)
        keys[i] = rand() % KEYS;
    for (int i = 0; i < DATA; i++)
        vals[i] = rand() % VALS;

    gettimeofday(&start, NULL);
    stream(map, keys, vals);
    //nostream(map, keys, vals);
    gettimeofday(&stop, NULL);

    cout << "**************" << endl;
    cout << "Size: " << map->size() << endl;
    cout << "Time: " << getTime(start, stop) << endl;
    cout << "**************" << endl;

    delete map;
    delete keys;
    delete vals;
    return 0;
}
