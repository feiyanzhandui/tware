#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include <string>

#include "data/Schema.h"
#include "proto/AttributeInfo.pb.h"
#include "proto/ExpressionInfo.pb.h"

using std::string;

namespace tupleware {
class FileInput : public Input {
public:
    FileInput(string filename) {
    }
};
}
#endif
