#ifndef FILE_READER_H
#define FILE_READER_H

#include "data/Schema.h"
#include "data/Store.h"
#include "file/Block.h"
#include "io/Reader.h"
#include "job/TaskController.h"

namespace tupleware {
template <typename T>
class FileReader : public Reader {
public:
    FileReader(Schema *sch, TaskController *taskCtlr) : Reader(sch, taskCtlr) {}

    virtual bool hasNext() {
        return true;
    }

    virtual Store<T> *next() {
        return NULL;
    }
};
}
#endif
