#ifndef REDUCE_OP_H
#define REDUCE_OP_H

#include <string>

#include "job/Udf.h"
#include "job/op/Op.h"
#include "serializer/Archive.h"
#include "serializer/Serializer.h"

using std::string;

namespace tupleware {
class ReduceOp : public Op {
public:
    ReduceOp() : Op() {
        m_src = NULL;
        m_udf = NULL;
        m_key = NULL;
    }

    ReduceOp(Op *src, Udf *udf, Udf *key) : Op() {
        m_src = src;
        m_udf = udf;
        m_key = key;
    }

    ~ReduceOp() {
        delete m_src;
        delete m_udf;
        if (m_key != NULL)
            delete m_key;
    }

    Op *source() {
        return m_src;
    }

    Udf *udf() {
        return m_udf;
    }

    Udf *key() {
        return m_key;
    }

    virtual void serialize(Archive *ar) {
        m_src->serialize(ar);
        m_udf->serialize(ar);
        m_key->serialize(ar);
    }

    virtual void deserialize(Archive *ar) {
        m_src = (Op *) Serializable::deserializePolymorphic(ar);
        m_udf = new Udf();
        m_udf->deserialize(ar);
        m_key = new Udf();
        m_key->deserialize(ar);
    }
private:
    Op *m_src;
    Udf *m_udf;
    Udf *m_key;
};
}
#endif
