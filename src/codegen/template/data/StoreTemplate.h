#ifndef STORE_TEMPLATE_H
#define STORE_TEMPLATE_H

#include <string>

#include "codegen/Printer.h"
#include "codegen/template/data/DataTemplate.h"

using std::string;

namespace tupleware {
class StoreTemplate : public DataTemplate {
public:
    StoreTemplate(string valType) : DataTemplate("d") {
        m_valType = valType;
    }

    virtual void generate(Printer &print) {
        print << "vector<" << m_valType << "> d" << m_id;
        if (m_inTempl == NULL)
            print << " = vector<" << m_valType << ">()";
        else
            print << " = b" << "0" << "->data()";
        print << ";\n";
    }
private:
    string m_valType;
};
}
#endif
