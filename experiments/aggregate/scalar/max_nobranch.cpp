#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include <vector>

#include "../util.h"

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

int main(int argc, char *argv[]) {
    struct timeval start, stop;

    int *data = new int[DATA];
    //for (int i = 0; i < DATA; i++)
    //    data[i] = i;
    //data[0] = 1;
    //for (int i = 1; i < DATA; i++)
    //    data[i] = 0;
    srand((unsigned) time(NULL));
    for (int i = 0; i < DATA; i++)
        data[i] = rand() % 100;
    int max = 0;

    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA; i++)
        max += (data[i] - max) * (data[i] > max);
    gettimeofday(&stop, NULL);
    cerr << "NOBRANCH: " << max << endl;

    double time = (double) stop.tv_sec + (double) stop.tv_usec * USEC
                - (double) start.tv_sec - (double) start.tv_usec * USEC;
    cout << time;

    delete[] data;
    return 0;
}
