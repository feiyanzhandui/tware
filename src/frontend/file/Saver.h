#ifndef LOADER_H
#define LOADER_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "job/Task.h"
#include "message/WorkerMessage.pb.h"
#include "net/Socket.h"
#include "net/TcpSocket.h"
#include "util/Defaults.h"
#include "util/Logging.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

namespace tupleware {
class Loader : public Task {
public:
    Loader(Socket *lreqSock, string filename, char delim, uint64_t fileId,
            vector<WorkerMessage *> *workers) : Task(lreqSock) {
        m_filename = filename;
        m_delim = delim;
        m_fileId = fileId;
        m_workers = workers;
    }

    virtual void run() {
        Logging::info("Loading file " + m_filename + "...");

        int port = Defaults::BLOCK_PORT;
        vector<Socket *> socks;
        for (int i = 0; i < m_workers->size(); i++) {
            string addr = (*m_workers)[i]->addr();
            Socket *sock = new TcpSocket(addr, port, ZMQ_PUSH);
            sock->connect();
            socks.push_back(sock);
        }

        ifstream f(m_filename);
        if (f.is_open()) {
            int size = 32768;
            char buff[size];
            f.getline(buff, size);

            int numAttr = 1;
            char *pos = strchr(buff, m_delim);
            while (pos != NULL) {
                ++numAttr;
                pos = strchr(++pos, m_delim);
            }

            size_t headSize = 2 * sizeof(uint64_t);
            size_t dataSize = size * numAttr * sizeof(float);
            size_t rawSize = headSize + dataSize;
            char *raw = new char[rawSize];
            float *data = (float *) (raw + headSize);

            for (uint64_t i = 0; f.good(); i++) {
                //for (int j = 0; j < size; j++) {
                    pos = strchr(buff, m_delim);
                    while (pos != NULL) {
                        *pos = '\0';
                        pos = strchr(++pos, m_delim);
                    }

                    int id = i % size;
                    int dataOff = id * numAttr;
                    pos = buff;
                    for (int k = 0; k < numAttr; k++) {
                        data[dataOff + k] = strtof(pos, &pos);
                        pos++;
                    }
                //}

                if (id == size - 1) {
                    Logging::info("Loaded Block(" + to_string(m_fileId) + ","
                            + to_string(i) + ")");
                    *((uint64_t *) (raw)) = m_fileId;
                    *((uint64_t *) (raw + sizeof(uint64_t))) = i;
                socks[i % socks.size()]->send(raw, rawSize);
                    raw = new char[rawSize];
                    data = (float *) (raw + headSize);
                }

                f.getline(buff, size);
            }
            f.close();
        }

        Logging::info("File " + m_filename + " loaded.");
    }
private:
    string m_filename;
    char m_delim;
    uint64_t m_fileId;
    vector<WorkerMessage *> *m_workers;
};
}
#endif
