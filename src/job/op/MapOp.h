#ifndef MAP_OP_H
#define MAP_OP_H

#include <string>

#include "job/Udf.h"
#include "job/op/Op.h"
#include "serializer/Archive.h"

using std::string;

namespace tupleware {
class MapOp : public Op {
public:
    MapOp() : Op() {
        m_src = NULL;
        m_udf = NULL;
    }

    MapOp(Op *src, Udf *udf) : Op() {
        m_src = src;
        m_udf = udf;
    }

    ~MapOp() {
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
        m_udf->deserialize(ar);
    }
private:
    Op *m_src;
    Udf *m_udf;
};
}
#endif
