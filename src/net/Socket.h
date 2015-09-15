#ifndef SOCKET_H
#define SOCKET_H

#include <cerrno>
#include <cstring>
#include <string>
#include <unistd.h>
#include <zmq.h>

#include "serializer/Serializable.h"
#include "util/Logging.h"

using std::strerror;
using std::string;

namespace tupleware {
class Socket {
public:
    static void startup() {
        if (m_zctx == NULL)
            m_zctx = zmq_ctx_new();
    }

    static void shutdown() {
        if (m_zctx != NULL) {
            zmq_term(m_zctx);
            m_zctx = NULL;
        }
    }

    static void setMaxSockets(int maxSocks) {
        zmq_ctx_set(m_zctx, ZMQ_MAX_SOCKETS, maxSocks);
    }

    virtual ~Socket() {}

    bool bind() {
        init();
        bool success = zmq_bind(m_sock, m_connStr.c_str()) == 0;
        if (!success) {
            Logging::error(__FILE__, __LINE__, strerror(errno));
            Logging::error(__FILE__, __LINE__, m_connStr);
        }
        return success;
    }

    bool connect() {
        init();
        bool success = zmq_connect(m_sock, m_connStr.c_str()) == 0;
        if (!success) {
            Logging::error(__FILE__, __LINE__, strerror(errno));
            Logging::error(__FILE__, __LINE__, m_connStr);
        }
        return success;
    }

    bool close() {
        usleep(1000);
        bool success = zmq_close(m_sock);
        if (success)
            Logging::error(__FILE__, __LINE__, strerror(errno));
        return success;
    }

    virtual int send(Serializable *msg) = 0;
    virtual Serializable *receive() = 0;
protected:
    string m_connStr;
    int m_sockType;
    void *m_sock;

    Socket(string connStr, int sockType) {
        m_connStr = connStr;
        m_sockType = sockType;
        m_sock = NULL;
    }

    void init() {
        if (m_zctx == NULL)
            Logging::info("NULL ZMQ_CTX");
        m_sock = zmq_socket(m_zctx, m_sockType);
        int hwm = 0;
        zmq_setsockopt(m_sock, ZMQ_SNDHWM, &hwm, sizeof(hwm));
        zmq_setsockopt(m_sock, ZMQ_RCVHWM, &hwm, sizeof(hwm));
        if (m_sockType == ZMQ_SUB)
            zmq_setsockopt(m_sock, ZMQ_SUBSCRIBE, NULL, 0);
    }
private:
    //static void zmqContext()
    static void *m_zctx;
};

void *Socket::m_zctx = NULL;
}
#endif
