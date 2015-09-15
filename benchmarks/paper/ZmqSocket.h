#ifndef ZMQ_SOCKET_H
#define ZMQ_SOCKET_H

#include <cstring>
#include <string>
#include <unistd.h>

#include <google/protobuf/message.h>
#include <zmq.h>

#include "net/Socket.h"

using std::memcpy;
using std::string;
using std::to_string;

using google::protobuf::Message;

namespace tupleware {
class ZmqSocket : public Socket {
public:
    static void setMaxSockets(int maxSocks) {
        zmq_ctx_set(m_ctx, ZMQ_MAX_SOCKETS, maxSocks);
    }

    static void terminate() {
        zmq_term(m_ctx);
    }

    ZmqSocket(string addr, int port, int sockType) {
        m_addr = addr;
        m_port = port;
        m_sockType = sockType;
        m_isOpen = false;
    }

    virtual bool bind() {
        m_isOpen = zmq_bind(m_sock, init().c_str()) == 0;
        return m_isOpen;
    }

    virtual bool connect() {
        m_isOpen = zmq_connect(m_sock, init().c_str()) == 0;
        return m_isOpen;
    }
/*
 * Presently, subscriptions are included in the Task packet sent by the
 * Coordinator. It may (or may not) be faster to do subscriptions using the
 * built-in ZMQ_SUBSCRIBE functionality. Benchmarking needs to be conducted to
 * see if this offers significant performance benefits.
 *
        bool addSubscription(string sub) {
            zmq_setsockopt(_sock, ZMQ_SUBSCRIBE, &sub, sub.size());
            return true;
        }

        bool removeSubscription(string sub) {
            return false;
        }
*/
    virtual bool isOpen() {
        return m_isOpen;
    }

    virtual bool send(char *data, size_t size) {
        zmq_msg_t zmsg;
        auto cleanup =
                [](void *data, void *hint) {
                    delete[] ((char *) data);
                };
        //zmq_msg_init_data(&zmsg, data, size, cleanup, NULL);
        zmq_msg_init_data(&zmsg, data, size, NULL, NULL);
        return zmq_msg_send(&zmsg, m_sock, 0) == size;
    }

    virtual bool send(Message *msg) {
//fix this method
        string *data = new string();
        if (!msg->SerializeToString(data))
            return false;
        zmq_msg_t zmsg;
        zmq_msg_init_size(&zmsg, data->size());
        memcpy(zmq_msg_data(&zmsg), data->data(), data->size());
        int send = zmq_msg_send(&zmsg, m_sock, 0);
        zmq_msg_close(&zmsg);
        delete data;
        return send > 0;
    }

    virtual int receive(char **data) {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        int recv = zmq_msg_recv(&zmsg, m_sock, 0);
        *data = new char[recv];
        memcpy(*data, zmq_msg_data(&zmsg), recv);
        zmq_msg_close(&zmsg);
        return recv;
    }

    virtual int receive(char *data, size_t size) {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        int recv = zmq_msg_recv(&zmsg, m_sock, 0);
        memcpy(data, zmq_msg_data(&zmsg), size * (recv == size));
        zmq_msg_close(&zmsg);
        return recv;
    }

    virtual int receive(Message *msg) {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        int recv = zmq_msg_recv(&zmsg, m_sock, 0);
        if (recv > 0) {
            char *data = (char *) zmq_msg_data(&zmsg);
            recv = msg->ParseFromArray(data, recv) ? recv : -1;
        }
        zmq_msg_close(&zmsg);
        return recv;
    }

    virtual bool close() {
        usleep(1000);
        zmq_close(m_sock);
        m_isOpen = false;
        return true;
    }
private:
    static void *m_ctx;

    string m_proto;
    string m_addr;
    int m_port;
    int m_sockType;
    bool m_isOpen;
    void *m_sock;

    string init() {
        m_sock = zmq_socket(m_ctx, m_sockType);
        int hwm = 0;
        zmq_setsockopt(m_sock, ZMQ_SNDHWM, &hwm, sizeof(hwm));
        zmq_setsockopt(m_sock, ZMQ_RCVHWM, &hwm, sizeof(hwm));
        if (m_sockType == ZMQ_SUB)
            zmq_setsockopt(m_sock, ZMQ_SUBSCRIBE, NULL, 0);
        return m_proto + "://" + m_addr + ":" + to_string(m_port);
    }
};

void *ZmqSocket::m_ctx = zmq_ctx_new();
}
#endif
