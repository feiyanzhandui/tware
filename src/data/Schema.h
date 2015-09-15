#ifndef SCHEMA_H
#define SCHEMA_H

#include <sstream>
#include <string>
#include <vector>

using std::ostringstream;
using std::string;
using std::vector;

namespace tupleware {
class Schema {
public:
enum AttributeType {
    DOUBLE,
    FLOAT,
    INT,
    STRING
};
    Schema() {
        m_numAttrs = 0;
        m_size = 0;
    }

    Schema(Schema *sch1, Schema *sch2) {
        m_numAttrs = 0;
        m_size = 0;
        for (short i = 0; i < sch1->m_numAttrs; i++)
            addAttribute(sch1->m_types[i], sch1->m_caps[i]);
        for (short i = 0; i < sch2->m_numAttrs; i++)
            addAttribute(sch2->m_types[i], sch2->m_caps[i]);
    }

    short numberOfAttributes() {
        return m_numAttrs;
    }

    int size() {
        return m_size;
    }

    AttributeType getType(short attrNum)  {
        return m_types[attrNum];
    }

    short getCapacity(short attrNum) {
        return m_caps[attrNum];
    }

    void addDouble() {
        addAttribute(DOUBLE, sizeof(double));
    }

    void addFloat() {
        addAttribute(FLOAT, sizeof(float));
    }

    void addInt() {
        addAttribute(INT, sizeof(int));
    }

    void addString(short cap) {
        addAttribute(STRING, cap);
        m_size += sizeof(short);
    }

    string toString() {
        ostringstream toStr;
        toStr << "SCHEMA={ ";
        for (int i = 0; i < m_types.size(); i++)
            toStr << m_types[i] << "," << m_caps[i] << ";";
        string str = toStr.str();
        return str.erase(str.size() - 1) + " }";
    }
private:
    short m_numAttrs;
    int m_size;
    vector<AttributeType> m_types;
    vector<short> m_caps;

    void addAttribute(AttributeType type, short cap) {
        m_types.push_back(type);
        m_caps.push_back(cap);
        m_size += cap;
        ++m_numAttrs;
    }
};
}
#endif
