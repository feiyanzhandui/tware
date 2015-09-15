#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <typeinfo>
#include <vector>

#include "frontend/Context.h"
#include "file/Block.h"
#include "file/File.h"
#include "file/Request.h"
#include "job/Result.h"
#include "net/InprocSocket.h"
#include "serializer/Serializable.h"
#include "util/Runnable.h"

using std::string;
using std::type_info;
using std::vector;

namespace tupleware {
class Executor : public Runnable {
public:
    ~Executor() {
        delete m_ctx;
        delete m_sock;
    }
protected:
    Context *m_ctx;
    InprocSocket *m_sock;

    Executor(Context *ctx, InprocSocket *sock) : Runnable() {
        m_ctx = ctx;
        m_sock = sock;
    }

    Block *getAny(string filename) {
        Request *req = new Request(Request::Type::ANY, filename, 0);
        m_sock->send(req);
        Serializable *msg = m_sock->receive();
        Request *rep = (Request *) msg;
        Block *blk = (Block *) rep->data();
        delete req;
        return blk;
    }

    Block *getBlock(string filename, uint64_t blkId) {
        Request *req = new Request(Request::Type::BLOCK, filename, blkId);
        m_sock->send(req);
        Serializable *msg = m_sock->receive();
        Request *rep = (Request *) msg;
        Block *blk = (Block *) rep->data();
        delete req;
        return blk;
    }

    File *getFile(string filename) {
        Request *req = new Request(Request::Type::FILE, filename, 0);
        m_sock->send(req);
        Serializable *msg = m_sock->receive();
        Request *rep = (Request *) msg;
        File *file = (File *) rep->data();
        delete req;
        return file;
    }

    void addBlock(string filename, Block *blk) {
        Request *req = new Request(Request::Type::ADD, filename, 0);
        req->setData(blk);
        m_sock->send(req);
        Serializable *msg = m_sock->receive();
        delete req;
    }

    void sendResult() {
        Result *res = new Result();
        m_sock->send(res);
        Serializable *msg = m_sock->receive();
    }
};
}
#endif
