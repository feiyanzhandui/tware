#include "LogitRegression.h"

int main() {
    int workerNum = 1;
    string filename = getenv("TUPLEWARE_HOME") + "/test/logit_regression/test1.csv";
    local(workerNum, filename);
}
