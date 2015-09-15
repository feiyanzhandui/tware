#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>

#include "file/Block.h"
#include "serializer/Serializable.h"

using std::string;
using std::vector;

namespace tupleware {
class File : public Serializable {
public:
    virtual ~File() {}

    string filename() {
        return m_filename;
    }

    virtual vector<Block *> *blocks() = 0;
    virtual void addBlock(Block *blk) = 0;
protected:
    string m_filename;

    File(string filename) : Serializable() {
        m_filename = filename;
    }
};
}
#endif
