#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <cstring>
#include <string>
#include <zmq.h>

#include "net/Socket.h"
#include "serializer/Serializable.h"
#include "serializer/Serializer.h"

using std::memcpy;
using std::string;
using std::to_string;

namespace tupleware {
class TcpSocket : public Socket {
public:
    TcpSocket(string addr, int port, int sockType)
            : Socket("tcp://" + addr + ":" + to_string(port), sockType) {
        m_addr = addr;
        m_port = port;
    }

    ~TcpSocket() {}

    virtual int send(Serializable *msg) {
        string data = Serializer::serialize(msg);
        zmq_msg_t zmsg;
        zmq_msg_init_size(&zmsg, data.size());
        memcpy(zmq_msg_data(&zmsg), data.data(), data.size());
        int send = zmq_msg_send(&zmsg, m_sock, 0);
        if (send < 0)
            Logging::error(__FILE__, __LINE__, m_connStr + ":" + strerror(errno));
        zmq_msg_close(&zmsg);
        return send;
        //zmq_msg_t zmsg;
        //zmq_msg_init_data(&zmsg, (void *) &msg, sizeof(&msg), NULL, NULL);
        //int send = zmq_msg_send(&zmsg, m_sock, 0);
        //zmq_msg_close(&zmsg);
        //return send;
    }

    virtual Serializable *receive() {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        int recv = zmq_msg_recv(&zmsg, m_sock, 0);
        char *data = new char[recv];
        memcpy(data, zmq_msg_data(&zmsg), recv);
        zmq_msg_close(&zmsg);
        Serializable *msg = Serializer::deserialize(string(data));
        delete[] data;
        return msg;
    }
private:
    string m_addr;
    int m_port;
};
}
#endif
