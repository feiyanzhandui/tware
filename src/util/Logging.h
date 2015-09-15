#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <string>

using std::cerr;
using std::endl;
using std::string;

namespace tupleware {
class Logging {
public:
    static void debug(string filename, int line, string msg) {
        log("[DEBUG]: ", filename, line, msg);
    }

    static void error(string filename, int line, string msg) {
        log("[ERROR]: ", filename, line, msg);
    }

    static void fatal(string filename, int line, string msg) {
        log("[FATAL]: ", filename, line, msg);
        exit(1);
    }

    static void info(string msg) {
        log("[INFO]:  ", msg);
    }

    static void warn(string msg) {
        log("[WARN]:  ", msg);
    }
private:
    static void log(string type, string filename, int line, string msg) {
        cerr << type << filename << " at " << line << " (" << msg << ")"
             << endl;
    }

    static void log(string type, string msg) {
        cerr << type << msg << endl;
    }
};
}
#endif
