#ifndef OPERATOR_STRATEGY_H
#define OPERATOR_STRATEGY_H

#include "codegen/strategy/Strategy.h"
#include "codegen/template/UdfTemplate.h"

namespace tupleware {
class OperatorStrategy : public Strategy {
public:
    virtual bool split(UdfTemplate *udfTempl) {
        return true;
    }
};
}
#endif
