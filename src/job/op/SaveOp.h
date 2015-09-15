#ifndef SAVE_OP_H
#define SAVE_OP_H

#include "job/op/Op.h"
#include "serializer/Archive.h"

namespace tupleware {
class SaveOp : public Op {
public:
    SaveOp(Op *src) : Op() {
        m_src = src;
    }

    ~SaveOp() {
        delete m_src;
    }

    Op *source() {
        return m_src;
    }

    virtual void serialize(Archive *ar) {
    }

    virtual void deserialize(Archive *ar) {
    }
private:
    Op *m_src;
};
}
#endif
