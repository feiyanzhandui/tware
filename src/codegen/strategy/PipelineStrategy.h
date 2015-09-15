#ifndef PIPELINE_STRATEGY_H
#define PIPELINE_STRATEGY_H

#include "codegen/strategy/Strategy.h"
#include "codegen/template/UdfTemplate.h"

namespace tupleware {
class PipelineStrategy : public Strategy {
public:
    virtual bool split(UdfTemplate *udfTempl) {
        return false;
    }
};
}
#endif
