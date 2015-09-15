#include <cstdlib>
#include <iostream>
#include <string>

#include "server/Worker.h"
#include "util/Logging.h"
#include "util/Runnable.h"
#include "util/Thread.h"

using std::cout;
using std::endl;
using std::string;

using tupleware::Logging;
using tupleware::Runnable;
using tupleware::Thread;
using tupleware::Worker;

int main(int argc, const char *argv[]) {
    if (argc < 3) {
        Logging::fatal(__FILE__, __LINE__, "Master info required");
        return 1;
    }

    //int sys = system("clear");
    //cout << "***** Welcome to Tupleware! *****\n"
    //     << "Starting up Worker...\n" << endl;

    string addr = argv[1];
    string myAddr = argv[2];
    //int port = atoi(argv[2]);
    Worker *worker = new Worker(addr, myAddr);
    Thread *wt = new Thread(worker);
    wt->start();
    wt->join();
    delete worker;
    return 0;
}
