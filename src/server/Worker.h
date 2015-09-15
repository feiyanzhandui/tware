#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <string>

#include "file/FileManager.h"
#include "job/Task.h"
#include "job/TaskController.h"
#include "net/InprocSocket.h"
#include "net/Socket.h"
#include "net/TcpSocket.h"
#include "util/Config.h"
#include "util/Defaults.h"
#include "util/Logging.h"
#include "util/Runnable.h"
#include "util/Thread.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

namespace tupleware {
class Worker : public Runnable {
public:
    Worker(string addr, string myAddr) {
        //Socket::setMaxSockets(16384);

        //m_workerMsg = new WorkerMessage();
        //m_workerMsg->set_id(-1);
        //m_workerMsg->set_addr(myAddr);
        //m_workerMsg->set_cores(sysconf(_SC_NPROCESSORS_ONLN));

        string hdfsAddr = "";
        int hdfsPort = 0;
        m_fmgr = new FileManager(0, hdfsAddr, hdfsPort);
        m_bcastd = new BroadcastDaemon(4001);
    }

    ~Worker() {}

    virtual void run() {
        //Socket::startup();
        //Socket::setMaxSockets(16384);
        //welcome message
        //int x = system("clear");
        //cout << "***** Welcome to Tupleware! *****\n"
        //     << "Starting up Worker...\n" << endl;

        //m_workerSock->connect();
        //if (!m_workerSock->send(m_workerMsg)) {
        //    Logging::fatal(__FILE__, __LINE__, "cannot connect to Master");
        //    return;
        //}

        //if (!m_workerSock->receive(m_workerMsg)) {
        //    Logging::error(__FILE__, __LINE__, "bad WorkerMsg");
        //    return;
        //}

        //int id = m_workerMsg->id();
        //Logging::info("Connected to master (id=" + to_string(id) + ")");

        Thread *fmt = new Thread(m_fmgr);
        fmt->start();

        Thread *bdt = new Thread(m_bcastd);
        bdt->start();

        bdt->join();
        fmt->join();
    }
private:
//        ServerDaemon(int port) : Runnable() {
//            m_sock = new InprocSocket("server", ZMQ_REQ);//("*", serverPort, ZMQ_REP);
//        }

    class BroadcastDaemon : public Runnable {
    public:
        BroadcastDaemon(int port) : Runnable() {
            //m_inSock = new TcpSocket("localhost", port, ZMQ_PULL);
            m_inSock = new InprocSocket("bcast2", ZMQ_PULL);
        }

        virtual void run() {
            m_inSock->connect();
            while (!m_kill) {
                Serializable *msg = m_inSock->receive();
                if (msg == NULL) {
                    Logging::error(__FILE__, __LINE__, "bad msg");
                    continue;
                }

                Task *task = (Task *) msg;
                TaskController *tc = new TaskController(task);
                Thread *tct = new Thread(tc);
                tct->start();
            }
            m_inSock->close();
        }
    private:
        //TcpSocket *m_inSock;
        InprocSocket *m_inSock;
    };

    //WorkerMessage *m_workerMsg;
    FileManager *m_fmgr;
    BroadcastDaemon *m_bcastd;
};
}
#endif
