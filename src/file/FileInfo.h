#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <string>

using std::string;

namespace tupleware {
class FileInfo : public Serializable {
public:
    FileInfo(string dataType) {
        m_dataType = dataType;
    }

    string dataType() {
        return m_dataType;
    }
private:
    string m_dataType;
};
}
#endif
