#ifndef RESULT_H
#define RESULT_H

#include "serializer/Archive.h"
#include "serializer/Serializable.h"

namespace tupleware {
class Result : public Serializable {
public:
    Result() : Serializable() {
    }

    ~Result() {}

    virtual void serialize(Archive *ar) {
    }

    virtual void deserialize(Archive *ar) {
    }
};
}
#endif
