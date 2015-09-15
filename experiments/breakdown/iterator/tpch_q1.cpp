#include "../tpch_q1_util.h"

class LineitemIterator : public Iterator<void *> {
public:
    LineitemIterator(char *l_returnflag, char *l_linestatus, float *l_quantity,
            float *l_extendedprice, float *l_discount, float *l_tax,
            long *l_shipdate) {
        m_returnflag = l_returnflag;
        m_linestatus = l_linestatus;
        m_quantity = l_quantity;
        m_extendedprice = l_extendedprice;
        m_discount = l_discount;
        m_tax = l_tax;
        m_shipdate = l_shipdate;
        m_pos = 0;
        m_next = new void*[7];
    }

    ~LineitemIterator() {
        delete[] m_next;
    }

    virtual bool hasNext() {
        return m_pos < Q1_LINEITEM;
    }

    virtual void **next() {
        m_next[0] = (void *) (m_returnflag + m_pos);
        m_next[1] = (void *) (m_linestatus + m_pos);
        m_next[2] = (void *) (m_quantity + m_pos);
        m_next[3] = (void *) (m_extendedprice + m_pos);
        m_next[4] = (void *) (m_discount + m_pos);
        m_next[5] = (void *) (m_tax + m_pos);
        m_next[6] = (void *) (m_shipdate + m_pos);
        m_pos++;
        return m_next;
    }
private:
    char *m_returnflag;
    char *m_linestatus;
    float *m_quantity;
    float *m_extendedprice;
    float *m_discount;
    float *m_tax;
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
            long l_shipdate = *((long *) m_next[6]);
            if (l_shipdate <= Q1_DATE1) {
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
    SumIterator(Iterator<void *> *itr, unordered_map<short,char *> *vals) {
        m_itr = itr;
        m_vals = vals;
    }

    ~SumIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        void **data = m_itr->next();
        char l_returnflag = *((char *) data[0]);
        char l_linestatus = *((char *) data[1]);
        float l_quantity = *((float *) data[2]);
        float l_extendedprice = *((float *) data[3]);
        float l_discount = *((float *) data[4]);
        float l_tax = *((float *) data[5]);

        short key = l_returnflag << 8;
        key |= l_linestatus;
        auto itr = m_vals->find(key);
        char *val;
        if (itr == m_vals->end()) {
            val = new char[Q1_VALSIZE];
            val[0] = l_returnflag;
            val[1] = l_linestatus;
            *((int *) (val + 2)) = 0;
            *((float *) (val + 6)) = 0.0f;
            *((float *) (val + 10)) = 0.0f;
            *((float *) (val + 14)) = 0.0f;
            *((float *) (val + 18)) = 0.0f;
            *((float *) (val + 22)) = 0.0f;
            (*m_vals)[key] = val;
        }
        else
            val = itr->second;
        *((int *) (val + 2)) += 1;
        *((float *) (val + 6)) += l_quantity;
        *((float *) (val + 10)) += l_extendedprice;
        *((float *) (val + 14)) += l_extendedprice * (1 - l_discount);
        *((float *) (val + 18)) += l_extendedprice * (1 - l_discount)
                                 * (1 + l_tax);
        *((float *) (val + 22)) += l_discount;

        return NULL;
    }
private:
    Iterator<void *> *m_itr;
    unordered_map<short,char *> *m_vals;
};

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    char *l_returnflag = new char[Q1_LINEITEM];
    char *l_linestatus = new char[Q1_LINEITEM];
    float *l_quantity = new float[Q1_LINEITEM];
    float *l_extendedprice = new float[Q1_LINEITEM];
    float *l_discount = new float[Q1_LINEITEM];
    float *l_tax = new float[Q1_LINEITEM];
    long *l_shipdate = new long[Q1_LINEITEM];
    init(file, l_returnflag, l_linestatus, l_quantity, l_extendedprice,
            l_discount, l_tax, l_shipdate);
    unordered_map<short,char *> *vals = new unordered_map<short,char *>();

    test(vals);
    gettimeofday(&start, NULL);
    Iterator<void *> *itr1 = new LineitemIterator(l_returnflag, l_linestatus,
                                         l_quantity, l_extendedprice,
                                         l_discount, l_tax, l_shipdate);
    Iterator<void *> *itr2 = new PredIterator(itr1);
    Iterator<void> *itr3 = new SumIterator(itr2, vals);
    while (itr3->hasNext())
        itr3->next();
    gettimeofday(&stop, NULL);
    test(vals);
    printTime("tpch_q1", start, stop);

    delete[] l_returnflag;
    delete[] l_linestatus;
    delete[] l_quantity;
    delete[] l_extendedprice;
    delete[] l_discount;
    delete[] l_tax;
    delete[] l_shipdate;
    delete vals;
    delete itr1;
    delete itr2;
    delete itr3;

    return 0;
}
