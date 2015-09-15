#include "util.h"

#define TILE 8
#define LINK 4068352//130160392
#define PAGE 5716810

inline void init(string file1, string file2, int *links, int *counts,
        float *ranks, float *temp) {
    //links
    ifstream f1(file1);
    if (f1.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < LINK && getline(f1, line); i++) {
            int linkOff = i * 2;
            istringstream buff(line);
            buff >> links[linkOff] >> delim >> links[linkOff + 1];
        }
        f1.close();
    }

    //counts
    ifstream f2(file2);
    if (f2.is_open()) {
        for (int i = 0; i < PAGE; i++)
            counts[i] = 0.0f;

        string line;
        char delim;
        float temp;
        int id;
        for (int i = 0; i < PAGE && getline(f2, line); i++) {
            istringstream buff(line);
            buff >> id >> delim;
            buff >> counts[id];
        }
        f2.close();
    }

    //ranks + temp
    for (int i = 0; i < PAGE; i++) {
        ranks[i] = 1.0f;
        temp[i] = 0.0f;
    }
}

inline void init(string file1, string file2, int *links, int *counts,
        float *ranks) {
    //links
    ifstream f1(file1);
    if (f1.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < LINK && getline(f1, line); i++) {
            int linkOff = i * 2;
            istringstream buff(line);
            buff >> links[linkOff] >> delim >> links[linkOff + 1];
        }
        f1.close();
    }

    //counts
    ifstream f2(file2);
    if (f2.is_open()) {
        for (int i = 0; i < PAGE; i++)
            counts[i] = 0.0f;

        string line;
        char delim;
        float temp;
        int id;
        for (int i = 0; i < PAGE && getline(f2, line); i++) {
            istringstream buff(line);
            buff >> id >> delim;
            buff >> counts[id];
        }
        f2.close();
    }

    //ranks
    for (int i = 0; i < PAGE; i++)
        ranks[i] = 1.0f;
}

inline void init(string file1, string file2, int *srcs, int *dsts, int *counts,
        float *ranks, float *temp) {
    //links
    ifstream f1(file1);
    if (f1.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < LINK && getline(f1, line); i++) {
            istringstream buff(line);
            buff >> srcs[i] >> delim >> dsts[i];
        }
        f1.close();
    }

    //counts
    ifstream f2(file2);
    if (f2.is_open()) {
        for (int i = 0; i < PAGE; i++)
            counts[i] = 0.0f;

        string line;
        char delim;
        float temp;
        int id;
        for (int i = 0; i < PAGE && getline(f2, line); i++) {
            istringstream buff(line);
            buff >> id >> delim;
            buff >> counts[id];
        }
        f2.close();
    }

    //ranks + temp
    for (int i = 0; i < PAGE; i++) {
        ranks[i] = 1.0f;
        temp[i] = 0.0f;
    }
}

inline void test(float *temp) {
    cout << "temp:\n";
    for (int i = 0; i < 20; i++)
        cout << "\ttemp[" << i << "]=" << temp[i] << "\n";
    cout << endl;
}

inline void test(unordered_map<int,float> *temp) {
    cout << "temp:\n";
    for (int i = 0; i < 20; i++)
        cout << "\ttemp[" << i << "]=" << (*temp)[i] << "\n";
    cout << endl;
}
