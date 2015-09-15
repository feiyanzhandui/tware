#ifndef HDFS_READER_H
#define HDFS_READER_H

#include "data/Schema.h"
#include "data/Store.h"
#include "file/Block.h"
#include "io/Parser.h"
#include "io/Reader.h"
#include "job/TaskController.h"

namespace tupleware {
template <typename T>
class HdfsReader : public Reader {
public:
    HdfsReader(TaskController *taskCtlr, Parser<T> *pars)
            : Reader(taskCtlr, pars) {}

    virtual bool hasNext() {
        return true;
    }

    virtual Store<T> *next() {
        return NULL;
    }
};
}
#endif
