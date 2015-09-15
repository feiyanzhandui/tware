#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>

#include "JoinWorker.h"
#include "Socket.h"
#include "TcpSocket.h"
#include "Util.h"
#include "Worker.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
    int numWorkers = atoi(argv[1]);
    string lhsFile = argv[2];
    string rhsFile = argv[3];
    string outFile = argv[4];
    string masterAddr = argv[5];
    vector<string> workerAddrs;
    char *addr = strtok(argv[6], ",");
    while (addr != NULL) {
        workerAddrs.push_back(string(addr));
        addr = strtok(NULL, ",");
    }

    TcpSocket::setMaxSockets(SOCKS);
    pthread_t t[numWorkers];
    Worker *workers[numWorkers];
    for (int i = 0; i < numWorkers; i++) {
        Socket *in = new TcpSocket(masterAddr, PORT1, ZMQ_SUB);
        Socket *out = new TcpSocket(masterAddr, PORT2, ZMQ_PUSH);
        Socket *lhsSock = new TcpSocket("*", PORT3 + i, ZMQ_PULL);
        vector<Socket *> rhsSocks;
        for (string addr : workerAddrs)
            for (int j = 0; j < numWorkers; j++)
                rhsSocks.push_back(new TcpSocket(addr, PORT3 + j, ZMQ_PUSH));
        string iStr = i < 10 ? "0" + to_string(i) : to_string(i);
        string iLhsFile = lhsFile + iStr;
        string iRhsFile = rhsFile + iStr;
        string iOutFile = outFile + iStr;
        workers[i] = new JoinWorker(in, out, iLhsFile, iRhsFile, iOutFile,
                                 lhsSock, rhsSocks);
    }
    for (int i = 0; i < numWorkers; i++)
        pthread_create(&t[i], NULL, Worker::run, (void *) workers[i]);
    for (int i = 0; i < numWorkers; i++)
        pthread_join(t[i], NULL);
    for (int i = 0; i < numWorkers; i++)
        delete workers[i];

    return 0;
}
