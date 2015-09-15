#ifndef DSV_PARSER_H
#define DSV_PARSER_H

#include <cstdlib>

#include "data/AttributeType.h"
#include "data/RowTuple.h"
#include "data/Schema.h"
#include "data/Tuple.h"
#include "data/Varchar.h"
#include "file/Block.h"
#include "io/Parser.h"

using std::strtod;
using std::strtof;
using std::strtol;

namespace tupleware {
template <typename T>
class DsvParser : public Parser<T> {
public:
    DsvParser(Schema *sch, Block blk) {
        this->m_sch = sch;
        m_pos = blk.data();
        m_end = m_pos + blk.size();
        m_curr = new T();
        m_next = new T();
        next();
    }

    ~DsvParser() {
        delete m_curr;
        delete m_next;
    }

    virtual bool hasNext() {
        return m_hasNext;
    }

    virtual T *next() {
        T *temp = m_curr;
        m_curr = m_next;
        m_next = temp;
        m_hasNext = false;
        while (!m_hasNext && m_pos < m_end)
            if (*m_pos == '\n')
                ++m_pos;
            else {
                char *val;
                setNext(val);
                m_hasNext = (*val == '\n');
                m_pos = val + 1;
            }
        return m_curr;
    }
private:
    char *m_pos;
    char *m_end;
    bool m_hasNext;
    T *m_curr;
    T *m_next;

    void setNext(char *val);
};

template <>
void DsvParser<double>::setNext(char *val) {
    *m_next = strtod(m_pos, &val);
}

template <>
void DsvParser<float>::setNext(char *val) {
    *m_next = strtof(m_pos, &val);
}

template <>
void DsvParser<int>::setNext(char *val) {
    *m_next = strtol(m_pos, &val, 0);
}

template<>
class DsvParser<Varchar> : public Parser<Varchar> {
public:
    DsvParser(Schema *sch, Block blk) {
        m_sch = sch;
        m_pos = blk.data();
        m_end = m_pos + blk.size();
        size_t schSize = m_sch->size();
        m_buff1 = new char[schSize];
        m_buff2 = new char[schSize];
        m_curr = new Varchar(0, m_buff1);
        m_next = new Varchar(0, m_buff2);
        next();
    }

    ~DsvParser() {
        delete[] m_buff1;
        delete[] m_buff2;
        delete m_curr;
        delete m_next;
    }

    virtual bool hasNext() {
        return m_hasNext;
    }

    virtual Varchar *next() {
        //RowTuple *temp = m_curr;
        //m_curr = m_next;
        //m_next = temp;
        //m_hasNext = false;
        //while (!m_hasNext && m_pos < m_end)
        //    if (*m_pos == '\n')
        //        ++m_pos;
        //    else {
        //        int numAttrs = m_sch->numberOfAttributes();
        //        char *val;
        //        int attrNum = 0;
        //        while (attrNum < numAttrs) {
        //            m_pos = val + 1;
        //            attrNum++;
        //        }
        //        m_hasNext = attrNum == numAttrs;
        //    }
        //return m_curr;
        return NULL;
    }
private:
    char *m_pos;
    char *m_end;
    char m_delim;
    bool m_hasNext;
    char *m_buff1;
    char *m_buff2;
    Varchar *m_curr;
    Varchar *m_next;
};

template<>
class DsvParser<Tuple> : public Parser<Tuple> {
public:
    DsvParser(Schema *sch, Block blk, char delim) {
        m_sch = sch;
        m_pos = blk.data();
        m_end = m_pos + blk.size();
        m_delim = delim;
        size_t schSize = m_sch->size();
        m_buff1 = new char[schSize];
        m_buff2 = new char[schSize];
        m_curr = new RowTuple(m_sch, m_buff1);
        m_next = new RowTuple(m_sch, m_buff2);
        next();
    }

    ~DsvParser() {
        delete[] m_buff1;
        delete[] m_buff2;
        delete m_curr;
        delete m_next;
    }

    virtual bool hasNext() {
        return m_hasNext;
    }

    virtual Tuple *next() {
        RowTuple *temp = m_curr;
        m_curr = m_next;
        m_next = temp;
        m_hasNext = false;
        while (!m_hasNext && m_pos < m_end)
            //for chars until newline
            //if just 1 pos, advance
            //else, parse it
            //else, we have a problem!
            if (*m_pos == '\n')
                ++m_pos;
            else {
                int numAttrs = m_sch->numberOfAttributes();
                char *val;
                int attrNum = 0;
                while (attrNum < numAttrs) {
                    switch (m_sch->getType(attrNum)) {
                        case DOUBLE:
                            m_next->setDouble(attrNum, strtod(m_pos, &val));
                            break;
                        case FLOAT:
                            m_next->setFloat(attrNum, strtof(m_pos, &val));
                            break;
                        case INT:
                            m_next->setInt(attrNum, strtol(m_pos, &val, 0));
                            break;
                        //case STRING:
                        //    m_next->setString(attrNum, m_pos);
                        //    break;
                    }
                    m_pos = val + 1;
                    attrNum++;
                }
                m_hasNext = attrNum == numAttrs;
            }
        return m_curr;
    }
private:
    char *m_pos;
    char *m_end;
    char m_delim;
    bool m_hasNext;
    char *m_buff1;
    char *m_buff2;
    RowTuple *m_curr;
    RowTuple *m_next;
};
}
#endif
