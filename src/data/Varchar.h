#ifndef VARCHAR_H
#define VARCHAR_H

#include <cstring>
#include <string>

using std::memcpy;
using std::string;

namespace tupleware {
class Varchar {
public:
    Varchar(short cap, char *data) {
        m_cap = cap;
        m_size = (short *) data;
        m_data = data + sizeof(short);
    }

    short capacity() {
        return m_cap;
    }

    short size() {
        return *m_size;
    }

    char *data() {
        return m_data;
    }

    Varchar substring(short start, short end) {
        return Varchar(end - start, m_data + start);
    }

    char charAt(short pos) {
        return m_data[pos];
    }

    //void setData(char *data, size_t size) {
    //    memcpy(m_data, data, size);
    //}

    //void setStringAt(int offset, char *val, int length, int max) {
    //    char *start = m_data + offset;
    //    int end = length < max ? length : max;
    //    strncpy(start, val, end);
    //    *(start + end) = '\0';
    //}

    string toString() {
        return "";
    }
private:
    short m_cap;
    short *m_size;
    char *m_data;
};
}
#endif
