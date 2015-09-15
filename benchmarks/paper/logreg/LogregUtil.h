#ifndef LOGREG_UTIL_H
#define LOGREG_UTIL_H

#define LOGREG_ITER 20
#define LOGREG_DATA 1525
//#define LOGREG_DATA 763
//#define LOGREG_DATA 7622*2
//#define LOGREG_DATA 76220
//#define LOGREG_DATA 86016//2747253
#define LOGREG_ATTR 1024
//#define LOGREG_ATTR 90
#define LOGREG_SEND LOGREG_ATTR * sizeof(float)

inline void init_logreg_data(string file, float *d) {
    //d
    ifstream f(file);
    if (f.is_open()) {
        string line;
        char delim;
        for (int i = 0; i < LOGREG_DATA && getline(f, line); i++) {
            istringstream buff(line);
            int dOff = i * (LOGREG_ATTR + 1);
            for (int j = 0; j <= LOGREG_ATTR; j++)
                buff >> d[dOff + j] >> delim;
        }
        f.close();
    }
}

#endif
