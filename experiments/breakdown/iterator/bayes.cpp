#include "../bayes_util.h"

class DataIterator : public Iterator<int> {
public:
    DataIterator(int *data) {
        m_pos = data;
        m_end = m_pos + DATA * (ATTR + 1);
    }

    ~DataIterator() {}

    virtual bool hasNext() {
        return m_pos != m_end;
    }

    virtual int *next() {
        return m_pos += ATTR + 1;
    }
private:
    int *m_pos;
    int *m_end;
};

class TrainIterator : public Iterator<void> {
public:
    TrainIterator(Iterator<int> *itr, int *counts) {
        m_itr = itr;
        m_counts = counts;
    }

    ~TrainIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        int *in = m_itr->next();
        int label = in[ATTR];
        m_counts[label]++;
        int countOff = VALUE + label * ATTR * VALUE;
        for (int i = 0; i < ATTR; i++)
            m_counts[countOff + i * VALUE + in[i]]++;
        return NULL;
    }
private:
    Iterator<int> *m_itr;
    int *m_counts;
};

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    int *data = new int[DATA * (ATTR + 1)];
    init(file, data);
    int *counts = new int[VALUE + VALUE * ATTR * VALUE];
    init(counts);

    test(counts);
    gettimeofday(&start, NULL);
    Iterator<int> *itr1 = new DataIterator(data);
    Iterator<void> *itr2 = new TrainIterator(itr1, counts);
    while (itr2->hasNext())
        itr2->next();
    gettimeofday(&stop, NULL);
    test(counts);
    printTime("bayes", start, stop);

    delete[] data;
    delete[] counts;
    delete itr1;
    delete itr2;

    return 0;
}
