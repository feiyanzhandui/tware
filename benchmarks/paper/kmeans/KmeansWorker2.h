#ifndef KMEANS_WORKER2_H
#define KMEANS_WORKER2_H

#include <cmath>
#include <string>
#include <vector>

#include "../Socket.h"
#include "../Util.h"
#include "../Worker.h"
#include "KmeansUtil.h"

using std::string;
using std::vector;

class KmeansWorker2 : public Worker {
public:
    KmeansWorker2(int id, int numWorkers, Socket *in, Socket *out,
            vector<string> files) : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        m_data = new float[KMEANS_DATA * KMEANS_ATTR];
        m_cent = new float[KMEANS_CENT * KMEANS_ATTR];
        m_send = new float[KMEANS_CENT * (KMEANS_ATTR + 1)];
        kmeans_init_data(m_files[0], m_data);
    }

    virtual void execute() {
        while (m_in->receive((char *) m_cent)) {
            for (int i = 0; i < KMEANS_CENT * (KMEANS_ATTR + 1); i++)
                m_send[i] = 0.0f;
            for (int i = 0; i < KMEANS_DATA; i++) {
                int dataOff = i * KMEANS_ATTR;

                //distance
                float dist[KMEANS_CENT] = {0.0f};
                for (int j = 0; j < KMEANS_CENT; j++) {
                    int centOff = j * KMEANS_ATTR;
                    for (int k = 0; k < KMEANS_ATTR; k++) {
                        float d = m_cent[centOff + k] - m_data[dataOff + k];
                        dist[j] += d * d;
                    }
                    dist[j] = sqrtf(dist[j]);
                }

                //minimum
                int assign = 0;
                float min = dist[0];
                for (int j = 1; j < KMEANS_CENT; j++)
                    if (dist[j] <= min) {
                        min = dist[j];
                        assign = j;
                    }

                //reassign
                int sendOff = assign * (KMEANS_ATTR + 1);
                for (int j = 0; j < KMEANS_ATTR; j++)
                    m_send[sendOff + j] += m_data[dataOff + j];
                m_send[sendOff + KMEANS_ATTR]++;
            }
            m_out->send((char *) m_send, KMEANS_SEND2);
        }
    }

    virtual void cleanup() {
        delete[] m_data;
        delete[] m_cent;
        delete[] m_send;
    }
private:
    float *m_data;
    float *m_cent;
    float *m_send;
};
#endif
