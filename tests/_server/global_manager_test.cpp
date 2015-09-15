#include <iostream>
#include <string>

#include "server/GlobalManager.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting Global Manager test..." << endl << endl;

/*
    GlobalManager gm1 = GlobalManager(NULL, NULL, NULL, NULL);

    gm1.addFile("exmaple file name here...");

    FileMessage *m1 = gm1.getFile("example file name here...");

    gm2.removeFile("example file name here...");

    gm1.addHdfs("example file name 2");

    HdfsMessage hdfsM1 = gm1.getHdfs("example file name 2");

    gm1.removeHdfs("exmaple file name 2");

    gm1.addFile("exmaple file name here...");
    gm1.addHdfs("example file name 2");
    //gm1.start()
*/
    cout << endl << "Global Manager test complete." << endl << endl;
    return 0;
}
