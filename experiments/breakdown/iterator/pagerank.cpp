#include "../pagerank_util.h"

class LinkIterator : public Iterator<int> {
public:
    LinkIterator(int *links) {
        m_pos = links;
        m_end = m_pos + LINK * 2;
    }

    ~LinkIterator() {}

    virtual bool hasNext() {
        return m_pos != m_end;
    }

    virtual int *next() {
        return m_pos += 2;
    }
private:
    int *m_pos;
    int *m_end;
};

class TrainIterator : public Iterator<void> {
public:
    TrainIterator(Iterator<int> *itr, int *counts, float *ranks, float *temp) {
        m_itr = itr;
        m_counts = counts;
        m_ranks = ranks;
        m_temp = temp;
    }

    ~TrainIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        int *in = m_itr->next();
        int src = in[0];
        int dst = in[1];
        m_temp[dst] += m_ranks[src] / m_counts[src];
        return NULL;
    }
private:
    Iterator<int> *m_itr;
    int *m_counts;
    float *m_ranks;
    float *m_temp;
};

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file1 = argv[1];
    string file2 = argv[2];

    int *links = new int[LINK * 2];
    int *counts = new int[PAGE];
    float *ranks = new float[PAGE];
    float *temp = new float[PAGE];
    init(file1, file2, links, counts, ranks, temp);

    test(temp);
    gettimeofday(&start, NULL);
    Iterator<int> *itr1 = new LinkIterator(links);
    Iterator<void> *itr2 = new TrainIterator(itr1, counts, ranks, temp);
    while (itr2->hasNext())
        itr2->next();
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("pagerank", start, stop);

    delete[] links;
    delete[] counts;
    delete[] ranks;
    delete[] temp;
    delete itr1;
    delete itr2;

    return 0;
}
