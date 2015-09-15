#include "../tpch_q6_util.h"

class LineitemIterator : public Iterator<void *> {
public:
    LineitemIterator(float *l_quantity, float *l_extendedprice,
            float *l_discount, long *l_shipdate) {
        m_quantity = l_quantity;
        m_extendedprice = l_extendedprice;
        m_discount = l_discount;
        m_shipdate = l_shipdate;
        m_pos = 0;
        m_next = new void*[4];
    }

    ~LineitemIterator() {
        delete[] m_next;
    }

    virtual bool hasNext() {
        return m_pos < Q6_LINEITEM;
    }

    virtual void **next() {
        m_next[0] = (void *) (m_quantity + m_pos);
        m_next[1] = (void *) (m_extendedprice + m_pos);
        m_next[2] = (void *) (m_discount + m_pos);
        m_next[3] = (void *) (m_shipdate + m_pos);
        m_pos++;
        return m_next;
    }
private:
    float *m_quantity;
    float *m_extendedprice;
    float *m_discount;
    long *m_shipdate;
    int m_pos;
    void **m_next;
};

class PredIterator : public Iterator<void *> {
public:
    PredIterator(Iterator<void *> *itr) {
        m_itr = itr;
        m_hasNext = false;
        m_next = NULL;
    }

    ~PredIterator() {}

    virtual bool hasNext() {
        while (m_itr->hasNext()) {
            m_next = m_itr->next();
            float l_quantity = *((float *) m_next[0]);
            float l_discount = *((float *) m_next[2]);
            long l_shipdate = *((long *) m_next[3]);
            if (l_shipdate >= Q6_DATE1
             && l_shipdate < Q6_DATE2
             && l_discount >= 0.05f
             && l_discount <= 0.07f
             && l_quantity < 24.0f) {
                m_hasNext = true;
                break;
            }
        }
        return m_hasNext;
    }

    virtual void **next() {
        m_hasNext = false;
        return m_next;
    }
private:
    Iterator<void *> *m_itr;
    bool m_hasNext;
    void **m_next;
};

class SumIterator : public Iterator<void> {
public:
    SumIterator(Iterator<void *> *itr, float *revenue) {
        m_itr = itr;
        m_revenue = revenue;
    }

    ~SumIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        void **data = m_itr->next();
        float l_extendedprice = *((float *) data[1]);
        float l_discount = *((float *) data[2]);
        *m_revenue += l_extendedprice * l_discount;
        return NULL;
    }
private:
    Iterator<void *> *m_itr;
    float *m_revenue;
};

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *l_quantity = new float[Q6_LINEITEM];
    float *l_extendedprice = new float[Q6_LINEITEM];
    float *l_discount = new float[Q6_LINEITEM];
    long *l_shipdate = new long[Q6_LINEITEM];
    init(file, l_quantity, l_extendedprice, l_discount, l_shipdate);
    float *revenue = new float(0.0f);

    gettimeofday(&start, NULL);
    Iterator<void *> *itr1 = new LineitemIterator(l_quantity, l_extendedprice,
                                         l_discount, l_shipdate);
    Iterator<void *> *itr2 = new PredIterator(itr1);
    Iterator<void> *itr3 = new SumIterator(itr2, revenue);
    while (itr3->hasNext())
        itr3->next();
    gettimeofday(&stop, NULL);
    test(*revenue);
    printTime("tpch_q6", start, stop);

    delete[] l_quantity;
    delete[] l_extendedprice;
    delete[] l_discount;
    delete[] l_shipdate;
    delete[] revenue;
    delete itr1;
    delete itr2;
    delete itr3;

    return 0;
}
