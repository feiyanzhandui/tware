#ifndef MASTER_H
#define MASTER_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "file/FileManager.h"
#include "job/Job.h"
#include "job/JobController.h"
//#include "message/ConnectionMessage.pb.h"
//#include "message/JobMessage.pb.h"
//#include "message/ManagementMessage.pb.h"
//#include "message/MasterMessage.pb.h"
//#include "message/TMessage.pb.h"
//#include "message/WorkerMessage.pb.h"
#include "net/InprocSocket.h"
#include "net/Socket.h"
#include "net/TcpSocket.h"
#include "serializer/Serializable.h"
//#include "scheduler/BasicScheduler.h"
//#include "scheduler/LocalityScheduler.h"
//#include "scheduler/MultitenancyScheduler.h"
//#include "scheduler/Scheduler.h"
//#include "server/GlobalManager.h"
#include "util/Config.h"
#include "util/Defaults.h"
#include "util/Logging.h"
#include "util/Runnable.h"
#include "util/Thread.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::vector;

#include "frontend/Connection.h"
#include "frontend/Context.h"
#include "frontend/TSet.h"
#include "server/Worker.h"

#include <valarray>
using std::valarray;

namespace tupleware {
class Master : public Runnable {
public:
    Master(int serverPort, int clientPort) {
        string hdfsAddr = "";
        int hdfsPort = 0;

        m_fmgr = new FileManager(0, hdfsAddr, hdfsPort);
        //m_gmgr = new GlobalManager("", 0);
        m_serverd = new ServerDaemon(serverPort);
        m_clientd = new ClientDaemon(clientPort);
        m_bcastd = new BroadcastDaemon(4001);
        /*
        m_mgmtSock = Socket::inproc("mgmtd", ZMQ_PULL);
        m_workers = new vector<WorkerMessage *>();

        Socket *gcatSock = Socket::inproc("gcat", ZMQ_REP);
        Socket *bcastSock = Socket::inproc("bcast", ZMQ_PULL);
        Socket *lcatSock = Socket::inproc("lcat", ZMQ_PUB);
        m_gmgr = new GlobalManager(gcatSock, bcastSock, lcatSock, "", 0);*/
    }

    ~Master() {
        delete m_fmgr;
        delete m_serverd;
        delete m_clientd;
        delete m_bcastd;
    }

    virtual void run() {
        Socket::startup();
        Socket::setMaxSockets(16384);

        Thread *fmt = new Thread(m_fmgr);
        Thread *bdt = new Thread(m_bcastd);
        Thread *sdt = new Thread(m_serverd);
        Thread *cdt = new Thread(m_clientd);

        //fmt->start();
        bdt->start();
        sdt->start();
        cdt->start();

        Runnable *worker = new Worker("", "");
        Thread *wt = new Thread(worker);
        wt->start();

        Logging::info("Master ready for jobs.");

        usleep(100000);

Connection *conn = Connection::getConnection("localhost", 4000);
TSet *t = new TSet(conn, NULL);

valarray<float> w(90);
w = 0.5f;
Context *ctx = t->context();
ctx->set("w", w);
valarray<float> v = ctx->get<valarray<float>>("w");
t = t->map("valarray<float> f1(valarray<float> &t, Context *c) {\n"
           "  valarray<float> w = c->get<valarray<float>>(\"w\");\n"
           "  valarray<float> attrs = t[slice(0,90,1)];\n"
           "  float dot = (attrs * w).sum();\n"
           "  float label = t[90];\n"
           "  float s = (1.0f / (1.0f + exp(-label * dot)) - 1.0f) * label;\n"
           "  valarray<float> g = s * attrs;\n"
           "  return g;\n"
           "}\n")
     ->reduce("valarray<float> f2(valarray<float> &t1, valarray<float> &t2) {\n"
              "  return t1 + t2;\n"
              "}\n")
     //->loop("bool f3(Context *c) { return true; }\n")
     ->evaluate();

//Udf *f1 = Udf::getUdf("int f1(string t) {return atoi(t.c_str());}");
//Udf *f2 = Udf::getUdf("tuple<int,int> f2(int t) {return make_tuple(t, 1);}");
//Udf *f3 = Udf::getUdf("tuple<int,int> f3(tuple<int,int> t1, tuple<int,int> t2) {int k = get<0>(t1); int v1 = get<1>(t1); int v2 = get<1>(t2); return make_tuple(k, v1 + v2);}");
//Udf *f4 = Udf::getUdf("int f4(tuple<int,int> t) {return get<0>(t);}");

//        LoadOp *o0 = new LoadOp("test.txt");
//        MapOp *o1 = new MapOp(o0, f1);
//        Op *o2 = new MapOp(o1, f2);
//        Op *o3 = new ReduceOp(o2, f3, f4);
//        Job *job = new Job(o3);

        cdt->join();
        sdt->join();
        bdt->join();
        //fmt->join();
/*

        Thread *gmt = new Thread(m_gmgr);
        gmt->start();
        TMessage *msg = new TMessage();
        while (true) {
            if (!m_mgmtSock->receive(msg)) {
                Logging::error(__FILE__, __LINE__, "bad msg");
                continue;
            }
        }

        Socket::terminate();

        wdt->stop();
        cdt->stop();
        jdt->stop();

        wdt->join();
        cdt->join();
        jdt->join();

        delete wdt;
        delete cdt;
        delete jdt;

        gmt->stop();
        gmt->join();
        delete gmt;*/
        Socket::shutdown();

        delete cdt;
        delete sdt;
        delete bdt;
        delete fmt;
    }
private:
    class BroadcastDaemon : public Runnable {
    public:
        BroadcastDaemon(int port) : Runnable() {
            m_inSock = new InprocSocket("bcast", ZMQ_PULL);
            //m_outSock = new TcpSocket("*", port, ZMQ_PUSH);
            m_outSock = new InprocSocket("bcast2", ZMQ_PUSH);
        }

        ~BroadcastDaemon() {
            delete m_inSock;
            delete m_outSock;
        }

        virtual void run() {
            m_outSock->bind();
            m_inSock->bind();
            while (!m_kill) {
                Serializable *msg = m_inSock->receive();
                if (msg == NULL) {
                    Logging::error(__FILE__, __LINE__, "bad msg");
                    continue;
                }
                m_outSock->send(msg);
            }
            m_inSock->close();
            m_outSock->close();
        }
    private:
        InprocSocket *m_inSock;
        //TcpSocket *m_outSock;
        InprocSocket *m_outSock;
    };

    class ServerDaemon : public Runnable {
    public:
        ServerDaemon(int port) : Runnable() {
            m_sock = new TcpSocket("*", port, ZMQ_REP);
        }

        ~ServerDaemon() {
            delete m_sock;
        }

        virtual void run() {
            m_sock->bind();
            while (!m_kill) {
                Serializable *msg = m_sock->receive();
                if (msg == NULL) {
                    Logging::error(__FILE__, __LINE__, "bad msg");
                Result *res = new Result();
                m_sock->send(res);
                    continue;
                }
                Result *res = new Result();
                m_sock->send(res);
            }
            m_sock->close();
        }
    private:
        TcpSocket *m_sock;
        //unordered_map<string,FileInfo *> m_cat;
    };

    class ClientDaemon : public Runnable {
    public:
        ClientDaemon(int port) : Runnable() {
            m_sock = new InprocSocket("client", ZMQ_REP);//TcpSocket("*", port, ZMQ_REP);
        }

        ~ClientDaemon() {
            delete m_sock;
        }

        virtual void run() {
            m_sock->bind();
            vector<Thread *> jcts;
            uint64_t jobId = 0;
            while (!m_kill) {
                Serializable *msg = m_sock->receive();
                if (msg == NULL) {
                    Logging::error(__FILE__, __LINE__, "bad msg");
                Result *res = new Result();
                m_sock->send(res);
                    continue;
                }

                Logging::info("New job (jobId=" + to_string(jobId) + ")");
                Job *job = (Job *) msg;
                JobController *jc = new JobController(jobId++, job);
                Thread *jct = new Thread(jc);
                jct->start();
                jcts.push_back(jct);

                Result *res = new Result();
                m_sock->send(res);
                //uint64_t jobId = nextJobId++;
                //Logging::info("New job (id=" + to_string(jobId) + ")");
                //JobController *jctlr = new JobController(0, jobMsg, NULL,
                //                                   m_workers, m_taskSock,
                //                                   m_resSock, m_clientSock);
                //TaskMessage *tmsg = (TaskMessage *) msg;
                //Logging::info(tmsg->llvm());
                //m_sock->send(tmsg);
            }
            //delete all jcts
            m_sock->close();
        }
    private:
        InprocSocket *m_sock;
    };

    FileManager *m_fmgr;
    BroadcastDaemon *m_bcastd;
    ServerDaemon *m_serverd;
    ClientDaemon *m_clientd;
    //vector<WorkerMessage *> *m_workers;
};
}
#endif
