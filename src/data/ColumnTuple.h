#ifndef COLUMN_TUPLE_H
#define COLUMN_TUPLE_H

#include <cstring>

#include "data/Tuple.h"

using std::memcpy;

namespace tupleware {
class ColumnTuple : public Tuple {
public:
    ColumnTuple(Schema *sch, char **data) : Tuple(sch) {
        m_data = data;
    }

    virtual double getDouble(int attrNum) {
        return *((double *) (m_data[attrNum]));
    }

    virtual float getFloat(int attrNum) {
        return *((float *) (m_data[attrNum]));
    }

    virtual int getInt(int attrNum) {
        return *((int *) (m_data[attrNum]));
    }

    virtual char *getString(int attrNum) {
        return m_data[attrNum];
    }

    virtual void setDouble(int attrNum, double val) {
        memcpy(m_data[attrNum], &val, sizeof(double));
    }

    virtual void setFloat(int attrNum, float val) {
        memcpy(m_data[attrNum], &val, sizeof(float));
    }

    virtual void setInt(int attrNum, int val) {
        memcpy(m_data[attrNum], &val, sizeof(int));
    }

    virtual void setString(int attrNum, char *val) {
        //int end = length < max ? length : max;
        int end = 8;
        memcpy(m_data[attrNum], val, end);
        *(m_data[attrNum] + end) = '\0';
    }
private:
    char **m_data;
};
}
#endif
