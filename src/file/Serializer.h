#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cereal/types/tuple.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <vector>

using cereal::BinaryOutputArchive;
using cereal::JSONOutputArchive;
using cereal::XMLOutputArchive;
using std::ostringstream;
using std::tuple;
using std::unordered_map;
using std::vector;

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

namespace tupleware {
class Serializer {
public:

//auto *v6 = new vector<tuple<int,string>>(); v6->push_back(make_tuple(0,"zero")); v6->push_back(make_tuple(1,"one")); v6->push_back(make_tuple(2,"two"));
//Serializer::serializeBinary(v6);
//Serializer::serializeJson(v6);
//Serializer::serializeXml(v6);
    template <typename T>
    static Block *serializeBinary(T *data) {
        ostringstream stream;
        BinaryOutputArchive *archive = new BinaryOutputArchive(stream);
        //(*archive)(get<0>((*data)[0]));
        (*archive)(*data);
        delete archive;
        return new Block(0, NULL, 0);
    }

    template <typename T>
    static Block *serializeJson(T *data) {
        ostringstream stream;
        JSONOutputArchive *archive = new JSONOutputArchive(stream);
        (*archive)(*data);
        delete archive;
        return new Block(0, NULL, 0);
    }

    template <typename T>
    static Block *serializePortable(vector<T> *data) {
        return NULL;
    }

    void serializeJson(T *data) {
        JSONOutputArchive *archive = new JSONOutputArchive(stream);
        (*archive)(*data);
        delete archive;
        return new Block(0, NULL, 0);
    }

    template <typename T>
    static Block *serializeXml(T *data) {
        ostringstream stream;
        XMLOutputArchive *archive = new XMLOutputArchive(stream);
        (*archive)(*data);
        delete archive;
        return new Block(0, NULL, 0);
    }

    template <typename T>
    static T *deserializeBinary(Block *data) {
        return NULL;
    }

    //template <typename T>
    //static T *deserializeJson(Block *data) {
    //    return NULL;
    //}

    template <typename T>
    static vector<T> *deserializeJson(Block *data) {
        return NULL;
    }

    template <typename T>
    static T *deserializePortable(Block *data) {
        return NULL;
    }

    template <typename T>
    static T *deserializeXml(Block *data) {
        return NULL;
    }
};
}
#endif
