#include <cstdlib>
#include <iostream>
#include <string>

#include "file/Block.h"
//#include "data/Schema.h"
//#include "data/Tuple.h"
//#include "io/Reader.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting reader test..." << endl << endl;
/*
    Schema s1 = Schema();
    s1.addInt();

    //FileReader(Schema *sch, TaskController *taskCtlr);
    FileReader fr1 = FileReader(s1, NULL);
    fr1.hasNext();
    Store<T> *fr1Next1 = fr1.next();


    //HdfsReader(TaskController *taskCtlr, Parser<T> *pars);
    HdfsReader hdfsR1 = HdfsReader(NULL, NULL);
    hdfsR1.hasNext();
    Store<T> *hdfsR1 = hdfsR1.next();



    srand((unsigned) time(NULL));
    int numTuples = 3;
    int numChars = 18;
    char *raw = new char[sizeof(size_t) + numChars];
    Block b1 = Block(numChars, raw);
    b1.setSize(numChars);
    char *data = b1.data();
    Schema *sch1 = new Schema();
    sch1->addInt();
    sch1->addInt();
    sch1->addInt();

    for (int i = 0; i < numTuples; i++) {
        for (int j = 0; j < numAttrs; j += 2) {
            data[j] = (int) rand() % 10;
            data1[j + 1] = ',';
        }
        data[i + numAttrs] = '\n';
    }

    DsvReader<Tuple> *r1 = new DsvReader<Tuple>(&sch1, b1, ',');

    while (r1->hasNext()) {
        Tuple *rt = r1->next();
        if (rt == 0)
            cout << "rt is null..." << endl;
        cout << "next: (" << rt->getInt(0) << "," << rt->getInt(1) << "," << rt->getInt(2) << ")" << endl;
    }

    cout << "r1 does not have next" << endl;
//

    char *data2 = new char[1000]{
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,

        '1',
        ',',
        '2',
        '3',
        '\n',

        '1',
        ',',
        '2',
        '3',
        '\n',

        '1',
        ',',
        '2',
        '3',
        '\n',

        '1',
        ',',
        '2',
        '3',
        '\n',

        '1',
        ',',
        '2',
        '3',
        '\n',

        '1',
        ',',
        '2',
        '3',
        '\n'
    };

    int tupleElems2 = 0;
    int numTuples2 = 0;

    int numElems2 = 0;
    Block b2 = Block(numElems2, data1);
    b1.setSize(numElems2);

    Schema sch2 = Schema();

    for (int i = 0; i < numElems2; i++)
        sch2.addInt();

    DsvReader r2 = DsvReader(&sch2, b2, ',');

    while (r2.hasNext()) {
        Tuple *rt = r2.next();
        cout << "(";
        for (int i = 0; i < tupleElems2; i++)
            cout << rt->getInt(i) << ",";
        cout << ")" << endl;
    }

    cout << "r1 does not have next" << endl;
*/



    cout <<endl << "Reader test complete." << endl << endl;
    return 0;
}
