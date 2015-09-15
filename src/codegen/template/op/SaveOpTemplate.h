#ifndef SAVE_OP_TEMPLATE_H
#define SAVE_OP_TEMPLATE_H

#include "codegen/Printer.h"
#include "codegen/template/Template.h"
#include "codegen/template/op/OpTemplate.h"

namespace tupleware {
class SaveOpTemplate : public Template {
public:
    SaveOpTemplate(Template *srcTempl) : Template() {
        m_srcTempl = srcTempl;
    }

    virtual void generate(Printer &print) {
    }
private:
    Template *m_srcTempl;
};
}
#endif
