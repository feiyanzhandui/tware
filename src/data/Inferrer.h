#ifndef INFERRER_H
#define INFERRER_H

#include <string>

#include "data/Schema.h"

using std::string;

namespace tupleware {
class Inferrer {
public:
    virtual Schema *infer(string filename) = 0;
};
}
#endif
