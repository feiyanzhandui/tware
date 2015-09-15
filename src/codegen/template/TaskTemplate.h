#ifndef TASK_TEMPLATE_H
#define TASK_TEMPLATE_H

#include <string>
#include <vector>

#include "codegen/Printer.h"
#include "codegen/template/Template.h"
#include "codegen/template/UdfTemplate.h"

using std::string;
using std::vector;

namespace tupleware {
class TaskTemplate : public Template {
public:
    TaskTemplate(uint64_t taskId) : Template() {
        m_taskId = taskId;
    }

    vector<string> filenames() {
        return m_filenames;
    }

    vector<UdfTemplate *> udfTemplates() {
        return m_udfTempls;
    }

    void addFilename(string filename) {
        m_filenames.push_back(filename);
    }

    void addUdfTemplate(UdfTemplate *udfTempl) {
        m_udfTempls.push_back(udfTempl);
    }

    void addBodyTemplate(Template *bodyTempl) {
        m_bodyTempls.push_back(bodyTempl);
    }

    virtual void generate(Printer &print) {
        print << "#include <array>\n"
              << "#include <string>\n"
              << "#include <tuple>\n"
              << "#include <unordered_map>\n"
              << "#include <valarray>\n"
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
              << "using std::slice;\n"
              << "using std::string;\n"
              << "using std::tuple;\n"
              << "using std::unordered_map;\n"
              << "using std::valarray;\n"
              << "using std::vector;\n"
              << "\n"
              << "using namespace tupleware;\n"
              << "\n";

//print << "#include <iostream>\n"
//      << "using namespace std;\n\n";

        for (UdfTemplate *udfTempl : m_udfTempls)
            udfTempl->generate(print);
        print << "\n";

        print << "class TaskExecutor" << m_taskId << " : public Executor {\n"
              << "public:\n" << IN
              << "TaskExecutor" << m_taskId << "(Context *ctx, InprocSocket *sock)"
                  << " : Executor(ctx, sock) {}\n"
              << "\n"
              << "virtual void run() {\n" << IN;
              //<< "m_sock->connect();\n";

        for (int i = 0; i < m_bodyTempls.size(); i++) {
            Template *bodyTempl = m_bodyTempls[i];
            bodyTempl->setId(i);
            bodyTempl->generate(print);
        }

        print << "sendResult();\n"
              << "m_sock->close();\n"
              << OUT << "}\n"
              << OUT << "};\n"
              << "\n"
              << "TaskExecutor" << m_taskId << " *execute(Context *ctx, "
                  << "InprocSocket *sock) {\n" << IN
              << "return new TaskExecutor" << m_taskId << "(ctx, sock);\n" << OUT
              << "}\n";
    }
private:
    uint64_t m_taskId;
    vector<string> m_filenames;
    vector<UdfTemplate *> m_udfTempls;
    vector<Template *> m_bodyTempls;
};
}
#endif
