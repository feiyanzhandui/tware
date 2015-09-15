#ifndef LOAD_OP_TEMPLATE_H
#define LOAD_OP_TEMPLATE_H

#include <string>

#include "codegen/Printer.h"
#include "codegen/template/Template.h"
#include "codegen/template/data/DataTemplate.h"
#include "codegen/template/op/OpTemplate.h"

using std::string;

namespace tupleware {
class LoadOpTemplate : public Template {
public:
    LoadOpTemplate(DataTemplate *inTempl) : Template() {
        m_inTempl = inTempl;
    }

    virtual void generate(Printer &print) {
        string inName = m_inTempl->name();
        print << "auto t" << m_id << " = " << inName << "[i" << m_inTempl->id()
                  << "];\n";
    }
private:
    DataTemplate *m_inTempl;
};
}
#endif
