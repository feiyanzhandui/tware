#include <cstdlib>
#include <iostream>
#include <string>

#include "data/Schema.h"
#include "data/Tuple.h"
#include "file/Block.h"
#include "io/DsvParser.h"
#include "io/Parser.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting parser test..." << endl << endl;

    Schema s1 = Schema();
    s1.addInt();

    int size0 = 2 * sizeof(int) + sizeof(size_t);
    char a0[size0];

    size_t *aa = (size_t *) &a0;
    aa[0] = 8;
    int *ab = (int *) &a0[8];
    for (int i = 1; i < size0 / sizeof(int); i++)
        ab[i] = i;

    Block b0 = Block(size0, a0);

    //DsvParser(Schema *sch, Block blk);
    DsvParser<int> dsvP1 = DsvParser<int>(&s1, b0);
/*
    dsvP1.hasNext();
    T *next1 = dsvP1.next();

    //TextParser(int maxStrSize, void *buff);
    TextParser tP1 = TextParser(-1, NULL);
    tP1.hasNext();
    string *tP1Next1 = tP1.next();
*/

    cout <<endl << "Parser test complete." << endl << endl;
    return 0;
}
