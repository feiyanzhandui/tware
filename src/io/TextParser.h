#ifndef TEXT_PARSER_H
#define TEXT_PARSER_H

#include <fstream>
#include <string>

#include "data/Schema.h"
#include "io/Parser.h"

using std::string;

namespace tupleware {
class TextParser : public Parser<string> {
public:
    TextParser(int maxStrSize, void *buff) : Parser() {
        m_sch = new Schema();
        m_sch->addString(maxStrSize);
        m_buff = (char *) buff;
        next();
    }

    ~TextParser() {
    }

    virtual bool hasNext() {
        return false;
    }

    virtual string *next() {
        /*
        while (*m_input >> word) {
            int start = -1;
            int end = word.length();
            while (start < end && isPunctuation(word[++start]));
            while (end > start && isPunctuation(word[--end]));

            if (end > start) {
                m_next = new string(word.substr(start, end + 1));
                break;
            }
        }
        */
        return NULL;
    }
private:
    char *m_buff;

    bool isPunctuation(char &c) {
        return (c < '0' && c != '-')
                || (c > '9' && c < 'A')
                || (c > 'Z' && c < 'a')
                || c > 'z';
    }
};
}
#endif
