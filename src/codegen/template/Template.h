#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "codegen/Printer.h"

namespace tupleware {
class Template {
public:
    virtual ~Template() {}

    int id() {
        return m_id;
    }

    void setId(int id) {
        m_id = id;
    }

    virtual void generate(Printer &print) = 0;
protected:
    int m_id;

    Template() {
        m_id = 999;
    }
};
}
#endif
