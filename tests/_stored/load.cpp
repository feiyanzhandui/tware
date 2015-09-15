#include <cstdint>
#include <iostream>
#include <string>

#include "message/JobMessage.pb.h"
#include "net/ZmqSocket.h"
#include "util/Defaults.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    string file = argv[1];
    uint64_t fileId = stoull(argv[2]);
    string addr = argv[3];


    JobMessage *jobMsg = new JobMessage();
    jobMsg->set_stored_proc(0);
    jobMsg->set_filename(file);
    jobMsg->set_file_id(fileId);
    if (argc > 4) {
        jobMsg->set_attr(atoi(argv[4]));
    }
    jobMsg->set_delim(",");
    Socket *jobSock = new ZmqSocket(addr, Defaults::JOB_PORT, ZMQ_PUSH);
    jobSock->connect();
    jobSock->send(jobMsg);
    usleep(100000);
    return 0;
}
