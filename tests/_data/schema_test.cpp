#include <gtest/gtest.h>

#include "file/Block.h"
#include "data/Collector.h"
#include "data/HashCollector.h"
#include "data/Schema.h"
#include "data/Store.h"
#include "data/Tuple.h"

using namespace tupleware;


TEST(Schema, Empty) {
    Schema s1 = Schema();
    ASSERT_EQ(s1.numberOfAttributes(), 0);
    ASSERT_EQ(s1.size(), 0);
}

TEST(Schema, Doubles) {
    Schema s1 = Schema();
    s1.addDouble();
    ASSERT_EQ(s1.numberOfAttributes(), 1);
    ASSERT_EQ(s1.size(), 8);
}

TEST(Schema, Floats) {
    Schema s1 = Schema();
    s1.addFloat();
    ASSERT_EQ(s1.numberOfAttributes(), 1);
    ASSERT_EQ(s1.size(), 4);
}

TEST(Schema, Ints) {
    Schema s1 = Schema();
    s1.addInt();
    ASSERT_EQ(s1.numberOfAttributes(), 1);
    ASSERT_EQ(s1.size(), 4);
}

TEST(Schema, Multi) {
    Schema s1 = Schema();
    s1.addInt();
    s1.addInt();
    s1.addFloat();
    s1.addDouble();
    s1.addDouble();
    ASSERT_EQ(s1.numberOfAttributes(), 5);
    ASSERT_EQ(s1.size(), 28);
}
