#include <iostream>
#include <string>

#include "file/Block.h"
#include "data/Collector.h"
#include "data/HashCollector.h"
#include "data/Schema.h"
#include "data/Store.h"
#include "data/Tuple.h"
#include "file/File.h"
#include "file/Partition.h"
#include "file/HdfsFile.h"
#include "file/HdfsFileInfo.h"
#include "file/FileInfo.h"
#include "file/TempFile.h"
#include "file/TempFileInfo.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting block test..." << endl << endl;

    int size0 = 2 * sizeof(int) + sizeof(size_t);
    char a0[size0];

    size_t *aa = (size_t *) &a0;
    aa[0] = 8;
    int *ab = (int *) &a0[8];
    for (int i = 1; i < size0 / sizeof(int); i++)
        ab[i] = i;

    Block b0 = Block(size0, a0);

    cout << "b0 capacity: " << b0.capacity() << endl;
    cout << "b0 size: " << b0.size() << endl;
    cout << "b0 data: " << b0.data() << endl;
    cout << "setting b0 size to 4..." << endl;
    b0.setSize(4);
    cout << "b0 size: " << b0.size() << endl;

    cout << endl << "block test complete." << endl << endl;

    cout << "Starting partition test..." << endl << endl;
    Partition *p = new Partition();

    cout << "valid? " << (p->isValid() == 1? "true":"false") << endl;

    Block b6 = Block(4, new char[0]);
    Block b7 = Block(4, new char[0]);

    vector<Block> v6 = vector<Block>();
    v6.push_back(b6);

    p->addBlocks(v6);

    Block reserved1 = p->reserve();

    cout << endl;

    cout << "Reserved Block: " << endl;
    cout << "\tcap: " << reserved1.capacity() << endl;
    cout << "\tsize:" << reserved1.size() << endl;

    cout << endl;

    p->setValid();

    cout << "valid? " << (p->isValid() == 1? "true":"false") << endl;

    cout << endl << "Partition test complete." << endl << endl;

    cout << endl << "Starting file info test..." << endl << endl;

    FileInfo fi = FileInfo();

    cout << endl << "file info test complete." << endl << endl;

    cout << endl << "Starting hdfs file info test..." << endl << endl;

    HdfsFileInfo hdfsFi = HdfsFileInfo();

    cout << endl << "hdfs file info test complete." << endl << endl;

    cout << endl << "Starting hdfs file test..." << endl << endl;

    HdfsFile hdfsFile = HdfsFile();

    vector<Block> hdfsBlocksToAdd = vector<Block>();

    hdfsFile.addBlocks(hdfsBlocksToAdd);

    vector<Block> hdfsBlocks = hdfsFile.blocks();

    Block b1 = hdfsFile.reserve();
    cout << "b1 cap: " << b1.capacity() << endl;

    Block b2 = hdfsFile.reserve();
    cout << "b2 cap: " << b2.capacity() << endl;

    Block b3 = hdfsFile.reserve();
    cout << "b3 cap: " << b3.capacity() << endl;

    hdfsFile.reset();

    cout << endl << "hdfs file test complete." << endl << endl;

    cout << endl << "Starting temp file info test..." << endl << endl;

    TempFileInfo tfi = TempFileInfo();

    cout << endl << "temp file info test complete." << endl << endl;

    cout << endl << "Starting temp file test..." << endl << endl;

    TempFile *tf = new TempFile();

    vector<Block> tfBlocksToAdd = vector<Block>();

    tf->addBlocks(tfBlocksToAdd);

    vector<Block> tempFileBlocks = tf->blocks();

    Block tb1 = tf->reserve();
    cout << "tb1 cap: " << tb1.capacity() << endl;

    Block tb2 = tf->reserve();
    cout << "tb2 cap: " << tb2.capacity() << endl;

    Block tb3 = tf->reserve();
    cout << "tb3 cap: " << tb3.capacity() << endl;

    tf->reset();

    cout << endl << "temp file test complete." << endl << endl;

    return 0;
}
