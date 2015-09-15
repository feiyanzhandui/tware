#include <iostream>
#include <string>

#include "server/LoadBalancer.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting Load Balancer test..." << endl << endl;

    LoadBalancer lb = LoadBalancer();
    lb.run();

    cout << endl << "Global Load Balancer complete." << endl << endl;
    return 0;
}
