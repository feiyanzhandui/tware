#ifndef DUMMY_TEMPLATE_H
#define DUMMY_TEMPLATE_H

#include "codegen/Printer.h"
#include "codegen/template/Template.h"

namespace tupleware {
class DummyTemplate : public Template {
public:
    DummyTemplate() : Template() {
        m_id = 999;
    }

    virtual void generate(Printer &print) {
        print << "*** DUMMY ***\n";
    }
};
}
#endif
