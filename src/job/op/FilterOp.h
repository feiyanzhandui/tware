#ifndef FILTER_OP_H
#define FILTER_OP_H

#include "job/Udf.h"
#include "job/op/Op.h"
#include "serializer/Archive.h"

namespace tupleware {
class FilterOp : public Op {
public:
    FilterOp() {
        m_src = NULL;
        m_udf = NULL;
    }

    FilterOp(Op *src, Udf *udf) : Op() {
        m_src = src;
        m_udf = udf;
    }

    ~FilterOp() {
        delete m_src;
        delete m_udf;
    }

    Op *source() {
        return m_src;
    }

    Udf *udf() {
        return m_udf;
    }

    virtual void serialize(Archive *ar) {
        m_src->serialize(ar);
        m_udf->serialize(ar);
    }

    virtual void deserialize(Archive *ar) {
        m_src = (Op *) Serializable::deserializePolymorphic(ar);
        m_udf = new Udf();
        m_udf->deserialize(ar);
    }
private:
    Op *m_src;
    Udf *m_udf;
};
}
#endif
