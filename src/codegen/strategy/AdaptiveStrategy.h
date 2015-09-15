#ifndef ADAPTIVE_STRATEGY_H
#define ADAPTIVE_STRATEGY_H

#include "codegen/strategy/Strategy.h"
#include "codegen/template/UdfTemplate.h"

namespace tupleware {
class AdaptiveStrategy : public Strategy {
public:
    virtual bool split(UdfTemplate *udfTempl) {
        return false;
    }
};
}
#endif
