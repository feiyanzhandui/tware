#ifndef SORT_READER_H
#define SORT_READER_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "job/Task.h"
#include "net/Socket.h"
#include "net/ZmqSocket.h"
#include "util/Defaults.h"
#include "util/Logging.h"

using std::ifstream;
using std::string;
using std::vector;

namespace tupleware {
class SortReader : public Task {
public:
    SortReader(string filename, int numWriters, uint64_t numData,
            vector<string> workers) : Task(NULL) {
        m_filename = filename;
        m_numWriters = numWriters;
        m_numData = numData;
        m_workers = workers;
    }

    virtual void run() {
        int port = Defaults::SORT_PORT;
        vector<Socket *> socks;
usleep(10000000);
        for (int i = 0; i < m_workers.size(); i++)
            for (int j = 0; j < m_numWriters; j++) {
                Socket *sock = new ZmqSocket(m_workers[i], port + j, ZMQ_PUSH);
usleep(1000);
                sock->connect();
                socks.push_back(sock);
            }

        int writers = m_numWriters * m_workers.size();
        int parts = 256 / writers;
        ifstream f(m_filename);
        if (f.is_open()) {
            char *read = new char[BLK_SIZE * REC_SIZE];
            char **send = new char*[writers];
            int *offs = new int[writers];
            for (int i = 0; i < writers; i++) {
                send[i] = new char[BLK_SIZE * REC_SIZE];
                offs[i] = 0;
            }

            for (int i = 0; i < m_numData; i += BLK_SIZE) {
                f.read(read, REC_SIZE * BLK_SIZE);
                int writer[BLK_SIZE];
                for (int j = 0; j < BLK_SIZE; j++)
                    writer[j] = ((unsigned char) read[j * REC_SIZE]) / parts;
                for (int j = 0; j < BLK_SIZE; j++) {
                    int w = writer[j];
                    memcpy(send[w] + offs[w], read + j * REC_SIZE, REC_SIZE);
                    offs[w] += REC_SIZE;
                }
                /*for (int j = 0; j < BLK_SIZE * REC_SIZE; j += REC_SIZE) {
                    int w = ((unsigned char) read[j]) / parts;
                    memcpy(send[w] + offs[w], read + j, REC_SIZE);
                    offs[w] += REC_SIZE;
                }*/
//bool toobig = writer >= max;
//writer = toobig * (max - 1) + (!toobig) * writer;
                for (int j = 0; j < writers; j++)
                    if (offs[j] > 0) {
                        socks[j]->send(send[j], offs[j]);
                        send[j] = new char[BLK_SIZE * REC_SIZE];
                        offs[j] = 0;
                    }
            }
            f.close();
            delete[] read;
            for (int i = 0; i < writers; i++)
                delete[] send[i];
            delete[] send;
            delete[] offs;
        }

        for (int i = 0; i < socks.size(); i++) {
usleep(1000);
            socks[i]->send(NULL, 0);
            //socks[i]->close();
            //delete socks[i];
        }
    }
private:
    static const int REC_SIZE = 100;
    static const int BLK_SIZE = 10000;

    string m_filename;
    int m_numWriters;
    uint64_t m_numData;
    vector<string> m_workers;
};
}
#endif
