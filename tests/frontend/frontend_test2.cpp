#include <iostream>

#include <gtest/gtest.h>

#include "frontend/TSet.h"
#include "data/Schema.h"
#include "util/Defaults.h"

using namespace tupleware;
using std::cout;
using std::endl;

TEST(Frontend, Ints) {
    pid_t pid = fork();

    if (pid == 0)
        system("$TUPLEWARE_HOME/build/bin/master > /dev/null");
    else if (pid > 0) {
        Connection *conn = Connection::getConnection("localhost", Defaults::CONNECTION_PORT);
        TSet t0 = TupleSet(conn, NULL);
        TSet *t1 = t0.map("[](int x) { return x+1;}")
                        ->map("[](int x) { return x*2;}")
                        ->map("[](int x) { return x/5;}")
                        ->evaluate();
        system("pkill -9 master");
    }
    else
        cout << "fork() failed!" << endl;

    Schema *s1 = new Schema();
    s1->addInt();
}
