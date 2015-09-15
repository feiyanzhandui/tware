#ifndef REQUEST_H
#define REQUEST_H

#include <cstdint>
#include <string>

#include "serializer/Archive.h"
#include "serializer/Serializable.h"

using std::string;

namespace tupleware {
class Request : public Serializable {
public:
    enum Type {
        ADD,
        ANY,
        BLOCK,
        FILE,
        INFO
    };

    Request() : Serializable() {}

    Request(Request::Type type, string filename, uint64_t blkId)
            : Serializable() {
        m_type = type;
        m_filename = filename;
        m_blkId = blkId;
        m_data = NULL;
    }

    ~Request() {
    }

    Request::Type type() {
        return m_type;
    }

    string filename() {
        return m_filename;
    }

    uint64_t blockId() {
        return m_blkId;
    }

    Serializable *data() {
        return m_data;
    }

    void setData(Serializable *data) {
        m_data = data;
    }

    virtual void serialize(Archive *ar) {
    }

    virtual void deserialize(Archive *ar) {
    }
private:
    Request::Type m_type;
    string m_filename;
    uint64_t m_blkId;
    Serializable *m_data;
};
}
#endif
