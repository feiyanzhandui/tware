#include <iostream>
#include <string>

#include "file/Block.h"
#include "data/Collector.h"
#include "data/HashCollector.h"
#include "data/Schema.h"
#include "data/Store.h"
#include "data/Tuple.h"

#include <limits>

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting store test..." << endl << endl;

    int size0 = 4 * sizeof(int) + sizeof(size_t);
    char a0[size0];

    size_t *aa = (size_t *) &a0;
    aa[0] = 16;
    int *ab = (int *) &a0[8];
    for (int i = 0; i < (size0 - sizeof(size_t)) / sizeof(int); i++)
        ab[i] = i;

    Block b0 = Block(size0, a0);

    Store<int> *t0 = new Store<int>(b0);

    cout << "t0 size: " << t0->size() << endl;
    for (int i = 0; i < t0->size(); i++)
        cout << "t0[" << i << "] = " << (*((*t0)[i])) << endl;

    cout << endl;

    int size2 = 4 * sizeof(int) + sizeof(size_t);
    char a2[size2];

    size_t *cc = (size_t *) &a2;
    cc[0] = 16;

    int *ca = (int *) &a2[8];
    ca[0] = (*((*t0)[0])) + 1;
    ca[1] = (*((*t0)[1])) + 1;
    ca[2] = (*((*t0)[2])) + 1;
    ca[3] = (*((*t0)[3])) + 1;

    Block b1 = Block(size2, a2);

    Store<int> *testGetFromT0 = new Store<int>(b1);

    cout << "testGetFromT0 size: " << testGetFromT0->size() << endl;
    for (int i = 0; i < testGetFromT0->size(); i++)
        cout << "testGetFromT0[" << i << "] = " << (*((*testGetFromT0)[i])) << endl;

    cout << endl;

    cout << "t0 unchanged: " << t0->size() << endl;
    for (int i = 0; i < t0->size(); i++)
        cout << "t0[" << i << "] = " << (*((*t0)[i])) << endl;

    cout << endl;

    int size3 = 100 * sizeof(int) + sizeof(size_t);
    char a3[size3];

    size_t *dd = (size_t *) &a3;
    dd[0] = 400;
    int *ad = (int *) &a3[sizeof(size_t)];
    for (int i = 0; i < (size3 - sizeof(size_t)) / sizeof(int); i++)
        ad[i] = i;

    Block b2 = Block(size3, a3);
    Store<int> *storeTest = new Store<int>(b2);

    cout << endl;

    int size4 = 10 * sizeof(double) + sizeof(size_t);
    char a4[size4];
    size_t *ee = (size_t *) &a4;
    ee[0] = 80;
    double *ea = (double *) &a4[sizeof(size_t)];
    for (int i = 0; i < (size4 - sizeof(size_t)) / sizeof(double); i++)
        ea[i] = i + 0.1;

    Block b3 = Block(size4, a4);
    Store<double> *doubleTest = new Store<double>(b3);

    cout << "double store: " << doubleTest->size() << endl;
    for (int i = 0; i < doubleTest->size(); i++)
        cout << "doubleTest[" << i << "] = " << (*((*doubleTest)[i])) << endl;

    cout << endl << endl;

    int size5 = 5 * sizeof(float) + sizeof(size_t);
    char a5[size5];
    size_t *ff = (size_t *) &a5;
    ff[0] = 20;
    float *fa = (float *) &a5[sizeof(size_t)];
    for (int i = 0; i < (size5 - sizeof(size_t)) / sizeof(float); i++)
        fa[i] = i + 0.12345;

    Block b4 = Block(size5, a5);
    Store<float> *floatTest = new Store<float>(b4);

    cout << "Float Store Test: " << floatTest->size() << endl;
    for (int i = 0; i < floatTest->size(); i++)
        cout << "floatTest[" << i << "] = " << (*((*floatTest)[i])) << endl;

    cout << endl << endl;

    int size6 = 26 * sizeof(char) + sizeof(size_t);
    char a6[size6];
    size_t *gg = (size_t *) &a6;
    gg[0] = 26;

    char *ga = (char *) &a6[sizeof(size_t)];
    for (int i = 0; i < (size6 - sizeof(size_t)) / sizeof(char); i++)
        ga[i] = 'a' + i;

    Block b5 = Block(size6, a6);
    Store<char> *charTest = new Store<char>(b5);

    cout << "Char Store Test: " << charTest->size() << endl;
    for (int i = 0; i < charTest->size(); i++)
        cout << "charTest[" << i << "] = " << (*((*charTest)[i])) << endl;

    cout << endl;

    int size6r = 26 * sizeof(char) + sizeof(size_t);
    char a6r[size6r];
    size_t *ggr = (size_t *) &a6r;
    ggr[0] = 26;

    char *gar = (char *) &a6r[sizeof(size_t)];

    int j = 25;
    for (int i = 0;  i < (size6r - sizeof(size_t)) / sizeof(char); i++) {
        gar[i] = (*((*charTest)[j]));
        j--;
    }

    Block b5r = Block(size6r, a6r);
    Store<char> *charTestr = new Store<char>(b5r);

    cout << "Char Store Test: " << charTestr->size() << endl;
    for (int i = 0; i < charTestr->size(); i++)
        cout << "charTestReverse[" << i << "] = " << (*((*charTestr)[i])) << endl;

    cout << endl << endl;

    int size7 = 4 * sizeof(short) + sizeof(size_t);
    char a7[size7];
    size_t *hh = (size_t *) &a7;
    hh[0] = 8;

    short *ha = (short *) &a7[sizeof(size_t)];
    for (int i = 0; i < (size7 - sizeof(size_t)) / sizeof(short); i++)
        ha[i] = i - 1;

    Block b6 = Block(size7, a7);
    Store<short> *shortTest = new Store<short>(b6);

    cout << "Short Store Test: " << shortTest->size() << endl;
    for (int i = 0; i < shortTest->size(); i++)
        cout << "shortTest[" << i << "] = " << (*((*shortTest)[i])) << endl;

    cout << endl << "Store<RowTuple> Test:" << endl;

    Schema *rtsSchema1 = new Schema();
    rtsSchema1->addInt();

    int rtsSize1 = sizeof(size_t) + sizeof(int) * 1;
    char rtsData[rtsSize1];
    size_t *rts1 = (size_t *) &rtsData;
    rts1[0] = 4;

    Block rtsBlock1 = Block(rtsSize1, rtsData);
    cout << "rtsBlock1.size() " << rtsBlock1.size() << endl;

    Store<RowTuple> *rtStore1 = new Store<RowTuple>(rtsSchema1, rtsBlock1);

    delete t0;
    delete testGetFromT0;
    delete storeTest;
    delete doubleTest;
    delete floatTest;
    delete charTest;
    delete charTestr;
    delete shortTest;

    cout << endl << "Store test complete." << endl << endl;
    return 0;
}
