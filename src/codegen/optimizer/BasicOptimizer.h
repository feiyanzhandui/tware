#ifndef BASIC_OPTIMIZER_H
#define BASIC_OPTIMIZER_H

#include "codegen/optimizer/Optimizer.h"

namespace tupleware {
class BasicOptimizer : public Optimizer {
public:
    virtual void opt() {}
};
}
#endif
