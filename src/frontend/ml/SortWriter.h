#ifndef SORT_WRITER_H
#define SORT_WRITER_H

#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <set>
#include <vector>

#include "job/Task.h"
#include "net/Socket.h"
#include "util/Logging.h"

using std::function;
using std::multiset;
using std::vector;

namespace tupleware {
class SortWriter : public Task {
public:
    SortWriter(string filename, int numReaders, uint64_t numData, Socket *sock)
            : Task(NULL) {
        m_filename = filename + "_s";
        m_numReaders = numReaders;
        m_numData = numData;
        m_sock = sock;
    }

    virtual void run() {
        m_sock->bind();
        multiset<char *,function<bool (char *, char *)>> *sort =
                new multiset<char *,function<bool (char *, char *)>>(
                [](char *c1, char *c2) {
                    return memcmp(c1, c2, KEY_SIZE) < 0;
                }
        );

        int done = 0;
        vector<char *> data;
        while (done < m_numReaders) {
            char *recv;
            int size = m_sock->receive(&recv);
            done += (size == 0);
            data.push_back(recv);
            for (int i = 0; i < size; i += REC_SIZE)
                sort->insert(recv + i);
        }

        ofstream f(m_filename);
        if (f.is_open()) {
            for (auto itr = sort->begin(); itr != sort->end(); ++itr)
                f.write(*itr, REC_SIZE);
            f.close();
        }

        for (int i = 0; i < data.size(); i++)
            delete[] data[i];
        delete sort;
        m_sock->close();
    }
private:

    static const int REC_SIZE = 100;
    static const int KEY_SIZE = 10;

    string m_filename;
    int m_numReaders;
    uint64_t m_numData;
    Socket *m_sock;
};
}
#endif
