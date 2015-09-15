#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "codegen/optimizer/Optimizer.h"

namespace tupleware {
class Optimizer {
public:
    virtual ~Optimizer() {}
    virtual void opt() = 0;
};
}
#endif
