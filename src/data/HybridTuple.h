#ifndef HYBRID_TUPLE_H
#define HYBRID_TUPLE_H

#include <cstring>

#include "data/ColumnTuple.h"
#include "data/RowTuple.h"
#include "data/Tuple.h"

using std::memcpy;

namespace tupleware {
class HybridTuple : public Tuple {
public:
    HybridTuple(Schema *sch, RowTuple row, ColumnTuple col) : Tuple(sch) {
        m_row = row;
        m_col = col;
    }

    virtual double getDouble(int colNum) {
        return 0;
    }

    virtual float getFloat(int colNum) {
        return 0;
    }

    virtual int getInt(int colNum) {
        return 0;
    }

    virtual char *getString(int colNum) {
        return NULL;
    }

    virtual void setDouble(int colNum, double val) {
    }

    virtual void setFloat(int colNum, float val) {
    }

    virtual void setInt(int colNum, int val) {
    }

    virtual void setString(int colNum, char *val) {
    }
private:
    RowTuple m_row;
    ColumnTuple m_col;
};
}
#endif
