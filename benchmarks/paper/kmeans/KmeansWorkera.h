#ifndef KMEANS_WORKER2_H
#define KMEANS_WORKER2_H

#include <cmath>
#include <string>
#include <vector>

#include "../Util.h"
#include "../Worker.h"
#include "../ZmqSocket.h"
#include "KmeansUtil.h"

using std::string;
using std::vector;

class KmeansWorker2 : public Worker {
public:
    KmeansWorker2(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        m_x = new float[KMEANS_DATA];
        m_y = new float[KMEANS_DATA];
        m_c = new float[KMEANS_CENT * 2];
        m_t = new float[KMEANS_CENT * 3];
        m_d = new float[KMEANS_CENT * KMEANS_DATA];
        m_a = new int[KMEANS_DATA];

        ifstream f0(m_files[0]);
        if (f0.is_open()) {
            string line;
            char delim;
            for (int i = 0; i < KMEANS_DATA && getline(f0, line); i++) {
                istringstream buff(line);
                buff >> m_x[i] >> delim >> m_y[i];
            }
            f0.close();
        }
    }

    virtual void execute() {
        float *cx = m_c;
        float *cy = m_c + KMEANS_CENT;
        float *tx = m_t;
        float *ty = m_t + KMEANS_CENT;
        float *tc = m_t + 2 * KMEANS_CENT;
        while (m_in->receive((char *) m_c)) {
            for (int i = 0; i < 3 * KMEANS_CENT; i++)
                m_t[i] = 0.0f;
            float *d = m_d;
            for (int i = 0; i < KMEANS_DATA; i++) {
                for (int j = 0; j < KMEANS_CENT; j++) {
                    float dx = cx[j] - m_x[i];
                    float dy = cy[j] - m_y[i];
                    d[j] = sqrtf(dx * dx + dy * dy);
                }
                d += KMEANS_CENT;
            }
            d = m_d;
            for (int i = 0; i < KMEANS_DATA; i++) {
                float m = d[0];
                m_a[i] = 0;
                for (int j = 1; j < KMEANS_CENT; j++) {
                    float n = d[j];
                    if (n < m) {
                        m = n;
                        m_a[i] = j;
                    }
                }
                d += KMEANS_CENT;
            }
            for (int i = 0; i < KMEANS_DATA; i++) {
                tx[m_a[i]] += m_x[i];
                ty[m_a[i]] += m_y[i];
                ++tc[m_a[i]];
            }
            m_out->send((char *) m_t, KMEANS_SEND2);
        }
    }

    virtual void cleanup() {
        delete[] m_x;
        delete[] m_y;
        delete[] m_c;
        delete[] m_t;
        delete[] m_d;
        delete[] m_a;
    }
private:
    float *m_x;
    float *m_y;
    float *m_c;
    float *m_t;
    float *m_d;
    int *m_a;
};
#endif
