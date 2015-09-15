#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <hdfs.h>
#include <string>
#include <unordered_map>

#include "file/Block.h"
#include "file/File.h"
#include "file/HdfsFile.h"
#include "file/MemFile.h"
#include "file/Request.h"
#include "net/InprocSocket.h"
#include "util/Logging.h"
#include "util/Runnable.h"
#include "util/Thread.h"

using std::string;
using std::unordered_map;

/*******/
#include <fstream>
#include <iostream>
#include <valarray>
using namespace std;
#define ELTS 8192
#define ATTR 90
/*******/

namespace tupleware {
class FileManager : public Runnable {
public:
    FileManager(int id, string hdfsAddr, int hdfsPort) : Runnable() {
        //m_workerMsg = workerMsg;
        m_fmgrSock = new InprocSocket("fmgr", ZMQ_REP);
        m_files = new unordered_map<string,File *>();
        //m_fs = hdfsConnect(hdfsAddr.data(), hdfsPort);
    }

    ~FileManager() {}

    virtual void run() {
        m_fmgrSock->bind();

string fname = "TEST";
Logging::info("Loading data...");
vector<vector<valarray<float>>> D;
ifstream f(fname);
if (f.is_open()) {
    string line;
    char delim;
    int elt = ELTS;
    for (int i = 0; getline(f, line); i++) {
        if (elt >= ELTS) {
            D.push_back(vector<valarray<float>>(ELTS));
            elt = 0;
        }
        istringstream buff(line);
        valarray<float> d(ATTR + 1);
        for (int j = 0; j <= ATTR; j++)
            buff >> d[j] >> delim;
        D.back().push_back(d);
        elt++;
    }
    f.close();
}

File *file = new MemFile(fname);
for (int i = 0; i < D.size(); i++) {
    vector<valarray<float>> d = D[i];
    Block *blk = new Block(i, d);
    file->addBlock(blk);
}
(*m_files)[fname] = file;
cout << "Blocks=" << D.size() << endl;

        while (!m_kill) {
            Serializable *msg = m_fmgrSock->receive();
            if (msg == NULL) {
                Logging::error(__FILE__, __LINE__, "bad msg");
                continue;
            }

            Request *req = (Request *) msg;
            Request::Type reqType = req->type();
            string filename = req->filename();
            if (reqType == Request::Type::ADD) {
                File *ff = new MemFile(filename);
                (*m_files)[filename] = ff;
            }
            else if (reqType == Request::Type::FILE)
                req->setData((*m_files)[filename]);
            else
                Logging::error(__FILE__, __LINE__, "bad req");

            m_fmgrSock->send(req);

                //RequestMessage *reqMsg = new RequestMessage();
                //Block **blks = m_files[reqMsg->file_id()]->blocks();
                //int numBlks = reqMsg->blk_id_size();
                //for (int i = 0; i < numBlks; i++)
                //    if (blks[reqMsg->blk_id(i)]->data() == NULL);
                //        //add to bcast message
                //if size of bcast message > 0, send bcast message
                //m_reqSock->send(NULL, 0);
            //do the action
        }

        m_fmgrSock->close();
/*
        while (!m_kill) {

//initHdfs (or something) called by TC
        //if (m_streams.find(filename) == m_streams.end())

        //hdfsFile stream = m_streams[hdfsMsg.filename()];
//        int pos = hdfsMsg.start();
//        for (int i = 0; i < blks.size(); i++) {
//            int size = blks[i]->size();
            //hdfsPread(m_fs, stream, pos, blks[i]->data(), size);
//            pos += size;
//        }

//        while (hdfsFileIsOpenForRead(stream) && start >= blks.size()) {}

            while (!m_kill) {
                char *raw;
                int recv = m_blkSock->receive(&raw);
                if (recv < 1) {
                    Logging::error(__FILE__, __LINE__, "error recv blk");
                    continue;
                }
//There is a memory leak here...the header never gets freed
//Look into zmq multipart messages
                uint64_t fileId = *((uint64_t *) (raw));
                //uint32_t fileId = *((uint32_t *) raw);
                //uint32_t wrkrId = *((uint32_t *) raw);
                uint64_t blkId = *((uint64_t *) (raw + sizeof(fileId)));
                char *data = (char *) (raw + sizeof(fileId) + sizeof(blkId));
                size_t size = recv - sizeof(fileId) - sizeof(blkId);
                m_lock->lock();
                if (m_files->find(fileId) == m_files->end())
                    (*m_files)[fileId] = new TempFile();
                (*m_files)[fileId]->addBlock(new Block(blkId, data, size));
                m_lock->unlock();
                Logging::info("Received Block(" + to_string(fileId) + ","
                        + to_string(blkId) + ")");
        Socket *m_blkSock;
        mutex *m_lock;*/
    }
private:
    InprocSocket *m_fmgrSock;
    //WorkerMessage *m_workerMsg;
    hdfsFS m_fs;
    unordered_map<string,File *> *m_files;
};
}
#endif
