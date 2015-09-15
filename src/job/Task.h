#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

#include "frontend/Context.h"
#include "serializer/Archive.h"
#include "serializer/Serializable.h"

using std::string;
using std::vector;

namespace tupleware {
class Task : public Serializable {
public:
    Task() : Serializable() {}

    Task(string llvm, vector<string> filenames, int numThreads, Context *ctx)
            : Serializable() {
        m_llvm = llvm;
        m_filenames = filenames;
        m_numThreads = numThreads;
        m_ctx = ctx;
    }

    string llvm() {
        return m_llvm;
    }

    vector<string> filenames() {
        return m_filenames;
    }

    int numberOfThreads() {
        return m_numThreads;
    }

    Context *context() {
        return m_ctx;
    }

    virtual void serialize(Archive *ar) {
        ar->save(m_llvm);
        ar->save(m_filenames);
        ar->save(m_numThreads);
        m_ctx->serialize(ar);
    }

    virtual void deserialize(Archive *ar) {
        ar->load(m_llvm);
        ar->load(m_filenames);
        ar->load(m_numThreads);
        m_ctx->deserialize(ar);
    }
private:
    string m_llvm;
    vector<string> m_filenames;
    int m_numThreads;
    Context *m_ctx;
};
}
#endif
