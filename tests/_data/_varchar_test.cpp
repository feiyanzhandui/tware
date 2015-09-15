#include <iostream>
#include <string>

#include "file/Block.h"
#include "data/Collector.h"
#include "data/HashCollector.h"
#include "data/Schema.h"
#include "data/Store.h"
#include "data/Tuple.h"
#include "data/Varchar.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting varchar test..." << endl << endl;

    int cap0 = 4;
    char a0[cap0 + sizeof(short)];

    short *aa = (short *) &a0;
    aa[0] = 4;
    char *ab = (char *) &a0[sizeof(short)];
    for (int i = 0; i < cap0; i++)
        ab[i] = '1' + i;

    Varchar vc = Varchar(cap0, a0);
    cout << "VC capacity: " << vc.capacity() << endl;;
    cout << "VC size: " << vc.size() << endl;
    cout << "VC data: " << vc.data() << endl;

    for (int i = 0; i < vc.size(); i++)
        cout << "char[" << i << "] = " << vc.charAt(i) << endl;

    cout << "toString(): " << vc.toString() << endl;
/*
    Varchar subVarchar = vc.substring(0, 1);
    cout << "VC capacity: " << subVarchar.capacity() << endl;;
    cout << "VC size: " << subVarchar.size() << endl;
    cout << "VC data: " << subVarchar.data() << endl;

    for (int i = 0; i < subVarchar.size(); i++)
        cout << "char[" << i << "] = " << subVarchar.charAt(i) << endl;

    cout << "toString(): " << subVarchar.toString() << endl;
*/
    //void setData(char *data, size_t size); not implemented
    //void setStringAt(int offset, char *val, int length, int max); not implemented

    cout <<endl << "Varchar test complete." << endl << endl;
    return 0;
}
