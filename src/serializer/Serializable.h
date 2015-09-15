#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <functional>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "serializer/Archive.h"

using std::function;
using std::string;
using std::type_info;
using std::unordered_map;

#include <iostream>
using namespace std;

namespace tupleware {
class Serializable {
typedef unordered_map<string,function<Serializable *(Archive *ar)>> Registry;
public:
    template <typename T>
    static void registerType() {
        Registry reg = Serializable::registry();
        string typeName = typeid(T).name();
        reg[typeName] = [](Archive *ar) {
            T *t = new T();
            t->deserialize(ar);
            return t;
        };
    }

    static Serializable *deserializePolymorphic(Archive *ar) {
        string typeName;
        ar->load(typeName);
        Registry reg = Serializable::registry();
        return reg[typeName](ar);
    }

    const type_info *type() {
        return &typeid(*this);
    }

    virtual void serialize(Archive *ar) = 0;
    virtual void deserialize(Archive *ar) = 0;
private:
    static Registry &registry() {
        static Registry reg;
        return reg;
    }
};
}
#endif
