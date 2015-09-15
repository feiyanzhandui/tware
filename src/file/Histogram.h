#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "file/Binner.h"

namespace tupleware {
class Histogram {
public:
    Histogram(Binner *bin) {
        m_bin = bin;
    }
private:
    Binner *m_bin;
};
}
#endif
