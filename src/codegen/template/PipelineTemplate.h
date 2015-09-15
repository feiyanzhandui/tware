#ifndef PIPELINE_TEMPLATE_H
#define PIPELINE_TEMPLATE_H

#include <vector>

#include "codegen/Printer.h"
#include "codegen/template/Template.h"
#include "codegen/template/data/DataTemplate.h"

using std::vector;

namespace tupleware {
class PipelineTemplate : public Template {
public:
    PipelineTemplate() {
        m_inTempl = NULL;
        m_outTempl = NULL;
    }

    void setInputTemplate(DataTemplate *inTempl) {
        m_inTempl = inTempl;
    }

    void setOutputTemplate(DataTemplate *outTempl) {
        m_outTempl = outTempl;
    }

    void addOpTemplate(Template *opTempl) {
        m_opTempls.push_back(opTempl);
    }

    virtual void generate(Printer &print) {
        int inId = m_inTempl->id();
        int outId = m_outTempl->id();
        print << "for (int i" << m_id << " = 0; i" << m_id << " < d" << inId
                  << ".size(); i" << m_id << "++) {\n" << IN;
        for (int i = 0; i < m_opTempls.size(); i++) {
            Template *opTempl = m_opTempls[i];
            opTempl->setId(i);
            opTempl->generate(print);
        }
        if (m_outTempl == NULL) {
            int outId = m_outTempl->id();
            print << "d" << outId << "[i" << m_id << "] = t"
                      << m_opTempls.back()->id() << ";" << "\n";
        }
        print << OUT << "}\n";
    }
private:
    DataTemplate *m_inTempl;
    DataTemplate *m_outTempl;
    vector<Template *> m_opTempls;
};
}
#endif
