#ifndef DATA_TEMPLATE_H
#define DATA_TEMPLATE_H

#include <string>

#include "codegen/Printer.h"
#include "codegen/template/Template.h"

using std::string;
using std::to_string;

namespace tupleware {
class DataTemplate : public Template {
public:
    virtual ~DataTemplate() {}

    string name() {
        return m_name + to_string(m_id);
    }

    void setInputTemplate(Template *inTempl) {
        m_inTempl = inTempl;
    }
protected:
    string m_name;
    Template *m_inTempl;

    DataTemplate(string name) : Template() {
        m_name = name;
        m_inTempl = NULL;
    }
};
}
#endif
