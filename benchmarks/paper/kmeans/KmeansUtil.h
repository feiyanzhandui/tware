#ifndef KMEANS_UTIL_H
#define KMEANS_UTIL_H

#include "../Util.h"

#define KMEANS_ITER 20
#define KMEANS_DATA 390625*2
//#define KMEANS_DATA 3906250
//#define KMEANS_DATA 39062500
//#define KMEANS_DATA 937984//29963548
#define KMEANS_ATTR 2
#define KMEANS_CENT 4
#define KMEANS_TILES KMEANS_DATA / TILE
#define KMEANS_EXTRA KMEANS_DATA % TILE
#define KMEANS_SEND1 (KMEANS_CENT * KMEANS_ATTR * sizeof(float))
#define KMEANS_SEND2 (KMEANS_CENT * (KMEANS_ATTR + 1) * sizeof(float))

inline void kmeans_init_data(string file, float *data) {
    //data
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < KMEANS_DATA && getline(f, line); i++) {
            istringstream buff(line);
            int dataOff = i * KMEANS_ATTR;
            for (int j = 0; j < KMEANS_ATTR; j++)
                buff >> data[dataOff + j] >> delim;
        }
        f.close();
    }
}

inline void kmeans_init_cent(string file, float *cent) {
    //cent
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < KMEANS_CENT && getline(f, line); i++) {
            istringstream buff(line);
            int centOff = i * KMEANS_ATTR;
            for (int j = 0; j < KMEANS_ATTR; j++)
                buff >> cent[centOff + j] >> delim;
        }
        f.close();
    }
}
#endif
