#ifndef READER_H
#define READER_H

#include "data/Store.h"
#include "io/Parser.h"
#include "job/TaskController.h"

namespace tupleware {
template <typename T>
class Reader {
public:
    virtual bool hasNext() = 0;
    virtual Store<T> *next() = 0;
protected:
    TaskController *m_taskCtlr;
    Parser<T> *m_pars;
    Store<T> *m_curr;
    Store<T> *m_next;

    Reader(TaskController *taskCtlr, Parser<T> *pars) {
        m_taskCtlr = taskCtlr;
        m_pars = pars;
        next();
    }
};
}
#endif
