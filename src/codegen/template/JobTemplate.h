#ifndef JOB_TEMPLATE_H
#define JOB_TEMPLATE_H

#include <cstdint>
#include <vector>

#include "codegen/Printer.h"
#include "codegen/template/TaskTemplate.h"
#include "codegen/template/Template.h"
#include "codegen/template/UdfTemplate.h"

using std::vector;

namespace tupleware {
class JobTemplate : public Template {
public:
    JobTemplate(uint64_t jobId) : Template() {
        m_jobId = jobId;
    }

    vector<TaskTemplate *> taskTemplates() {
        return m_taskTempls;
    }

    vector<UdfTemplate *> udfTemplates() {
        return m_udfTempls;
    }

    void addUdfTemplate(UdfTemplate *udfTempl) {
        m_udfTempls.push_back(udfTempl);
    }

    void addBodyTemplate(Template *bodyTempl) {
        bodyTempl->setId(m_bodyTempls.size());
        m_bodyTempls.push_back(bodyTempl);
    }

    void addTaskTemplate(TaskTemplate *taskTempl) {
        m_taskTempls.push_back(taskTempl);
    }

    virtual void generate(Printer &print) {
        print << "#include <array>\n"
              << "#include <string>\n"
              << "#include <tuple>\n"
              << "#include <unordered_map>\n"
              << "#include <vector>\n"
              << "\n"
              << "#include \"file/Block.h\"\n"
              << "#include \"frontend/Context.h\"\n"
              << "#include \"job/Executor.h\"\n"
              << "#include \"job/Result.h\"\n"
              << "#include \"net/InprocSocket.h\"\n"
              << "#include \"util/Runnable.h\"\n"
              << "#include \"net/Socket.h\"\n"
              << "\n"
              << "using std::array;\n"
              << "using std::get;\n"
              << "using std::string;\n"
              << "using std::tuple;\n"
              << "using std::unordered_map;\n"
              << "using std::vector;\n"
              << "\n"
              << "using namespace tupleware;\n"
              << "\n";

        for (UdfTemplate *udfTempl : m_udfTempls)
            udfTempl->generate(print);
        print << "\n";

        print << "class JobExecutor" << m_jobId << " : public Executor {\n"
              << "public:\n" << IN
              << "JobExecutor" << m_jobId << "(Context *ctx, InprocSocket *sock)"
                  << " : Executor(ctx, sock) {}\n"
              << "\n"
              << "virtual void run() {\n" << IN
              << "sendResult();\n";

        for (int i = 0; i < m_bodyTempls.size(); i++) {
            Template *bodyTempl = m_bodyTempls[i];
            bodyTempl->setId(i);
            bodyTempl->generate(print);
        }

        print << "\n"
              << "sendResult();\n"
              << "m_sock->close();\n"
              << OUT << "}\n"
              << OUT << "};\n"
              << "\n"
              << "JobExecutor" << m_jobId << " *execute(Context *ctx, "
                  << "InprocSocket *sock) {\n" << IN
              << "return new JobExecutor" << m_jobId << "(ctx, sock);\n" << OUT
              << "}\n";
    }
private:
    uint64_t m_jobId;
    vector<UdfTemplate *> m_udfTempls;
    vector<Template *> m_bodyTempls;
    vector<TaskTemplate *> m_taskTempls;
};
}
#endif
