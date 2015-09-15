#include <iostream>
#include <string>

#include "file/Binner.h"
#include "file/BasicBinner.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting binner test..." << endl << endl;

    BasicBinner bb = BasicBinner();

    cout << endl << "binner test complete." << endl << endl;
    return 0;
}
