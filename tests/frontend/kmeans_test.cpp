#include <iostream>

#include "frontend/TSet.h"
#include "frontend/Connection.h"
#include "util/Defaults.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    Connection *conn = Connection::getConnection("localhost", Defaults::CONN_PORT);
    TSet *t0 = new TSet(conn, NULL);
    TSet *t1 = t0->kmeans("float", 50, 70)->evaluate();
    cout << "Job Submitted" << endl;
    usleep(100000);
    return 0;
}
