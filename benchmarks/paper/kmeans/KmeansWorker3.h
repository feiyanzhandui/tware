#ifndef KMEANS_WORKER3_H
#define KMEANS_WORKER3_H

#include <cmath>
#include <string>
#include <vector>

#include "../Socket.h"
#include "../Util.h"
#include "../Worker.h"
#include "KmeansUtil.h"

using std::string;
using std::vector;

class KmeansWorker3 : public Worker {
public:
    KmeansWorker3(int id, int numWorkers, Socket *in, Socket *out,
            vector<string> files) : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        m_data = new float[KMEANS_DATA * KMEANS_ATTR];
        m_cent = new float[KMEANS_CENT * KMEANS_ATTR];
        m_send = new float[KMEANS_CENT * (KMEANS_ATTR + 1)];
        m_assign = new int[TILE];
        kmeans_init_data(m_files[0], m_data);
    }

    virtual void execute() {
        while (m_in->receive((char *) m_cent)) {
            for (int i = 0; i < KMEANS_CENT * (KMEANS_ATTR + 1); i++)
                m_send[i] = 0.0f;
            for (int i = 0; i < KMEANS_DATA / TILE; i++) {
                for (int j = 0; j < TILE; j++) {
                    int dataOff = i * TILE * KMEANS_ATTR + j * KMEANS_ATTR;

                    //distance
                    float dist[KMEANS_CENT] = {0.0f};
                    for (int k = 0; k < KMEANS_CENT; k++) {
                        int centOff = k * KMEANS_ATTR;
                        for (int l = 0; l < KMEANS_ATTR; l++) {
                            float d = m_cent[centOff + l] - m_data[dataOff + l];
                            dist[k] += d * d;
                        }
                        dist[k] = sqrtf(dist[k]);
                    }

                    //minimum
                    m_assign[j] = 0;
                    float min = dist[0];
                    for (int k = 1; k < KMEANS_CENT; k++)
                        if (dist[k] < min) {
                            min = dist[k];
                            m_assign[j] = k;
                        }
                }

                for (int j = 0; j < TILE; j++) {
                    int dataOff = i * TILE * KMEANS_ATTR + j * KMEANS_ATTR;

                    //reassign
                    int sendOff = m_assign[j] * (KMEANS_ATTR + 1);
                    for (int k = 0; k < KMEANS_ATTR; k++)
                        m_send[sendOff + k] += m_data[dataOff + k];
                    m_send[sendOff + KMEANS_ATTR]++;
                }
            }
            m_out->send((char *) m_send, KMEANS_SEND2);
        }
    }

    virtual void cleanup() {
        delete[] m_data;
        delete[] m_cent;
        delete[] m_send;
        delete[] m_assign;
    }
private:
    float *m_data;
    float *m_cent;
    float *m_send;
    int *m_assign;
};
#endif
