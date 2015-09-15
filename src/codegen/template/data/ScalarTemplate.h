#ifndef SCALAR_TEMPLATE_H
#define SCALAR_TEMPLATE_H

#include <string>

#include "codegen/Printer.h"
#include "codegen/template/data/DataTemplate.h"

using std::string;

namespace tupleware {
class ScalarTemplate : public DataTemplate {
public:
    ScalarTemplate(string valType) : DataTemplate("d") {
        m_valType = valType;
    }

    virtual void generate(Printer &print) {
        print << m_valType << " d" << m_id;
        if (m_inTempl == NULL)
            print << " = " << m_valType << "(90)";
        else
            print << " = b" << "0" << "->data()";
        print << ";\n";
    }
private:
    string m_valType;
};
}
#endif
