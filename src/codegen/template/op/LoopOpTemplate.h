#ifndef LOOP_OP_TEMPLATE_H
#define LOOP_OP_TEMPLATE_H

#include <string>
#include <vector>

#include "codegen/Printer.h"
#include "codegen/template/StageTemplate.h"
#include "codegen/template/Template.h"
#include "codegen/template/UdfTemplate.h"
#include "codegen/template/op/OpTemplate.h"

using std::string;
using std::vector;

namespace tupleware {
class LoopOpTemplate : public Template {
public:
    LoopOpTemplate(UdfTemplate *udfTempl) : Template() {
        m_udfTempl = udfTempl;
    }

    UdfTemplate *udfTemplate() {
        return m_udfTempl;
    }

    void addStageTemplates(vector<StageTemplate *> *stageTempls) {
        m_stageTempls = stageTempls;
    }

    virtual void generate(Printer &print) {
        string udf = m_udfTempl->udf()->name();
        print << "while (" << udf << "(c)) {\n" << IN;
        for (StageTemplate *stageTempl : *m_stageTempls)
            stageTempl->generate(print);
        print << OUT << "}\n";
    }
private:
    vector<StageTemplate *> *m_stageTempls;
    UdfTemplate *m_udfTempl;
};
}
#endif
