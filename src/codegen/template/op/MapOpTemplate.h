#ifndef MAP_OP_TEMPLATE_H
#define MAP_OP_TEMPLATE_H

#include <string>

#include "codegen/Printer.h"
#include "codegen/template/Template.h"
#include "codegen/template/UdfTemplate.h"
#include "codegen/template/op/OpTemplate.h"

using std::string;

namespace tupleware {
class MapOpTemplate : public Template {
public:
    MapOpTemplate(Template *srcTempl, UdfTemplate *udfTempl) : Template() {
        m_srcTempl = srcTempl;
        m_udfTempl = udfTempl;
    }

    UdfTemplate *udfTemplate() {
        return m_udfTempl;
    }

    virtual void generate(Printer &print) {
        int src = m_srcTempl->id();
        string udf = m_udfTempl->udf()->name();
        print << "auto t" << m_id << " = " << udf << "(t" << src;
        if (m_udfTempl->udf()->args().find("Context") != string::npos)
            print << ", m_ctx";
        print << ");\n";
    }
private:
    Template *m_srcTempl;
    UdfTemplate *m_udfTempl;
};
}
#endif
