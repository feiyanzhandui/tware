#ifndef PRINTER_H
#define PRINTER_H

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace tupleware {
class Printer {
public:
    Printer() {
        m_stream = new ostringstream();
        m_line = new ostringstream();
        m_indent = 0;
    }

    ~Printer() {
        delete m_stream;
        delete m_line;
    }

    string str() {
        return m_stream->str();
    }

    void indent() {
        m_indent += 2;
    }

    void unindent() {
        m_indent -= 2;
    }

    template <typename T>
    Printer &operator<<(const T &val) {
        (*m_line) << val;
        return *this;
    }

    Printer &operator<<(const char *val) {
        (*m_line) << val;
        if (string(val).back() == '\n') {
            (*m_stream) << string(m_indent, ' ') << m_line->str();
            m_line->str(string());
        }
        return *this;
    }

    Printer &operator<<(Printer &(*f)(Printer &)) {
        return f(*this);
    }
private:
    ostringstream *m_stream;
    ostringstream *m_line;
    int m_indent;
};

Printer &IN(Printer &print) {
    print.indent();
    return print;
}

Printer &OUT(Printer &print) {
    print.unindent();
    return print;
}
}
#endif
