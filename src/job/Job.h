#ifndef JOB_H
#define JOB_H

#include "frontend/Context.h"
#include "job/op/Op.h"
#include "serializer/Archive.h"
#include "serializer/Serializable.h"

namespace tupleware {
class Job : public Serializable {
public:
    Job(Op *wflow, Context *ctx) {
        m_wflow = wflow;
        m_ctx = ctx;
    }

    ~Job() {
        delete m_wflow;
    }

    Op *workflow() {
        return m_wflow;
    }

    Context *context() {
        return m_ctx;
    }

    virtual void serialize(Archive *ar) {
        m_wflow->serialize(ar);
        m_ctx->serialize(ar);
    }

    virtual void deserialize(Archive *ar) {
        m_wflow = (Op *) Serializable::deserializePolymorphic(ar);
        m_ctx->deserialize(ar);
    }
private:
    Op *m_wflow;
    Context *m_ctx;
};
}
#endif
