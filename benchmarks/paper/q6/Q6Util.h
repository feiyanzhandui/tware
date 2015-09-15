#ifndef Q6_UTIL_H
#define Q6_UTIL_H

#include <fstream>
#include <string>

#include "../Util.h"

using std::ifstream;
using std::string;

//#define Q6_LINEITEM 2998605
#define Q6_LINEITEM 59986052
#define Q6_EXTRA (Q6_LINEITEM % TILE)
#define Q6_DATE1 757382400
#define Q6_DATE2 788918400
#define Q6_MINDATE 662774400
#define Q6_SDATE1 1095
#define Q6_SDATE2 1460
#define Q6_VALSIZE sizeof(float)

inline void load(string file, float *l_quantity, float *l_extendedprice,
        float *l_discount, long *l_shipdate) {
    //lineitem
    ifstream f(file);
    if (f.is_open()) {
        char line[LINE_SIZE];
        struct tm tm = {0};
        f.getline(line, LINE_SIZE);
        for (int i = 0; i < Q6_LINEITEM && f.good(); i++) {
            //skip
            char *pos = line;
            pos = strchr(pos, TPCH_DELIM);
            pos = strchr(++pos, TPCH_DELIM);
            pos = strchr(++pos, TPCH_DELIM);
            pos += 3;

            //l_quantity
            l_quantity[i] = strtof(++pos, &pos);

            //l_extendedprice
            l_extendedprice[i] = strtof(++pos, &pos);

            //l_discount
            l_discount[i] = strtof(++pos, &pos);

            //skip
            pos = strchr(++pos, TPCH_DELIM);
            pos = strchr(++pos, TPCH_DELIM);

            //l_shipdate
            pos += 15;
            *pos = '\0';
            strptime(pos - 10, "%Y-%m-%d", &tm);
            l_shipdate[i] = (long) mktime(&tm);

            //next
            f.getline(line, LINE_SIZE);
        }
        f.close();
    }
}
#endif
