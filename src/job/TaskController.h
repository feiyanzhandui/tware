#ifndef TASK_CONTROLLER_H
#define TASK_CONTROLLER_H

#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "compiler/Compiler.h"
#include "compiler/Executable.h"
#include "file/Block.h"
#include "file/File.h"
#include "file/Request.h"
#include "job/Result.h"
#include "job/Task.h"
#include "net/InprocSocket.h"
#include "net/TcpSocket.h"
#include "serializer/Serializable.h"
#include "util/Logging.h"
#include "util/Runnable.h"
#include "util/Thread.h"
#include "util/Timer.h"

using std::string;
using std::type_info;
using std::unordered_map;
using std::vector;

namespace tupleware {
class TaskController : public Runnable {
public:
    TaskController(Task *task) : Runnable() {
        m_task = task;
        m_taskSock = new InprocSocket("task", ZMQ_PULL);
        m_taskExecSock = new InprocSocket("task_exec", ZMQ_REP);
        m_fmgrSock = new InprocSocket("fmgr", ZMQ_REQ);
        m_resSock = new InprocSocket("server", ZMQ_REQ);//TcpSocket("localhost", 4000, ZMQ_REQ);
        //vector<uint64_t> files;
        //files.push_back(taskMsg->file_id());
        //m_reqd = new RequestDaemon(m_taskMsg, files, reqSock, bcastSock);
        //m_resSock = resSock;
        //    for (int i = 0; i < files.size(); i++) {
        //        uint64_t fileId = files[i];f
        //        *((uint64_t *) buff) = fileId;
        //        lmgrSock->send(buff, buffSize);
        //        lmgrSock->receive(buff, buffSize);
            //for (int j = 0; j < m_taskMsg->iter(); j++) {
            //    m_files[fileId + j] = *((File **) buff);
            //    m_avail[fileId + j] = 0;
            //}*/
    }

    ~TaskController() {
        delete m_task;
        delete m_taskSock;
        delete m_fmgrSock;
        delete m_resSock;
    }

    virtual void run() {
        m_taskSock->bind();
        m_taskExecSock->bind();
        m_fmgrSock->connect();
        m_resSock->connect();

        Logging::info("Fetching data...");
        for (string filename : m_task->filenames()) {
            Request *req = new Request(Request::Type::FILE, filename, 0);
            m_fmgrSock->send(req);
            Request *rep = (Request *) m_fmgrSock->receive();
            m_files[filename] = (File *) rep->data();
            delete req;
            //delete rep;
        }

        Logging::info("Compiling task executable...");
        Executable *exec = Compiler::compile(m_task->llvm());

        Logging::info("Launching task executors...");
Context *ctx = m_task->context();
        Timer *t = new Timer();
        t->start();
        int numThreads = m_task->numberOfThreads();
        Thread **taskExecs = new Thread*[numThreads];
        for (int i = 0; i < numThreads; i++) {
            InprocSocket *sock = new InprocSocket("task_exec", ZMQ_REQ);
            sock->connect();
            taskExecs[i] = new Thread(exec->getRunnable(ctx, sock));
            taskExecs[i]->start();
        }

        for (int i = 0; i < numThreads;) {
            Serializable *msg = m_taskExecSock->receive();
            if (msg == NULL) {
                Logging::error(__FILE__, __LINE__, "bad msg");
                continue;
            }

            const type_info *type = msg->type();
            if (*type == typeid(Request)) {
                Request *req = (Request *) msg;
                Request::Type reqType = req->type();
                string filename = req->filename();
                if (reqType == Request::Type::ADD) {
                    Block *blk = (Block *) req->data();
                    File *ff = new MemFile(filename);
                    m_files[filename] = ff;
                    m_files[filename]->addBlock(blk);
                }
                else if (reqType == Request::Type::ANY) {
                    vector<Block *> *blks = m_files[filename]->blocks();
                    size_t blkNum = m_avail[filename]++;
                    if (blkNum < blks->size())
                        req->setData((*blks)[blkNum]);
                }
                else if (reqType == Request::Type::BLOCK) {
                    Logging::error(__FILE__, __LINE__, "not implemented");
                }
                else if (reqType == Request::Type::FILE)
                    Logging::error(__FILE__, __LINE__, "not implemented");
                else
                    Logging::error(__FILE__, __LINE__, "bad req");

                m_taskExecSock->send(req);

                //RequestMessage *reqMsg = new RequestMessage();
                //Block **blks = m_files[reqMsg->file_id()]->blocks();
                //int numBlks = reqMsg->blk_id_size();
                //for (int i = 0; i < numBlks; i++)
                //    if (blks[reqMsg->blk_id(i)]->data() == NULL);
                //        //add to bcast message
                //if size of bcast message > 0, send bcast message
                //m_reqSock->send(NULL, 0);
            }
            else if (*type == typeid(Result)) {
                Logging::info("result#" + to_string(i));
                m_taskExecSock->send(msg);
                i++;
            }
            else
                Logging::error(__FILE__, __LINE__, "unknown msg");
        }

        for (int i = 0; i < numThreads; i++)
            delete taskExecs[i];
        delete[] taskExecs;
        t->stop();

        Logging::info("Task complete.");
        Logging::info("Executed in " + to_string(t->time()) + "s");
        delete t;
    }
private:
    Task *m_task;
    InprocSocket *m_taskSock;
    InprocSocket *m_taskExecSock;
    InprocSocket *m_fmgrSock;
    InprocSocket *m_resSock;
    //tcpSocket
    unordered_map<string,File *> m_files;
    unordered_map<string,size_t> m_avail;
};
}
#endif
