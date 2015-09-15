#include <cstdlib>
#include <pthread.h>

#include "JoinMaster.h"
#include "Master.h"
#include "Socket.h"
#include "TcpSocket.h"
#include "Util.h"

int main(int argc, char *argv[]) {
    int numWorkers = atoi(argv[1]);

    TcpSocket::setMaxSockets(SOCKS);
    Socket *in = new TcpSocket("*", PORT2, ZMQ_PULL);
    Socket *out = new TcpSocket("*", PORT1, ZMQ_PUB);
    pthread_t t;
    Master *master = new JoinMaster(numWorkers, in, out);
    pthread_create(&t, NULL, Master::run, (void *) master);
    pthread_join(t, NULL);
    delete master;

    return 0;
}
