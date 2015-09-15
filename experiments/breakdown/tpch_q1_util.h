#include "util.h"

#define TILE 1024
#define Q1_LINE_SIZE 256
#define Q1_DELIM '|'
#define Q1_VALSIZE 26
#define Q1_LINEITEM 59986052
#define Q1_DATE1 904694400

inline void init(string file, char *l_returnflag, char *l_linestatus,
        float *l_quantity, float *l_extendedprice, float *l_discount,
        float *l_tax, long *l_shipdate) {
    //lineitem
    ifstream f(file);
    if (f.is_open()) {
        char line[Q1_LINE_SIZE];
        struct tm tm = {0};
        f.getline(line, Q1_LINE_SIZE);
        for (int i = 0; i < Q1_LINEITEM && f.good(); i++) {
            //skip
            char *pos = line;
            pos = strchr(pos, Q1_DELIM);
            pos = strchr(++pos, Q1_DELIM);
            pos = strchr(++pos, Q1_DELIM);
            pos += 3;

            //l_quantity
            l_quantity[i] = strtof(pos, &pos);

            //l_extendedprice
            l_extendedprice[i] = strtof(++pos, &pos);

            //l_discount
            l_discount[i] = strtof(++pos, &pos);

            //l_tax
            l_tax[i] = strtof(++pos, &pos);

            //l_returnflag
            l_returnflag[i] = *(++pos);
            pos = strchr(pos, Q1_DELIM);

            //l_linestatus
            l_linestatus[i] = *(++pos);
            pos = strchr(pos, Q1_DELIM);

            //l_shipdate
            pos += 11;
            *pos = '\0';
            strptime(pos - 10, "%Y-%m-%d", &tm);
            l_shipdate[i] = (long) mktime(&tm);

            //next
            f.getline(line, Q1_LINE_SIZE);
        }
        f.close();
    }
}

inline void init(char *l_returnflag, char *l_linestatus) {
    for (int i = 0; i < Q1_LINEITEM; i++) {
        if (l_returnflag[i] == 'R')
            l_returnflag[i] = 0;
        else if (l_returnflag[i] == 'A')
            l_returnflag[i] = 1;
        else
            l_returnflag[i] = 2;

        if (l_linestatus[i] == 'O')
            l_linestatus[i] = 0;
        else
            l_linestatus[i] = 1;
    }
}

inline void init(char *vals) {
    //vals
    for (int i = 0; i < 14; i++) {
        int valOff = i * Q1_VALSIZE;
        vals[valOff] = (i / 2) * 2;
        vals[valOff + 1] = i % 2;
        *((int *) (vals + valOff + 2)) = 0;
        *((float *) (vals + valOff + 6)) = 0.0f;
        *((float *) (vals + valOff + 10)) = 0.0f;
        *((float *) (vals + valOff + 14)) = 0.0f;
        *((float *) (vals + valOff + 18)) = 0.0f;
        *((float *) (vals + valOff + 22)) = 0.0f;
    }
}

inline void test(unordered_map<short,char *> *vals) {
    cout << "vals:\n";
    for (auto itr = vals->begin(); itr != vals->end(); ++itr) {
        short key = itr->first;
        char *val = itr->second;
        cout << key << "=(" << val[0] << ","
                            << val[1] << ","
                            << *((int *) (val + 2)) << ","
                            << *((float *) (val + 6)) << ","
                            << *((float *) (val + 10)) << ","
                            << *((float *) (val + 14)) << ","
                            << *((float *) (val + 18)) << ","
                            << *((float *) (val + 22)) << ")"
                            << endl;
    }
}

inline void test(char *vals) {
    for (int i = 0; i < 14; i++) {
        int valOff = i * Q1_VALSIZE;
        cout << i << "=(" << vals[valOff] << ","
                          << vals[valOff + 1] << ","
                          << *((int *) (vals + valOff + 2)) << ","
                          << *((float *) (vals + valOff + 6)) << ","
                          << *((float *) (vals + valOff + 10)) << ","
                          << *((float *) (vals + valOff + 14)) << ","
                          << *((float *) (vals + valOff + 18)) << ","
                          << *((float *) (vals + valOff + 22)) << ")"
                          << endl;
    }
}
