#include "util.h"

#define TILE 128
#define DATA 86016//2747253
#define ATTR 90

inline void init(string file, float *d, float *w, float *g) {
    //d
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < DATA && getline(f, line); i++) {
            int dOff = i * (ATTR + 1);
            istringstream buff(line);
            for (int j = 0; j <= ATTR; j++)
                buff >> d[dOff + j] >> delim;
        }
        f.close();
    }

    //w + g
    for (int i = 0; i < ATTR; i++) {
        w[i] = 0.5f;
        g[i] = 0.0f;
    }
}

inline void init(string file, float *d, unordered_map<int,float> *w,
        unordered_map<int,float> *g) {
    //d
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char c;
        for (int i = 0; i < DATA && getline(f, line); i++) {
            int dOff = i * (ATTR + 1);
            istringstream buff(line);
            for (int j = 0; j <= ATTR; j++)
                buff >> d[dOff + j] >> c;
        }
        f.close();
    }

    //w + g
    for (int i = 0; i < ATTR; i++) {
        (*w)[i] = 0.5f;
        (*g)[i] = 0.0f;
    }
}

inline void init(string file, float *d, float *w, float *g, float *l) {
    //d
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < DATA && getline(f, line); i++) {
            int dOff = i * ATTR;
            istringstream buff(line);
            for (int j = 0; j < ATTR; j++)
                buff >> d[dOff + j] >> delim;
            buff >> l[i];
        }
        f.close();
    }

    //w + g
    for (int i = 0; i < ATTR; i++) {
        w[i] = 0.5f;
        g[i] = 0.0f;
    }
}

inline void test(float *g) {
    cout << "g={ ";
    for (int i = 0; i < ATTR; i++)
        cout << g[i] << " ";
    cout << "}" << endl;
}

inline void test(unordered_map<int,float> *g) {
    cout << "g={ ";
    for (auto itr = g->begin(); itr != g->end(); ++itr)
        cout << itr->second << " ";
    cout << "}" << endl;
}
