#ifndef COLLECTOR_TEMPLATE_H
#define COLLECTOR_TEMPLATE_H

#include <string>

#include "codegen/Printer.h"
#include "codegen/template/data/DataTemplate.h"

using std::string;

namespace tupleware {
class CollectorTemplate : public DataTemplate {
public:
    CollectorTemplate(string keyType, string valType) : DataTemplate("d") {
        m_keyType = keyType;
        m_valType = valType;
    }

    virtual void generate(Printer &print) {
        print << "unordered_map<" << m_keyType << "," << m_valType << "> d"
                  << m_id;
        if (m_inTempl == NULL)
            print << " = unordered_map<" << m_keyType << "," << m_valType
                      << ">()";
        else
            print << " = b" << "0" << "->data()";
        print << ";\n";
    }
private:
    string m_keyType;
    string m_valType;
};
}
#endif
