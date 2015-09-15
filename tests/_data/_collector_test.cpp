#include <iostream>
#include <string>

#include "file/Block.h"
#include "data/Collector.h"
#include "data/HashCollector.h"
#include "data/Schema.h"
#include "data/Store.h"
#include "data/Tuple.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting collector test..." << endl << endl;

    int numNumbers = 10;
    int mod = 10;

    int size0 = numNumbers * sizeof(int) + sizeof(size_t);
    cout << "size 0: " << size0 << endl;
    char a0[size0];

    size_t *aa = (size_t *) &a0;
    aa[0] = (numNumbers * sizeof(int));
    int *ab = (int *) &a0[8];
    srand((unsigned) time(NULL));

    cout << endl << "Random Numbers:" << endl;
    for (int i = 0; i < (size0 - sizeof(size_t)) / sizeof(int); i++) {
        int randNum = rand() % mod;
        cout << randNum << endl;
        ab[i] = randNum;
    }

    Block b0 = Block(size0, a0);
    cout << endl << "b0 size: " << b0.size() << endl;

    Store<int> *t0 = new Store<int>(b0);
    cout << endl << "t0 size: " << t0->size() << endl;
    for (int i = 0; i < t0->size(); i++)
        cout << "t0[" << i << "] = " << (*(*t0)[i]) << endl;

    cout << endl;

    int size2 = numNumbers * sizeof(int) * 4 + sizeof(size_t) * 5;
    cout << "size 2: " << size2 << endl;
    char a2[size2];

    size_t *bb = (size_t *) &a2;
    bb[0] = (numNumbers * sizeof(int));
    int *ba = (int *) &a2[8];
    for (int i = 1; i < size2 / sizeof(int); i++)
        ba[i] = -1;

    Block b1 = Block(size2, a2);
    cout << "b1 size: " << b1.size() << endl;

    cout<<endl;

    Schema *sch1 = new Schema();
    sch1->addInt();

    HashCollector<int> hash1 = HashCollector<int>(sch1, b1);
    cout << endl << "hash1 capacity: " << hash1.capacity() << endl;
    cout << "hash1 size: " << hash1.size() << endl;

    for (int i = 0; i < t0->size(); i++) {
        int key = (*(*t0)[i]);
        Tuple *x = hash1.get(key);
        if (x == NULL) {
            x = hash1.put(key);
            x->setInt(1, 0);
        }
        int val = x->getInt(1) + 1;
        x->setInt(1, val);
    }

    cout << endl << "hash1.size() = " << hash1.size() << endl;
    for (int i = 0; i < hash1.size(); i++) {
        Tuple *x = hash1[i];
        int key = x->getInt(0);
        int val = x->getInt(1);
        cout << "(" << key << "," << val << ")" << endl;
    }

    for (int i = 0; i < mod; i++)
        cout << "hash1.contains(" << i << "): " << (hash1.contains(i) == 1? "true":"false") << endl;

    cout << endl << endl;

    for (int i = 0; i < t0->size(); i++)
        cout << "t0[" << i << "] = " << (*(*t0)[i]) << endl;

    cout <<endl << "Collector test complete." << endl << endl;
    return 0;
}
