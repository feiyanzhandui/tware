#ifndef WRITER_H
#define WRITER_H

namespace tupleware {
template <typename T>
class Writer {
public:
    virtual void write(T *val) = 0;
};
}
#endif
