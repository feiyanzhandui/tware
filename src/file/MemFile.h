#ifndef MEM_FILE_H
#define MEM_FILE_H

#include <string>
#include <vector>

#include "file/Block.h"
#include "file/File.h"
#include "serializer/Archive.h"

using std::string;
using std::vector;

namespace tupleware {
class MemFile : public File {
public:
    MemFile(string filename) : File(filename) {
        m_blks = new vector<Block *>();
    }

    ~MemFile() {
        delete m_blks;
    }

    virtual vector<Block *> *blocks() {
        return m_blks;
    }

    virtual void addBlock(Block *blk) {
        m_blks->push_back(blk);
    }

    virtual void serialize(Archive *ar) {
    }

    virtual void deserialize(Archive *ar) {
    }
private:
    vector<Block *> *m_blks;
};
}
#endif
