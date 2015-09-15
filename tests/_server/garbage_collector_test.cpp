#include <iostream>
#include <string>

#include "server/GarbageCollector.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting Garbage Collector test..." << endl << endl;

    GarbageCollector gc = GarbageCollector();
    gc.run();

    cout << endl << "Global Garbage Collector complete." << endl << endl;
    return 0;
}
