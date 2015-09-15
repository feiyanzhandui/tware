#ifndef SOCKET_H
#define SOCKET_H

class Socket {
public:
    virtual void bind() = 0;
    virtual void connect() = 0;
    virtual void send(char *data, size_t size) = 0;
    virtual void sendOrders(int *o_orderkey, long *o_orderdate,
            char *o_orderpriority, size_t size) = 0;
    virtual void sendLineitem(int *l_orderkey, long *l_commitdate,
            long *l_receiptdate, size_t size) = 0;
    virtual int receive(char **data) = 0;
    virtual int receiveOrders(int **o_orderkey, long **o_orderdate,
            char **o_orderpriority) = 0;
    virtual int receiveLineitem(int **l_orderkey, long **l_commitdate,
            long **l_receiptdate) = 0;
    virtual void close() = 0;
};
#endif
