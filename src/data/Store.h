#ifndef STORE_H
#define STORE_H

//#include "data/ColumnTuple.h"
//#include "data/RowTuple.h"
#include "data/Schema.h"
#include "file/Block.h"

namespace tupleware {
template <typename T>
class Store {
public:
    Store(Block *blk) {
        m_data = (T *) blk->data();
        m_size = blk->size() / sizeof(T);
    }

    Schema *schema();

    T operator[](size_t pos) {
        return m_data[pos];
    }

    size_t size() {
        return m_size;
    }
private:
    size_t m_size;
    T *m_data;
};

template<>
Schema *Store<double>::schema() {
    return new Schema();
}

template<>
Schema *Store<float>::schema() {
    return new Schema();
}

template<>
Schema *Store<int>::schema() {
    return new Schema();
}
/*
template <>
class Store<RowTuple> {
public:
    Store(Schema *sch, Block *blk) {
        m_sch = sch;
        m_size = blk->size() / m_sch->size();
        m_data = blk->data();
        m_out = new RowTuple*[m_cap];
        size_t schSize = m_sch->size();
        for (int i = 0; i < m_cap; i++)
            m_out[i] = new RowTuple(m_sch, m_data + i * schSize);
    }

    ~Store() {
        for (int i = 0; i < m_cap; i++)
            delete m_out[i];
        delete[] m_out;
    }

    Schema *schema() {
        return m_sch;
    }

    size_t size() {
        return m_size;
    }

    RowTuple *operator[](size_t pos) {
        return m_out[pos];
    }
private:
    Schema *m_sch;
    size_t m_size;
    char *m_data;
    RowTuple **m_out;
};

template <>
class Store<ColumnTuple> {
public:
    Store() {
        m_sch = NULL;
        m_size = 0;
        m_data = NULL;
    }

    Store(Schema *sch, Block *blk) {
        m_sch = sch;
        m_size = blk->size() / m_sch->size();
        m_data = blk->data();
    }

    Schema *schema() {
        return m_sch;
    }

    size_t capacity() {
        return m_cap;
    }

    size_t size() {
        return m_size;
    }

    ColumnTuple *operator[](size_t pos) {
        return NULL;
    }
private:
    Schema *m_sch;
    size_t m_size;
    char *m_data;
};*/
}
#endif
