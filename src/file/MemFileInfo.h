#ifndef TEMP_FILE_INFO_H
#define TEMP_FILE_INFO_H

#include "file/FileInfo.h"
#include "file/Histogram.h"

namespace tupleware {
class TempFileInfo : public FileInfo {
public:
    TempFileInfo() : FileInfo() {
    }
};
}
#endif
