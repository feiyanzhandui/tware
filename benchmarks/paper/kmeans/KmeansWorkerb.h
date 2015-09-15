#ifndef KMEANS_WORKER3_H
#define KMEANS_WORKER3_H

#include <cmath>
#include <string>
#include <vector>

#include "../Util.h"
#include "../Worker.h"
#include "../ZmqSocket.h"
#include "KmeansUtil.h"

using std::string;
using std::vector;

class KmeansWorker3 : public Worker {
public:
    KmeansWorker3(int id, int numWorkers, ZmqSocket *in, ZmqSocket *out,
            vector<string> files)
            : Worker(id, numWorkers, in, out, files) {}

    virtual void init() {
        m_x = new float[KMEANS_DATA];
        m_y = new float[KMEANS_DATA];
        m_c = new float[KMEANS_CENT * 2];
        m_t = new float[KMEANS_CENT * 3];
        m_d = new float[KMEANS_CENT * TILE];
        m_a = new int[TILE];

        ifstream f(m_files[0]);
        if (f.is_open()) {
            string line;
            char delim;
            for (int i = 0; i < KMEANS_DATA && getline(f, line); i++) {
                istringstream buff(line);
                buff >> m_x[i] >> delim >> m_y[i];
            }
            f.close();
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
            float *x = m_x;
            float *y = m_y;
            for (int i = 0; i < KMEANS_TILES; i++) {
                float *d = m_d;
                for (int j = 0; j < TILE; j++) {
                    for (int k = 0; k < KMEANS_CENT; k++) {
                        float dx = cx[k] - x[j];
                        float dy = cy[k] - y[j];
                        d[k] = sqrtf(dx * dx + dy * dy);
                    }
                    d += KMEANS_CENT;
                }
                d = m_d;
                for (int j = 0; j < TILE; j++) {
                    float m = d[0];
                    m_a[j] = 0;
                    for (int k = 1; k < KMEANS_CENT; k++) {
                        float n = d[k];
                        if (n < m) {
                            m = n;
                            m_a[j] = k;
                        }
                    }
                    d += KMEANS_CENT;
                }
                for (int j = 0; j < TILE; j++) {
                    tx[m_a[j]] += x[j];
                    ty[m_a[j]] += y[j];
                    ++tc[m_a[j]];
                }
                x += TILE;
                y += TILE;
            }
            if (KMEANS_EXTRA > 0) {
                float *d = m_d;
                for (int i = 0; i < KMEANS_EXTRA; i++) {
                    for (int j = 0; j < KMEANS_CENT; j++) {
                        float dx = cx[j] - x[i];
                        float dy = cy[j] - y[i];
                        d[i] = sqrtf(dx * dx + dy * dy);
                    }
                    d += KMEANS_CENT;
                }
                d = m_d;
                for (int i = 0; i < KMEANS_EXTRA; i++) {
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
                for (int i = 0; i < KMEANS_EXTRA; i++) {
                    tx[m_a[i]] += x[i];
                    ty[m_a[i]] += y[i];
                    ++tc[m_a[i]];
                }
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
