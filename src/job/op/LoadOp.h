#ifndef LOAD_OP_H
#define LOAD_OP_H

#include <string>

#include "job/op/Op.h"
#include "serializer/Archive.h"

using std::string;

namespace tupleware {
class LoadOp : public Op {
public:
    LoadOp() {}

    LoadOp(string filename) : Op() {
        m_filename = filename;
    }

    ~LoadOp() {}

    string filename() {
        return m_filename;
    }

    virtual void serialize(Archive *ar) {
        ar->save(m_filename);
    }

    virtual void deserialize(Archive *ar) {
        ar->load(m_filename);
    }
private:
    string m_filename;
};
}
#endif
