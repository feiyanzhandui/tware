#include "LogitRegression.h"

int main() {
    int numWorkers = 2;
    double conv = 0.05;
    int maxIterations = 100000;
    global(numWorkers, conv, maxIterations);
}
