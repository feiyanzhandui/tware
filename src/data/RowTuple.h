#ifndef ROW_TUPLE_H
#define ROW_TUPLE_H

#include <cstring>

#include "data/Schema.h"
#include "data/Tuple.h"

using std::memcpy;

namespace tupleware {
class RowTuple : public Tuple {
public:
    RowTuple(Schema *sch, char *data) : Tuple(sch) {
        m_data = data;
        int numAttrs = m_sch->numberOfAttributes();
        m_offsets = new int[numAttrs];
        m_offsets[0] = 0;
        for (int i = 1; i < numAttrs; i++)
            m_offsets[i] = m_offsets[i - 1] + m_sch->getCapacity(i - 1);
    }

    char *data() {
        return m_data;
    }

    double getDoubleAt(int offset) {
        return *((double *) (m_data + offset));
    }

    double getFloatAt(int offset) {
        return *((float *) (m_data + offset));
    }

    int getIntAt(int offset) {
        return *((int *) (m_data + offset));
    }

    char *getStringAt(int offset) {
        return m_data + offset;
    }

    void setData(char *data, size_t size) {
        memcpy(m_data, data, size);
    }

    void setDoubleAt(int offset, double val) {
        memcpy(m_data + offset, &val, sizeof(double));
    }

    void setFloatAt(int offset, float val) {
        memcpy(m_data + offset, &val, sizeof(float));
    }

    void setIntAt(int offset, int val) {
        memcpy(m_data + offset, &val, sizeof(int));
    }

    void setStringAt(int offset, char *val, int length, int max) {
        char *start = m_data + offset;
        int end = length < max ? length : max;
        strncpy(start, val, end);
        *(start + end) = '\0';
    }

    virtual double getDouble(int attrNum) {
        return getDoubleAt(m_offsets[attrNum]);
    }

    virtual float getFloat(int attrNum) {
        return getFloatAt(m_offsets[attrNum]);
    }

    virtual int getInt(int attrNum) {
        return getIntAt(m_offsets[attrNum]);
    }

    virtual char *getString(int attrNum) {
        return getStringAt(m_offsets[attrNum]);
    }

    virtual void setDouble(int attrNum, double val) {
        setDoubleAt(m_offsets[attrNum], val);
    }

    virtual void setFloat(int attrNum, float val) {
        setFloatAt(m_offsets[attrNum], val);
    }

    virtual void setInt(int attrNum, int val) {
        setIntAt(m_offsets[attrNum], val);
    }

    virtual void setString(int attrNum, char *val) {
        setStringAt(m_offsets[attrNum], val, 8, 0);//add max
    }
private:
    char *m_data;
    int *m_offsets;
};
}
#endif
