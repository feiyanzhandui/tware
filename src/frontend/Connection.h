#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>

#include "job/Job.h"
#include "net/InprocSocket.h"
#include "net/Socket.h"
#include "util/Defaults.h"

using std::string;

#include <iostream>
using namespace std;

namespace tupleware {
class TSet;

class Connection {
friend class TSet;
public:
    static Connection *getConnection(string addr, int port) {
        Connection *conn = new Connection(addr, port);
        conn->m_sock->connect();
        //connMsg->set_id(conn->m_id);
        //conn->m_connSock->send(connMsg);
        //conn->m_connSock->receive(connMsg);
        //conn->m_id = connMsg->id();
        //delete connMsg;
        return conn;
    }

    string address() {
        return m_addr;
    }

    int port() {
        return m_port;
    }
private:
    string m_addr;
    int m_port;
    Socket *m_sock;

    Connection(string addr, int port) {
        m_addr = addr;
        m_port = port;
        m_sock = new InprocSocket("client", ZMQ_REQ);//(m_addr, m_port, ZMQ_REQ);
    }
};
}
#endif
