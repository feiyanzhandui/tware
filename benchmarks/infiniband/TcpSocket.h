#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <cstring>
#include <string>
#include <unistd.h>

#include <zmq.h>

#include "JoinUtil.h"
#include "Socket.h"

using std::memcpy;
using std::string;
using std::to_string;

class TcpSocket : public Socket {
public:
    static void setMaxSockets(int maxSocks) {
        zmq_ctx_set(m_ctx, ZMQ_MAX_SOCKETS, maxSocks);
    }

    static void terminate() {
        zmq_term(m_ctx);
    }

    TcpSocket(string addr, int port, int sockType) {
        m_addr = addr;
        m_port = port;
        m_sockType = sockType;
    }

    virtual void bind() {
        zmq_bind(m_sock, init().c_str());
    }

    virtual void connect() {
        zmq_connect(m_sock, init().c_str());
    }

    virtual void send(char *data, size_t size) {
        zmq_msg_t zmsg;
        zmq_msg_init_data(&zmsg, data, size, NULL, NULL);
        zmq_msg_send(&zmsg, m_sock, 0);
    }

    virtual void sendOrders(int *o_orderkey, long *o_orderdate,
            char *o_orderpriority, size_t size) {
        //o_orderkey
        size_t size1 = size * O_ORDERKEY;
        zmq_msg_t zmsg1;
        zmq_msg_init_data(&zmsg1, (char *) o_orderkey, size1,
                [](void *data, void *hint) {
                    delete[] ((int *) data);
                }, NULL);
        zmq_msg_send(&zmsg1, m_sock, ZMQ_SNDMORE);

        //o_orderdate
        size_t size2 = size * O_ORDERDATE;
        zmq_msg_t zmsg2;
        zmq_msg_init_data(&zmsg2, (char *) o_orderdate, size2,
                [](void *data, void *hint) {
                    delete[] ((long *) data);
                }, NULL);
        zmq_msg_send(&zmsg2, m_sock, ZMQ_SNDMORE);

        //o_orderpriority
        size_t size3 = size * O_ORDERPRIORITY;
        zmq_msg_t zmsg3;
        zmq_msg_init_data(&zmsg3, o_orderpriority, size3,
                [](void *data, void *hint) {
                    delete[] ((char *) data);
                }, NULL);
        zmq_msg_send(&zmsg3, m_sock, 0);
    }

    virtual void sendLineitem(int *l_orderkey, long *l_commitdate,
            long *l_receiptdate, size_t size) {
        //l_orderkey
        size_t size1 = size * L_ORDERKEY;
        zmq_msg_t zmsg1;
        zmq_msg_init_data(&zmsg1, (char *) l_orderkey, size1,
                [](void *data, void *hint) {
                    delete[] ((int *) data);
                }, NULL);
        zmq_msg_send(&zmsg1, m_sock, ZMQ_SNDMORE);

        //l_commitdate
        size_t size2 = size * L_COMMITDATE;
        zmq_msg_t zmsg2;
        zmq_msg_init_data(&zmsg2, (char *) l_commitdate, size2,
                [](void *data, void *hint) {
                    delete[] ((long *) data);
                }, NULL);
        zmq_msg_send(&zmsg2, m_sock, ZMQ_SNDMORE);

        //l_receiptdate
        size_t size3 = size * L_RECEIPTDATE;
        zmq_msg_t zmsg3;
        zmq_msg_init_data(&zmsg3, (char *) l_receiptdate, size3,
                [](void *data, void *hint) {
                    delete[] ((long *) data);
                }, NULL);
        if (size3 != 2*size1) { cout << "size3=" << size3 << endl;}
        if (zmq_msg_send(&zmsg3, m_sock, 0) == -1) cout << "ERROR SEND" << endl;
    }

    virtual int receive(char **data) {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        int recv = zmq_msg_recv(&zmsg, m_sock, 0);
        if (recv > 0) {
            *data = new char[recv];
            memcpy(*data, zmq_msg_data(&zmsg), recv);
        }
        zmq_msg_close(&zmsg);
        return recv;
    }

    virtual int receiveOrders(int **o_orderkey, long **o_orderdate,
            char **o_orderpriority) {
        //o_orderkey
        zmq_msg_t zmsg1;
        zmq_msg_init(&zmsg1);
        int recv1 = zmq_msg_recv(&zmsg1, m_sock, 0);

        //o_orderdate
        zmq_msg_t zmsg2;
        zmq_msg_init(&zmsg2);
        int recv2 = zmq_msg_recv(&zmsg2, m_sock, 0);

        //o_orderpriority
        zmq_msg_t zmsg3;
        zmq_msg_init(&zmsg3);
        int recv3 = zmq_msg_recv(&zmsg3, m_sock, 0);

        //copy
        if (recv1 > 0) {
            *o_orderkey = new int[recv1 / O_ORDERKEY];
            memcpy(*o_orderkey, zmq_msg_data(&zmsg1), recv1);

            *o_orderdate = new long[recv2 / O_ORDERDATE];
            memcpy(*o_orderdate, zmq_msg_data(&zmsg2), recv2);

            *o_orderpriority = new char[recv3 / O_ORDERPRIORITY];
            memcpy(*o_orderpriority, zmq_msg_data(&zmsg3), recv3);
        }

        //close
        zmq_msg_close(&zmsg1);
        zmq_msg_close(&zmsg2);
        zmq_msg_close(&zmsg3);

        return recv1 / O_ORDERKEY;
    }

    virtual int receiveLineitem(int **l_orderkey, long **l_commitdate,
            long **l_receiptdate) {
        //l_orderkey
        zmq_msg_t zmsg1;
        zmq_msg_init(&zmsg1);
        int recv1 = zmq_msg_recv(&zmsg1, m_sock, 0);

        //l_commitdate
        zmq_msg_t zmsg2;
        zmq_msg_init(&zmsg2);
        int recv2 = zmq_msg_recv(&zmsg2, m_sock, 0);

        //l_receiptdate
        zmq_msg_t zmsg3;
        zmq_msg_init(&zmsg3);
        int recv3 = zmq_msg_recv(&zmsg3, m_sock, 0);
//if (errno != 0) cout << errno << ": " << strerror(errno) << endl;
if (recv2 != 2*recv1) { cout << errno << ": " << strerror(errno) << endl; cout << "recv1=" << recv1 << ",recv2=" << recv2 << ",recv3=" << recv3 << endl; }
if (recv3 != 2*recv1) { cout << errno << ": " << strerror(errno) << endl; cout << "recv1=" << recv1 << ",recv2=" << recv2 << ",recv3=" << recv3 << endl; }
        //copy
        if (recv1 > 0) {
            *l_orderkey = new int[recv1 / L_ORDERKEY];
            memcpy(*l_orderkey, zmq_msg_data(&zmsg1), recv1);

            *l_commitdate = new long[recv2 / L_COMMITDATE];
            memcpy(*l_commitdate, zmq_msg_data(&zmsg2), recv2);

            *l_receiptdate = new long[recv3 / L_RECEIPTDATE];
            memcpy(*l_receiptdate, zmq_msg_data(&zmsg3), recv3);
        }

        //close
        zmq_msg_close(&zmsg1);
        zmq_msg_close(&zmsg2);
        zmq_msg_close(&zmsg3);

        return recv1 / L_ORDERKEY;
    }

    virtual void close() {
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
        //int hwm = 200000;
        int hwm = 100;
        zmq_setsockopt(m_sock, ZMQ_SNDHWM, &hwm, sizeof(hwm));
        zmq_setsockopt(m_sock, ZMQ_RCVHWM, &hwm, sizeof(hwm));
        if (m_sockType == ZMQ_SUB)
            zmq_setsockopt(m_sock, ZMQ_SUBSCRIBE, NULL, 0);
        return "tcp://" + m_addr + ":" + to_string(m_port);
    }
};

void *TcpSocket::m_ctx = zmq_ctx_new();
#endif
