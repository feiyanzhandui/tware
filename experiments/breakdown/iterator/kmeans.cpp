#include "../kmeans_util.h"

class DataIterator : public Iterator<float> {
public:
    DataIterator(float *data) {
        m_pos = data;
        m_end = m_pos + DATA * ATTR;
    }

    ~DataIterator() {}

    virtual bool hasNext() {
        return m_pos != m_end;
    }

    virtual float *next() {
        return m_pos += ATTR;
    }
private:
    float *m_pos;
    float *m_end;
};

class DistanceIterator : public Iterator<float> {
public:
    DistanceIterator(Iterator<float> *itr, float *cent) {
        m_itr = itr;
        m_cent = cent;
        m_out = new float[ATTR + CENT];
    }

    ~DistanceIterator() {
        delete[] m_out;
    }

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual float *next() {
        float *in = m_itr->next();
        for (int i = 0; i < ATTR; i++)
            m_out[i] = in[i];
        for (int i = 0; i < CENT; i++)
            m_out[ATTR + i] = 0.0f;
        for (int i = 0; i < CENT; i++) {
            int centOff = i * ATTR;
            for (int j = 0; j < ATTR; j++) {
                float d = m_cent[centOff + j] - in[j];
                m_out[ATTR + i] += d * d;
            }
            m_out[ATTR + i] = sqrtf(m_out[ATTR + i]);
        }
        return m_out;
    }
private:
    Iterator<float> *m_itr;
    float *m_cent;
    float *m_out;
};

class MinimumIterator : public Iterator<float> {
public:
    MinimumIterator(Iterator<float> *itr) {
        m_itr = itr;
        m_out = new float[ATTR + 1];
    }

    ~MinimumIterator() {
        delete[] m_out;
    }

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual float *next() {
        float *in = m_itr->next();
        for (int i = 0; i < ATTR; i++)
            m_out[i] = in[i];
        *((int *) (m_out + ATTR)) = 0;
        float min = in[ATTR];
        for (int i = 1; i < CENT; i++)
            if (in[ATTR + i] < min) {
                min = in[ATTR + i];
                *((int *) (m_out + ATTR)) = i;
            }
        return m_out;
    }
private:
    Iterator<float> *m_itr;
    float *m_out;
};

class ReassignIterator : public Iterator<void> {
public:
    ReassignIterator(Iterator<float> *itr, float *temp) {
        m_itr = itr;
        m_temp = temp;
    }

    ~ReassignIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        float *in = m_itr->next();
        int tempOff = *((int *) (in + ATTR)) * (ATTR + 1);
        for (int i = 0; i < ATTR; i++)
            m_temp[tempOff + i] += in[i];
        m_temp[tempOff + ATTR]++;
        return NULL;
    }
private:
    Iterator<float> *m_itr;
    float *m_temp;
};

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *data = new float[DATA * ATTR];
    float *cent = new float[CENT * ATTR];
    float *temp = new float[CENT * (ATTR + 1)];
    init(file, data, cent, temp);

    test(temp);
    gettimeofday(&start, NULL);
    Iterator<float> *itr1 = new DataIterator(data);
    Iterator<float> *itr2 = new DistanceIterator(itr1, cent);
    Iterator<float> *itr3 = new MinimumIterator(itr2);
    Iterator<void> *itr4 = new ReassignIterator(itr3, temp);
    while (itr4->hasNext())
        itr4->next();
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("kmeans", start, stop);

    delete[] data;
    delete[] cent;
    delete[] temp;
    delete itr1;
    delete itr2;
    delete itr3;
    delete itr4;

    return 0;
}
