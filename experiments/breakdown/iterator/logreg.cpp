#include "../logreg_util.h"

class DataIterator : public Iterator<float> {
public:
    DataIterator(float *d) {
        m_pos = d;
        m_end = m_pos + DATA * (ATTR + 1);
    }

    ~DataIterator() {}

    virtual bool hasNext() {
        return m_pos != m_end;
    }

    virtual float *next() {
        return m_pos += ATTR + 1;
    }
private:
    float *m_pos;
    float *m_end;
};

class TrainIterator : public Iterator<void> {
public:
    TrainIterator(Iterator<float> *itr, float *w, float *g) {
        m_itr = itr;
        m_w = w;
        m_g = g;
    }

    ~TrainIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        float *in = m_itr->next();
        float dot = 0.0f;
        for (int i = 0; i < ATTR; i++)
            dot += in[i] * m_w[i];
        float label = in[ATTR];
        float scale = (1.0f / (1.0f + exp(-label * dot)) - 1.0f) * label;
        for (int i = 0; i < ATTR; i++)
            m_g[i] += scale * in[i];
        return NULL;
    }
private:
    Iterator<float> *m_itr;
    float *m_w;
    float *m_g;
};

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *d = new float[DATA * (ATTR + 1)];
    float *w = new float[ATTR];
    float *g = new float[ATTR];
    init(file, d, w, g);

    test(g);
    gettimeofday(&start, NULL);
    Iterator<float> *itr1 = new DataIterator(d);
    Iterator<void> *itr2 = new TrainIterator(itr1, w, g);
    while (itr2->hasNext())
        itr2->next();
    gettimeofday(&stop, NULL);
    test(g);
    printTime("logreg", start, stop);

    delete[] d;
    delete[] w;
    delete[] g;
    delete itr1;
    delete itr2;

    return 0;
}
