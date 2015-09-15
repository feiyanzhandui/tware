#ifndef PARSER_H
#define PARSER_H

#include "data/Schema.h"

namespace tupleware {
template <typename T>
class Parser {
public:
    virtual bool hasNext() = 0;
    virtual T *next() = 0;
protected:
    Schema *m_sch;
};
}
#endif
