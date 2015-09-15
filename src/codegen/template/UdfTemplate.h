#ifndef UDF_TEMPLATE_H
#define UDF_TEMPLATE_H

#include "codegen/Printer.h"
#include "codegen/template/Template.h"
#include "job/Udf.h"

namespace tupleware {
class UdfTemplate : public Template {
public:
    UdfTemplate(Udf *udf) : Template() {
        m_udf = udf;
    }

    Udf *udf() {
        return m_udf;
    }
/*
        //vector<string> udfArgTypes = m_udfs[udfId]->argumentTypes();
        //UdfTemplate *udfTempl = new UdfTemplate(udfId, udfRetType, udfArgTypes);
        //m_taskTempls.front()->addUdf(udfTempl);

    int m_id;
    string m_llvm;
    string m_retType;
    vector<string> m_argTypes;
    bool m_isVect;
    int m_compCycles;
    int m_loadCycles;

        string retType = "ERR";
        vector<string> types = {"double", "float", "i32", "i64"};
        for (string type : types)
            if ((llvm.compare(35, type.length(), type)) == 0)
                retType = type;
            Udf *udf = new Udf(i, llvm, retType, vector<string>(), false, 1, 1);
*/

    virtual void generate(Printer &print) {
        print << m_udf->returnType() << " " << m_udf->name() << "("
                  << m_udf->args() << ");\n";
    }
private:
    Udf *m_udf;
};
}
#endif
