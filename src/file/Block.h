#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>

#include "data/Any.h"
#include "serializer/Archive.h"
#include "serializer/Serializable.h"

namespace tupleware {
class Block : public Serializable {
public:
    template <typename T>
    Block(uint64_t id, T &data) : Serializable() {
        m_id = id;
        m_data = data;
    }

    uint64_t id() {
        return m_id;
    }

    Any &data() {
        return m_data;
    }

    virtual void serialize(Archive *ar) {
        ar->save(m_id);
        //ar->serialize(m_data);
    }

    virtual void deserialize(Archive *ar) {
        ar->load(m_id);
        //ar->load(m_data);
    }
private:
    uint64_t m_id;
    Any m_data;
};
}
#endif
