#ifndef HASH_COLLECTOR_H
#define HASH_COLLECTOR_H

#include <unordered_map>

//#include "data/Collector.h"
#include "file/Block.h"

using std::unordered_map;

namespace tupleware {
template <typename K,typename V>
class HashCollector {//: public Collector<K,V> {
public:
    HashCollector() {//: Collector<K,V>() {
        m_data = new unordered_map<K,V>();
    }

    virtual V get(K key) {
        return m_data[key];
    }

    virtual void put(K key, V val) {
        m_data[key] = val;
    }

    virtual bool contains(K key) {
        return m_data.find(key) != m_data.end();
    }

    virtual Block *toBlocks() {
        return NULL;
    }
private:
    unordered_map<K,V> *m_data;
};
}
#endif
