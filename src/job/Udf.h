#ifndef UDF_H
#define UDF_H

#include <string>

#include "compiler/Compiler.h"
#include "serializer/Archive.h"
#include "serializer/Serializable.h"

using std::string;

namespace tupleware {
class Udf : public Serializable {
public:
    static Udf *getUdf(string fun) {
        int argStart = fun.find("(");
        int argEnd = fun.find(")");
        int nameStart = fun.substr(0, argStart).rfind(" ") + 1;

        string name = fun.substr(nameStart, argStart - nameStart);
        string retType = fun.substr(0, nameStart - 1);
        string args = fun.substr(argStart + 1, argEnd - argStart - 1);

        string code = "#include <array>\n"
                      "#include <cmath>\n"
                      "#include <string>\n"
                      "#include <tuple>\n"
                      "#include <unordered_map>\n"
                      "#include <valarray>\n"
                      "#include <vector>\n"
                      "\n"
                      "#include \"frontend/Context.h\"\n"
                      "\n"
                      "using std::array;\n"
                      "using std::get;\n"
                      "using std::make_tuple;\n"
                      "using std::slice;\n"
                      "using std::string;\n"
                      "using std::tuple;\n"
                      "using std::unordered_map;\n"
                      "using std::valarray;\n"
                      "using std::vector;\n"
                      "\n"
"#include <iostream>\nusing namespace std;\n"
                      "using namespace tupleware;\n" +
                      fun + "\n";

        string llvm = Compiler::getLlvm(code);
        return new Udf(name, retType, args, llvm);
    }

    Udf() : Serializable() {}

    Udf(string name, string retType, string args, string llvm)
            : Serializable () {
        m_name = name;
        m_retType = retType;
        m_args = args;
        m_llvm = llvm;
    }

    string name() {
        return m_name;
    }

    string returnType() {
        return m_retType;
    }

    string args() {
        return m_args;
    }

    string llvm() {
        return m_llvm;
    }

    virtual void serialize(Archive *ar) {
        ar->save(m_name);
        ar->save(m_retType);
        ar->save(m_args);
        ar->save(m_llvm);
    }

    virtual void deserialize(Archive *ar) {
        ar->load(m_name);
        ar->load(m_retType);
        ar->load(m_args);
        ar->load(m_llvm);
    }
private:
    string m_name;
    string m_retType;
    string m_args;
    string m_llvm;
};
}
#endif
