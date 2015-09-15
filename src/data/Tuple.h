#ifndef TUPLE_H
#define TUPLE_H

#include "data/Schema.h"

namespace tupleware {
class Tuple {
public:
    Schema *schema() {
        return m_sch;
    }

    virtual double getDouble(int attrNum) = 0;
    virtual float getFloat(int attrNum) = 0;
    virtual int getInt(int attrNum) = 0;
    virtual char *getString(int attrNum) = 0;
    virtual void setDouble(int attrNum, double val) = 0;
    virtual void setFloat(int attrNum, float val) = 0;
    virtual void setInt(int attrNum, int val) = 0;
    virtual void setString(int attrNum, char *val) = 0;
protected:
    Schema *m_sch;

    Tuple(Schema *sch) {
        m_sch = sch;
    }
};
}
#endif
