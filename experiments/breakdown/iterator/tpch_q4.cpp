#include "../tpch_q4_util.h"

class LineitemIterator : public Iterator<void *> {
public:
    LineitemIterator(int *l_orderkey, long *l_commitdate, long *l_receiptdate) {
        m_orderkey = l_orderkey;
        m_commitdate = l_commitdate;
        m_receiptdate = l_receiptdate;
        m_pos = 0;
        m_next = new void*[3];
    }

    ~LineitemIterator() {
        delete[] m_next;
    }

    virtual bool hasNext() {
        return m_pos < Q4_LINEITEM;
    }

    virtual void **next() {
        m_next[0] = (void *) (m_orderkey + m_pos);
        m_next[1] = (void *) (m_commitdate + m_pos);
        m_next[2] = (void *) (m_receiptdate + m_pos);
        m_pos++;
        return m_next;
    }
private:
    int *m_orderkey;
    long *m_commitdate;
    long *m_receiptdate;
    int m_pos;
    void **m_next;
};

class Pred1Iterator : public Iterator<void *> {
public:
    Pred1Iterator(Iterator<void *> *itr) {
        m_itr = itr;
        m_hasNext = false;
        m_next = NULL;
    }

    ~Pred1Iterator() {}

    virtual bool hasNext() {
        while (m_itr->hasNext()) {
            m_next = m_itr->next();
            long l_commitdate = *((long *) m_next[1]);
            long l_receiptdate = *((long *) m_next[2]);
            if (l_commitdate < l_receiptdate) {
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

class ExistsIterator : public Iterator<void> {
public:
    ExistsIterator(Iterator<void *> *itr, unordered_set<int> *exists) {
        m_itr = itr;
        m_exists = exists;
    }

    ~ExistsIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        void **data = m_itr->next();
        int l_orderkey = *((int *) data[0]);
        m_exists->insert(l_orderkey);
        return NULL;
    }
private:
    Iterator<void *> *m_itr;
    unordered_set<int> *m_exists;
};

class OrdersIterator : public Iterator<void *> {
public:
    OrdersIterator(int *o_orderkey, long *o_orderdate, char *o_orderpriority) {
        m_orderkey = o_orderkey;
        m_orderdate = o_orderdate;
        m_orderpriority = o_orderpriority;
        m_pos = 0;
        m_next = new void*[3];
    }

    ~OrdersIterator() {
        delete[] m_next;
    }

    virtual bool hasNext() {
        return m_pos < Q4_ORDERS;
    }

    virtual void **next() {
        m_next[0] = (void *) (m_orderkey + m_pos);
        m_next[1] = (void *) (m_orderdate + m_pos);
        m_next[2] = (void *) (m_orderpriority + m_pos);
        m_pos++;
        return m_next;
    }
private:
    int *m_orderkey;
    long *m_orderdate;
    char *m_orderpriority;
    int m_pos;
    void **m_next;
};

class Pred2Iterator : public Iterator<void *> {
public:
    Pred2Iterator(Iterator<void *> *itr, unordered_set<int> *exists) {
        m_itr = itr;
        m_exists = exists;
        m_hasNext = false;
        m_next = NULL;
    }

    ~Pred2Iterator() {}

    virtual bool hasNext() {
        while (m_itr->hasNext()) {
            m_next = m_itr->next();
            long o_orderkey = *((int *) m_next[0]);
            long o_orderdate = *((long *) m_next[1]);
            if (o_orderdate >= Q4_DATE1 && o_orderdate < Q4_DATE2)
                if (m_exists->find(o_orderkey) != m_exists->end()) {
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
    unordered_set<int> *m_exists;
    bool m_hasNext;
    void **m_next;
};

class SumIterator : public Iterator<void> {
public:
    SumIterator(Iterator<void *> *itr, unordered_map<char,int> *vals) {
        m_itr = itr;
        m_vals = vals;
    }

    ~SumIterator() {}

    virtual bool hasNext() {
        return m_itr->hasNext();
    }

    virtual void *next() {
        void **data = m_itr->next();
        char o_orderpriority = *((char *) data[2]);
        (*m_vals)[o_orderpriority]++;
        return NULL;
    }
private:
    Iterator<void *> *m_itr;
    unordered_map<char,int> *m_vals;
};

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file1 = argv[1];
    string file2 = argv[2];

    int *o_orderkey = new int[Q4_ORDERS];
    long *o_orderdate = new long[Q4_ORDERS];
    char *o_orderpriority = new char[Q4_ORDERS];
    int *l_orderkey = new int[Q4_LINEITEM];
    long *l_commitdate = new long[Q4_LINEITEM];
    long *l_receiptdate = new long[Q4_LINEITEM];
    init(file1, file2, o_orderkey, o_orderdate, o_orderpriority, l_orderkey,
            l_commitdate, l_receiptdate);
    unordered_set<int> *exists = new unordered_set<int>();
    unordered_map<char,int> *vals = new unordered_map<char,int>();

    test(vals);
    gettimeofday(&start, NULL);
    Iterator<void *> *itr1 = new LineitemIterator(l_orderkey, l_commitdate,
                                         l_receiptdate);
    Iterator<void *> *itr2 = new Pred1Iterator(itr1);
    Iterator<void> *itr3 = new ExistsIterator(itr2, exists);
    Iterator<void *> *itr4 = new OrdersIterator(o_orderkey, o_orderdate,
                                         o_orderpriority);
    Iterator<void *> *itr5 = new Pred2Iterator(itr4, exists);
    Iterator<void> *itr6 = new SumIterator(itr5, vals);
    while (itr3->hasNext())
        itr3->next();
    while (itr6->hasNext())
        itr6->next();
    gettimeofday(&stop, NULL);
    test(vals);
    printTime("tpch_q4", start, stop);

    delete[] o_orderkey;
    delete[] o_orderdate;
    delete[] o_orderpriority;
    delete[] l_orderkey;
    delete[] l_commitdate;
    delete[] l_receiptdate;
    delete exists;
    delete vals;
    delete itr1;
    delete itr2;
    delete itr3;
    delete itr4;
    delete itr5;
    delete itr6;

    return 0;
}
