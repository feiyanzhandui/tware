#ifndef STRATEGY_H
#define STRATEGY_H

#include "codegen/template/UdfTemplate.h"

namespace tupleware {
class Strategy {
public:
    virtual ~Strategy() {}
    virtual bool split(UdfTemplate *udfTempl) = 0;
};
}
#endif
