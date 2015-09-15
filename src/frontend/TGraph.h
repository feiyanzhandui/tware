#ifndef T_GRAPH_H
#define T_GRAPH_H

#include <dirent.h>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "compiler/ClangCompiler.h"
#include "data/Schema.h"
#include "job/JobValidator.h"
#include "message/AttributeMessage.pb.h"
#include "message/ExpressionMessage.pb.h"
#include "message/JobMessage.pb.h"
#include "net/Connection.h"
#include "net/TcpConnection.h"
#include "util/Logger.h"
#include "util/LogLevel.h"

using std::ifstream;
using std::string;

namespace tupleware {
class TGraph {
public:
    TGraph(Input *in) {
    }

    void eval(string addr, string functDir) {
    }
/*
    bool hasNext() {
        return true;
    }

    T *next() {
        return NULL;
    }
*/
protected:
    ExpressionMessage *m_expr;

    void addAttribute(Schema *sch, int attrNum, AttributeMessage *attr) {
        switch (sch->getType(attrNum)) {
            case DOUBLE:
                attr->set_type(AttributeMessage::DOUBLE);
                break;
            case FLOAT:
                attr->set_type(AttributeMessage::FLOAT);
                break;
            case INT:
                attr->set_type(AttributeMessage::INT);
                break;
            case STRING:
                attr->set_type(AttributeMessage::STRING);
                attr->set_length(sch->getLength(attrNum));
                break;
            default:
                break;
        }
    }
};
}
#endif
