#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include <string>

#include "data/Schema.h"
#include "proto/AttributeInfo.pb.h"
#include "proto/ExpressionInfo.pb.h"

using std::string;

namespace tupleware {
class FileSource : public Input, Output {
public:
    FileSource(string filename) {
    }
};
}
#endif
