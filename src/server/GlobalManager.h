#ifndef GLOBAL_MANAGER_H
#define GLOBAL_MANAGER_H

#include <hdfs.h>
#include <string>
#include <unordered_map>

#include "file/File.h"
#include "net/InprocSocket.h"
#include "net/Socket.h"
#include "serializer/Serializable.h"
#include "util/Logging.h"
#include "util/Runnable.h"

using std::string;
using std::unordered_map;

namespace tupleware {
class GlobalManager : public Runnable {
public:
    GlobalManager(string hdfsAddr, int hdfsPort) : Runnable() {
        m_gmgrSock = new InprocSocket("gmgr", ZMQ_PULL);
        m_bcastSock = new InprocSocket("bcast", ZMQ_PUSH);
        //m_fs = hdfsConnect(hdfsAddr.c_str(), hdfsPort);
    }

    ~GlobalManager() {
        delete m_gmgrSock;
        delete m_bcastSock;
    }

    virtual void run() {
        m_gmgrSock->bind();
        m_bcastSock->connect();

        while (!m_kill) {
            Serializable *msg = m_gmgrSock->receive();
            if (msg == NULL) {
                Logging::error(__FILE__, __LINE__, "bad msg");
                continue;
            }

            //do the action

            m_gmgrSock->send(NULL);
        }

        m_bcastSock->close();
        m_gmgrSock->close();
    }
private:
    Socket *m_gmgrSock;
    Socket *m_bcastSock;
    hdfsFS m_fs;
    unordered_map<string,File *> m_files;
};
}
#endif
