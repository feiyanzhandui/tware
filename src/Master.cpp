#include <cstdlib>
#include <iostream>

#include "server/Master.h"
#include "util/Runnable.h"
#include "util/Thread.h"

using std::cout;
using std::endl;

using tupleware::Master;
using tupleware::Runnable;
using tupleware::Thread;

int main(int argc, const char *argv[]) {
    //int sys = system("clear");
    //cout << "***** Welcome to Tupleware! *****\n"
    //     << "Starting up Master...\n" << endl;

    Master *master = new Master(4000, 4001);
    Thread *mt = new Thread(master);
    mt->start();
    mt->join();
    delete master;
    return 0;
}
