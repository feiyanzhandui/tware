#ifndef ANY_H
#define ANY_H

#include <typeinfo>

#include "serializer/Serializable.h"

using std::type_info;

namespace tupleware {
class Any : public Serializable {
public:
    Any() : Serializable() {
        m_val = NULL;
    }

    template <typename T>
    Any(T &val) : Serializable() {
        m_val = new Value<T>(val);
    }

    ~Any() {
        delete m_val;
    }

    Any &operator=(const Any &other) {
        m_val = other.m_val;
        return *this;
    }

    template <typename T>
    Any &operator=(T &val) {
        delete m_val;
        m_val = new Value<T>(val);
        return *this;
    }

    template <typename T>
    operator T &() {
        return dynamic_cast<Value<T> *>(m_val)->value();
    }

    virtual void serialize(Archive *ar) {
        m_val->serialize(ar);
    }

    virtual void deserialize(Archive *ar) {
        m_val->deserialize(ar);
    }
private:
    class AnyValue : public Serializable {
    public:
        virtual ~AnyValue() {}
        virtual const type_info &type() const = 0;
    };

    template <typename T>
    class Value : public AnyValue {
    public:
        Value(T val) : AnyValue() {
            m_val = val;
        }

        virtual ~Value() {}

        T &value() {
            return m_val;
        }

        virtual const type_info &type() const {
            return typeid(T);
        }

        virtual void serialize(Archive *ar) {
            //ar->save(m_val);
        }

        virtual void deserialize(Archive *ar) {
            //ar->load(m_val);
        }
    private:
        T m_val;
    };

    AnyValue *m_val;
};
}
#endif
