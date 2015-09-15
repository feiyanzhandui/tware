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
#include "net/ZmqSocket.h"
#include "util/Defaults.h"
#include "util/Logging.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

#include "util/Config.h"

namespace tupleware {
class Loader : public Task {
public:
    Loader(Socket *lreqSock, string filename, char delim, uint64_t fileId,
            int numAttr, vector<WorkerMessage *> *workers) : Task(lreqSock) {
        m_filename = filename;
        m_delim = delim;
        m_fileId = fileId;
        m_numAttr = numAttr;
        m_workers = workers;
    }

    virtual void run() {
        Logging::info("Loading file " + m_filename + "...");

        int port = Defaults::BLOCK_PORT;
        vector<Socket *> socks;
        for (int i = 0; i < m_workers->size(); i++) {
            string addr = (*m_workers)[i]->addr();
            Socket *sock = new ZmqSocket(addr, port, ZMQ_PUSH);
            sock->connect();
            socks.push_back(sock);
        }

        ifstream f(m_filename);
        if (f.is_open()) {
            int size = 32768;
            char buff[size];
            f.getline(buff, size);

            if (m_numAttr == 0) {
                char *pos = strchr(buff, m_delim);
                while (pos != NULL) {
                    ++m_numAttr;
                    pos = strchr(++pos, m_delim);
                }
            }

            size_t headSize = 2 * sizeof(uint64_t);
            size_t dataSize = size * m_numAttr * sizeof(TYPE);
            size_t rawSize = headSize + dataSize;
            char *raw = new char[rawSize];
            TYPE *data = (TYPE *) (raw + headSize);
int total = 0;
            for (uint64_t i = 0; f.good(); i++) {
                int j = 0;
                for (; j < size && f.good(); j++) {
                    char *pos = strchr(buff, m_delim);
                    while (pos != NULL) {
                        *pos = '\0';
                        pos = strchr(++pos, m_delim);
                    }

                    int dataOff = j * m_numAttr;
                    pos = buff;
                    for (int k = 0; k < m_numAttr; k++) {
                        #if TYPE == double
                            data[dataOff + k] = strtod(pos, &pos);
                        #elif TYPE == float
                            data[dataOff + k] = strtof(pos, &pos);
                        #elif TYPE == int
                            data[dataOff + k] = strtol(pos, &pos, 0);
                        #endif
                        pos++;
                    }
                    f.getline(buff, size);
total++;
                }

                Logging::info("Loaded Block(" + to_string(m_fileId) + ","
                        + to_string(i) + ")");
                *((uint64_t *) (raw)) = m_fileId;
                *((uint64_t *) (raw + sizeof(uint64_t))) = i;
                size_t size = j * m_numAttr * sizeof(TYPE);
                socks[i % socks.size()]->send(raw, headSize + size);
                raw = new char[rawSize];
                data = (TYPE *) (raw + headSize);
            }
            f.close();
Logging::info("File " + m_filename + " loaded (" + to_string(total) + " elts)");
        }
    }
private:
    string m_filename;
    char m_delim;
    uint64_t m_fileId;
    int m_numAttr;
    vector<WorkerMessage *> *m_workers;
};
}
#endif
