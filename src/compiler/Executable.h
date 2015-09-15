#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include "util/Runnable.h"

using llvm::ExecutionEngine;

#include <iostream>
using namespace std;

namespace tupleware {
class Executable {
public:
    Executable(ExecutionEngine *ee) {
        m_ee = ee;
    }

    template <typename... T>
    Runnable *getRunnable(T... args) {
        //string kayhan = "_Z8executorv";
        string NAME = "_Z7executePN9tupleware7ContextEPNS_12InprocSocketE";
        return ((Runnable *(*)(T...)) m_ee->getFunctionAddress(NAME))(args...);
    }
private:
    ExecutionEngine *m_ee;
};
}
#endif
