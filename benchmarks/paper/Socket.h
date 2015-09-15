#ifndef SOCKET_H
#define SOCKET_H

#include <cstring>
#include <string>
#include <unistd.h>

#include <zmq.h>

using std::memcpy;
using std::string;
using std::to_string;

class Socket {
public:
    static void setMaxSockets(int maxSocks) {
        zmq_ctx_set(m_ctx, ZMQ_MAX_SOCKETS, maxSocks);
    }

    Socket(string addr, int port, int sockType) {
        m_addr = addr;
        m_port = port;
        m_sockType = sockType;
    }

    void bind() {
        zmq_bind(m_sock, init().c_str());
    }

    void connect() {
        zmq_connect(m_sock, init().c_str());
    }

    void send(char *data, size_t size) {
        zmq_msg_t zmsg;
        zmq_msg_init_data(&zmsg, data, size, NULL, NULL);
        zmq_msg_send(&zmsg, m_sock, 0);
    }

    int receive(char *data) {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        int recv = zmq_msg_recv(&zmsg, m_sock, 0);
        memcpy(data, zmq_msg_data(&zmsg), recv);
        zmq_msg_close(&zmsg);
        return recv;
    }

    void close() {
        usleep(1000);
        zmq_close(m_sock);
    }
private:
    static void *m_ctx;

    string m_addr;
    int m_port;
    int m_sockType;
    void *m_sock;

    string init() {
        m_sock = zmq_socket(m_ctx, m_sockType);
        int hwm = 0;
        zmq_setsockopt(m_sock, ZMQ_SNDHWM, &hwm, sizeof(hwm));
        zmq_setsockopt(m_sock, ZMQ_RCVHWM, &hwm, sizeof(hwm));
        if (m_sockType == ZMQ_SUB)
            zmq_setsockopt(m_sock, ZMQ_SUBSCRIBE, NULL, 0);
        return "tcp://" + m_addr + ":" + to_string(m_port);
    }
};

void *Socket::m_ctx = zmq_ctx_new();
#endif
