#include <iostream>
#include <string>

#include "message/JobMessage.pb.h"
#include "net/ZmqSocket.h"
#include "util/Defaults.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    string addr = argv[1];
    int data = atoi(argv[2]);
    int write = atoi(argv[3]);
    int read = atoi(argv[4]);
    string file = argv[5];

    JobMessage *jobMsg = new JobMessage();
    jobMsg->set_stored_proc(1);
    jobMsg->set_filename(file);
    jobMsg->set_data(data);
    jobMsg->set_write(write);
    jobMsg->set_read(read);
    jobMsg->set_exec(write + read);

    Socket *jobSock = new ZmqSocket(addr, Defaults::JOB_PORT, ZMQ_PUSH);
    jobSock->connect();
    jobSock->send(jobMsg);
    usleep(100000);
    return 0;
}
