#include "util.h"

#define TILE 256
#define Q6_LINE_SIZE 256
#define Q6_DELIM '|'
#define Q6_LINEITEM 59986052
#define Q6_DATE1 757382400
#define Q6_DATE2 788918400

inline void init(string file, float *l_quantity, float *l_extendedprice,
        float *l_discount, long *l_shipdate) {
    //lineitem
    ifstream f(file);
    if (f.is_open()) {
        char line[Q6_LINE_SIZE];
        struct tm tm = {0};
        f.getline(line, Q6_LINE_SIZE);
        for (int i = 0; i < Q6_LINEITEM && f.good(); i++) {
            //skip
            char *pos = line;
            pos = strchr(pos, Q6_DELIM);
            pos = strchr(++pos, Q6_DELIM);
            pos = strchr(++pos, Q6_DELIM);
            pos += 3;

            //l_quantity
            l_quantity[i] = strtof(pos, &pos);

            //l_extendedprice
            l_extendedprice[i] = strtof(++pos, &pos);

            //l_discount
            l_discount[i] = strtof(++pos, &pos);

            //skip
            pos = strchr(++pos, Q6_DELIM);

            //l_shipdate
            pos += 15;
            *pos = '\0';
            strptime(pos - 10, "%Y-%m-%d", &tm);
            l_shipdate[i] = (long) mktime(&tm);

            //next
            f.getline(line, Q6_LINE_SIZE);
        }
        f.close();
    }
}

inline void test(float revenue) {
    cout << "revenue:" << revenue << endl;
}
