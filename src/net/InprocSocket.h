#ifndef INPROC_SOCKET_H
#define INPROC_SOCKET_H

#include <cstring>
#include <string>
#include <zmq.h>

#include "net/Socket.h"
#include "serializer/Serializable.h"
#include "util/Logging.h"

using std::memcpy;
using std::string;
using std::to_string;


#include "serializer/Serializer.h"
#include <iostream>
using namespace std;

namespace tupleware {
class InprocSocket : public Socket {
public:
    InprocSocket(string endpoint, int sockType)
            : Socket("inproc://" + endpoint, sockType) {
        m_endpoint = endpoint;
    }

    ~InprocSocket() {}

    virtual int send(Serializable *msg) {
//Serializer *ser = new Serializer();
//msg->serialize(ser);
//cout << ser->serialize() << endl;
        zmq_msg_t zmsg;
        zmq_msg_init_size(&zmsg, sizeof(msg));
        memcpy(zmq_msg_data(&zmsg), &msg, sizeof(msg));
        int send = zmq_msg_send(&zmsg, m_sock, 0);
        if (send < 0)
            Logging::error(__FILE__, __LINE__, m_connStr + ":" + strerror(errno));
        zmq_msg_close(&zmsg);
        return send;
    }

    virtual Serializable *receive() {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        int recv = zmq_msg_recv(&zmsg, m_sock, 0);
        if (recv <= 0)
            Logging::error(__FILE__, __LINE__, m_connStr + ":" + strerror(errno));
        Serializable *msg = NULL;
        memcpy(&msg, zmq_msg_data(&zmsg), recv);
        zmq_msg_close(&zmsg);
        return msg;
    }
private:
    string m_endpoint;
};
}
#endif
