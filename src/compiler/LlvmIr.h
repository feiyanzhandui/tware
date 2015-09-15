#ifndef LLVM_IR_H
#define LLVM_IR_H

#include <string>

#include "serializer/Serializable.h"

using std::string;

namespace tupleware {
class LlvmIr : public Serializable {
public:
    LlvmIr(string llvm) : Serializable() {
        m_llvm = llvm;
    }

    string llvm() {
        return m_llvm;
    }

    virtual void serialize() {
    }
private:
    string m_llvm;
};
}
#endif
