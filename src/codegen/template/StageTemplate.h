#ifndef STAGE_TEMPLATE_H
#define STAGE_TEMPLATE_H

#include <cstdint>
#include <string>
#include <vector>

#include "codegen/Printer.h"
#include "codegen/template/PipelineTemplate.h"
#include "codegen/template/Template.h"
#include "codegen/template/data/DataTemplate.h"
#include "file/Request.h"

using std::string;
using std::vector;

namespace tupleware {
class StageTemplate : public Template {
public:
    StageTemplate(Request::Type type, string filename, uint64_t blkId)
            : Template() {
        m_type = type;
        m_filename = filename;
        m_blkId = blkId;
        m_inTempl = NULL;
        m_outTempl = NULL;
    }

    void setInputTemplate(DataTemplate *inTempl) {
        m_inTempl = inTempl;
    }

    void setOutputTemplate(DataTemplate *outTempl) {
        m_outTempl = outTempl;
    }

    void addDataTemplate(DataTemplate *dataTempl) {
        m_dataTempls.push_back(dataTempl);
    }

    void addPipelineTemplate(PipelineTemplate *pipeTempl) {
        m_pipeTempls.push_back(pipeTempl);
    }

    virtual void generate(Printer &print) {
        m_inTempl->setId(0);
        m_outTempl->setId(1);
        //m_inTempl->generate(print);
        m_outTempl->generate(print);
        print << "Block *b" << "0" << " = getAny(\"" << m_filename << "\");\n"
              << "while (b" << "0" << " != NULL) {\n" << IN;
        m_inTempl->generate(print);

        for (int i = 0; i < m_dataTempls.size(); i++) {
            Template *dataTempl = m_dataTempls[i];
            dataTempl->setId(i);
            dataTempl->generate(print);
        }

        for (int i = 0; i < m_pipeTempls.size(); i++) {
            PipelineTemplate *pipeTempl = m_pipeTempls[i];
            pipeTempl->setId(i);
            pipeTempl->generate(print);
        }

        print << "b0 = getAny(\"" << m_filename << "\");\n"
              << OUT << "}\n"
              << "Block *b" << "1" << " = new Block(0, d1);\n"
              << "addBlock(\"TEST_red\", b" << "1" << ");\n";
    }
private:
    Request::Type m_type;
    string m_filename;
    uint64_t m_blkId;
    DataTemplate *m_inTempl;
    DataTemplate *m_outTempl;
    vector<DataTemplate *> m_dataTempls;
    vector<PipelineTemplate *> m_pipeTempls;
};
}
#endif
