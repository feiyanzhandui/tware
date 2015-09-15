#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/deque.hpp>
#include <cereal/types/forward_list.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/stack.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/valarray.hpp>
#include <cereal/types/vector.hpp>
#include <sstream>

using cereal::BinaryInputArchive;
using cereal::BinaryOutputArchive;
using cereal::JSONInputArchive;
using cereal::JSONOutputArchive;
using cereal::PortableBinaryInputArchive;
using cereal::PortableBinaryOutputArchive;
using cereal::XMLInputArchive;
using cereal::XMLOutputArchive;
using std::istringstream;
using std::ostringstream;

#include <iostream>
using std::cout;
using std::endl;

namespace tupleware {
class Archive {
public:
    Archive(istringstream &in) {
        m_jsonIn = new JSONInputArchive(in);
        m_jsonOut = NULL;
    }

    Archive(ostringstream &out) {
        m_jsonIn = NULL;
        m_jsonOut = new JSONOutputArchive(out);
    }

    ~Archive() {
        if (m_jsonIn != NULL)
            delete m_jsonIn;
        if (m_jsonOut != NULL)
            delete m_jsonOut;
    }

    template <typename T>
    void load(T &val) {
        (*m_jsonIn)(val);
    }

    template <typename T>
    void save(T &val) {
        (*m_jsonOut)(val);
    }
    /*template <typename T>
    Archive &operator>>(Archive &arch, const T &val) {
        //(*(arch->m_in))(val);
        return *this;
    }

    template <typename T>
    Archive &operator<<(const T &val) {
        (*m_out)(val);
        return *this;
    }*/
private:
    BinaryInputArchive *m_binaryIn;
    BinaryOutputArchive *m_binaryOut;
    JSONInputArchive *m_jsonIn;
    JSONOutputArchive *m_jsonOut;
    PortableBinaryInputArchive *m_portableIn;
    PortableBinaryOutputArchive *m_portableOut;
    XMLInputArchive *m_xmlIn;
    XMLOutputArchive *m_xmlOut;
};
}
#endif
