#include "util.h"

#define TILE 128
#define DATA 69632//86016//2747253
#define ATTR 90
#define VALUE 10

inline int getKey(int label, int attr, int val) {
    int key = label << 26;
    key |= attr << 10;
    key |= val;
    return key;
}

inline void init(string file, int *data) {
    //data
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < DATA && getline(f, line); i++) {
            istringstream buff(line);
            int dataOff = i * (ATTR + 1);
            for (int j = 0; j <= ATTR; j++)
                buff >> data[dataOff + j] >> delim;
        }
        f.close();
    }
}

inline void init(int *counts) {
    //counts
    for (int i = 0; i < VALUE + VALUE * ATTR * VALUE; i++)
        counts[i] = 0;
}

inline void init(string file, int *data, int *labels) {
    //data
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < DATA && getline(f, line); i++) {
            istringstream buff(line);
            int dataOff = i * ATTR;
            for (int j = 0; j < ATTR; j++)
                buff >> data[dataOff + j] >> delim;
            buff >> labels[i];
        }
        f.close();
    }
}

inline void test(int *counts) {
    cout << "labels: { ";
    for (int i = 0; i < VALUE; i++)
        cout << counts[i] << " ";
    cout << "}\ncounts: { ";
    for (int i = VALUE; i < 20; i++)
        cout << counts[i] << " ";
    cout << "}\n";
}

inline void test(unordered_map<int,int> *counts) {
    cout << "labels: {";
    //for (int i = 0; i < LABEL; i++)
    //    cout << counts[i] << " ";
    cout << "}\ncounts: { \n";
    //for (int i = LABEL; i < 20; i++)
    //    cout << counts[i] << " ";
    cout << "}\n";
}
