#include "LogitRegression.h"

int main() {
    int workerNum = 2;
    string filename =  getenv("TUPLEWARE_HOME") + "/test/logit_regression/test2.csv";
    local(workerNum, filename);
}
