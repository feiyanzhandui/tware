#ifndef HASH_COLLECTOR_H
#define HASH_COLLECTOR_H

#include "data/Collector.h"
#include "data/RowTuple.h"
#include "data/Schema.h"
#include "data/Store.h"
#include "data/Tuple.h"
#include "file/Block.h"

#include <iostream>
using std::cout;
using std::endl;

namespace tupleware {
template <typename T>
class HashCollector : public Collector<T> {
public:
    HashCollector(Schema *sch, Block blk)
            : Collector<T>(sch, blk) {
        size_t size = this->m_sch->size() + 2 * sizeof(int);
        this->m_cap = (blk.capacity() - sizeof(size_t)) / size;
        this->m_size = (blk.size() - sizeof(size_t)) / size;
        size_t dataSize = this->m_cap * this->m_sch->size() + sizeof(size_t);
        size_t chainSize = this->m_cap * sizeof(int);
        size_t listSize = this->m_cap * sizeof(int);
        m_data = new Store<RowTuple>(sch, Block(dataSize, blk.data()));
        m_chain = (int *) (blk.data() + dataSize);
        m_list = (int *) (blk.data() + dataSize + chainSize);

        m_out = new RowTuple*[this->m_cap];
        size_t schSize = this->m_sch->size();
        for (int i = 0; i < this->m_cap; i++)
            m_out[i] = new RowTuple(this->m_sch, m_data + i * schSize);
    }

    virtual Tuple *get(T key) {
        int head = hash(key);
        int pos = head;
        if (m_chain[pos] >= 0)
//            while (pos != head && !equals((*m_data)[pos], key))
//                pos = m_chain[pos];
            do {
                //if (equals((*m_data)[pos], key))
                //    return (*m_data)[pos];
                pos = m_chain[pos];
            } while (pos != head);
        return NULL;
    }

    virtual Tuple *put(T key) {
        int head = hash(key);
        int pos = head;
        if (m_chain[pos] >= 0) {
            while (m_chain[pos] != head)
                pos = m_chain[pos];
            int tail = pos;
            do {
                pos++;
            } while (m_chain[pos] >= 0 && pos < this->m_cap);
            if (pos == this->m_cap) {
                pos = 0;
                while (m_chain[pos] >= 0 && pos < tail)
                    pos++;
            }
            m_chain[tail] = pos;
        }
        m_chain[pos] = head;
        m_list[this->m_size] = pos;
        this->m_size++;
        Tuple *t;// = (*m_data)[pos];
        setKey(t, key);
        return t;
    }

    virtual bool contains(T key) {
        int head = hash(key);
        int pos = head;
        if (m_chain[pos] >= 0)
            do {
                //if (equals((*m_data)[pos], key))
                //    return true;
                pos = m_chain[pos];
            } while (pos != head);
        return false;
    }

    virtual Tuple *operator[](size_t pos) {
        return NULL;
        //return (*m_data)[m_list[pos]];
    }

void printChain() {
cout << "\tchain={ ";
for (int i = 0; i < this->m_cap; i++)
    cout << m_chain[i] << " ";
cout << "}" << endl;
}
private:
    char *m_data;
    RowTuple **m_out;
    int *m_chain;
    int *m_list;

    int hash(T key);
    bool equals(Tuple *t, T key);
    void setKey(Tuple *t, T key);
};

template<>
int HashCollector<int>::hash(int key) {
    return key % m_cap;
}

template<>
bool HashCollector<int>::equals(Tuple *t, int key) {
    return t->getInt(0) == key;
}

template<>
void HashCollector<int>::setKey(Tuple *t, int key) {
    t->setInt(0, key);
}

template<>
int HashCollector<Tuple *>::hash(Tuple *key) {
    return 0;
}

template<>
bool HashCollector<Tuple *>::equals(Tuple *t, Tuple *key) {
    return true;
}

template<>
void HashCollector<Tuple *>::setKey(Tuple *t, Tuple *key) {
}
}
#endif
