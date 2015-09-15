#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <sstream>
#include <string>

#include "serializer/Archive.h"
#include "serializer/Serializable.h"

using std::istringstream;
using std::ostringstream;
using std::string;

namespace tupleware {
class Serializer {
public:
    static string serialize(Serializable *ser) {
        ostringstream out;
        Archive *ar = new Archive(out);
        string name = ser->type()->name();
cout << "name=" << name << endl;
        ar->save(name);
        ser->serialize(ar);
        delete ar;
cout << out.str() << endl;
        return out.str();
    }

    static Serializable *deserialize(string data) {
        istringstream in(data);
cout << "data=" << data << endl;
        Archive *ar = new Archive(in);
        Serializable *msg = Serializable::deserializePolymorphic(ar);
        delete ar;
        return msg;
    }
};
}
#endif
