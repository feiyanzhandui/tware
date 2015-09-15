#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "data/Schema.h"
#include "data/Tuple.h"

namespace tupleware {
template <typename T>
class Collector {
public:
    size_t capacity() {
        return m_cap;
    }

    size_t size() {
        return m_size;
    }

    virtual Tuple *get(T key) = 0;
    virtual Tuple *put(T key) = 0;
    virtual bool contains(T key) = 0;
    virtual Tuple *operator[](size_t pos) = 0;
protected:
    Schema *m_sch;
    size_t m_cap;
    size_t m_size;

    Collector(Schema *sch, Block blk) {
        m_sch = sch;
        m_cap = 0;
        m_size = 0;
    }
};
}
#endif
