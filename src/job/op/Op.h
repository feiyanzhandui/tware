#ifndef OP_H
#define OP_H

#include <string>

#include "serializer/Serializable.h"

using std::string;

namespace tupleware {
class Op : public Serializable {
public:
    virtual ~Op() {}
protected:
    Op() {}
};
}
#endif
