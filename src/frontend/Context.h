#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>

#include "data/Any.h"
#include "serializer/Archive.h"
#include "serializer/Serializable.h"

using std::string;
using std::unordered_map;

#include <vector>
using std::vector;

namespace tupleware {
class TSet;

class Context : public Serializable {
friend class TSet;
public:
    template <typename T>
    T get(string name) {
        return m_vals[name];
    }

    template <typename T>
    void set(string name, T &val) {
        m_vals[name] = val;
    }

    virtual void serialize(Archive *ar) {
        for (auto itr = m_vals.begin(); itr != m_vals.end(); ++itr) {
            ar->save(itr->first);
            itr->second.serialize(ar);
        }
    }

    virtual void deserialize(Archive *ar) {
        //vector<float> temp;
        //ar->load(temp);
        //set<vector<float>>("w", temp);
    }
private:
    unordered_map<string,Any> m_vals;

    Context() : Serializable() {}
};
}
#endif
