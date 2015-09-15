#include "util.h"

#define TILE 1024
#define DATA 936960//29963548
#define ATTR 2
#define CENT 4

inline void init(string file, float *data, float *cent, float *temp) {
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
        }
        f.close();
    }

    //cent + temp
    for (int i = 0; i < CENT; i++) {
        int centOff = i * ATTR;
        int tempOff = i * (ATTR + 1);
        for (int j = 0; j < ATTR; j++) {
            cent[centOff + j] = data[centOff + j];
            temp[tempOff + j] = 0.0f;
        }
        temp[tempOff + ATTR] = 0.0f;
    }
}

inline void init(string file, float *data, float *cent,
        unordered_map<int,float *> *temp) {
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
        }
        f.close();
    }

    //cent + temp
    for (int i = 0; i < CENT; i++) {
        int centOff = i * ATTR;
        float *t = new float[ATTR + 1];
        for (int j = 0; j < ATTR; j++) {
            cent[centOff + j] = data[centOff + j];
            t[j] = 0.0f;
        }
        t[ATTR] = 0.0f;
        (*temp)[i] = t;
    }
}

inline void test(float *temp) {
    cout << "temp=\n";
    for (int i = 0; i < CENT; i++) {
        cout << "\t( ";
        int tempOff = i * (ATTR + 1);
        for (int j = 0; j <= ATTR; j++)
            cout << temp[tempOff + j] << " ";
        cout << ")\n";
    }
    cout << endl;
}

inline void test(unordered_map<int,float *> *temp) {
    cout << "temp:\n";
    for (auto itr = temp->begin(); itr != temp->end(); ++itr) {
        cout << "\t( ";
        float *t = itr->second;
        for (int j = 0; j <= ATTR; j++)
            cout << t[j] << " ";
        cout << ")\n";
    }
    cout << endl;
}
