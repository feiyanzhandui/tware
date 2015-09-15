#ifndef REDUCE_OP_TEMPLATE_H
#define REDUCE_OP_TEMPLATE_H

#include <string>

#include "codegen/Printer.h"
#include "codegen/template/Template.h"
#include "codegen/template/UdfTemplate.h"
#include "codegen/template/op/OpTemplate.h"

using std::to_string;

namespace tupleware {
class ReduceOpTemplate : public Template {
public:
    ReduceOpTemplate(Template *srcTempl, UdfTemplate *udfTempl,
            UdfTemplate *keyTempl, Template *outTempl) : Template() {
        m_srcTempl = srcTempl;
        m_udfTempl = udfTempl;
        m_keyTempl = keyTempl;
        m_outTempl = outTempl;
    }

    UdfTemplate *udf() {
        return m_udfTempl;
    }

    UdfTemplate *key() {
        return m_keyTempl;
    }

    virtual void generate(Printer &print) {
        int src = m_srcTempl->id();
        int out = m_outTempl->id();
        string udf = m_udfTempl->udf()->name();
        if (m_keyTempl != NULL) {
            string key = m_keyTempl->udf()->name();
            print << "auto k" << m_id << " = " << key << "(t" << src << ");\n"
                  << "auto v" << m_id << " = d" << out << ".find(k" << m_id
                      << ");\n"
                  << "if (v" << m_id << " == d" << out << ".end()) {\n" << IN
                  << "d" << out << "[k" << m_id << "] = t" << src << ";\n"
                  << OUT << "}\n"
                  << "else {\n" << IN
                  << "d" << out << "[k" << m_id << "] = " << udf << "(t" << src
                      << ", v" << m_id << "->second);\n"
                  << OUT << "}\n";
        }
        else
            print << "auto t" << m_id << " = d" << out << ";\n"
                  << "d" << out << " = " << udf << "(t" << m_id << ", t" << src
                      << ");\n";
    }
private:
    Template *m_srcTempl;
    UdfTemplate *m_udfTempl;
    UdfTemplate *m_keyTempl;
    Template *m_outTempl;
};
}
#endif
