#include <gtest/gtest.h>

#include "data/ColumnTuple.h"
#include "data/RowTuple.h"
#include "data/Schema.h"
#include "data/Tuple.h"

using namespace std;
using namespace tupleware;

TEST(Tuple, Ints) {
    Schema sch1 = Schema();
    sch1.addInt();
    sch1.addInt();
    sch1.addInt();
    sch1.addInt();

    int size1 = 4 * sizeof(int);
    char a1[size1];

    RowTuple rt1 = RowTuple(&sch1, a1);

    for (int i = 0; i < size1/sizeof(int); i++)
        rt1.setIntAt(i * sizeof(int), i);
    for (int i = 0; i < size1/sizeof(int); i++)
        ASSERT_EQ(rt1.getIntAt(i*sizeof(int)), rt1.getInt(i));
}

TEST(Tuple, Floats) {
    Schema sch1 = Schema();
    sch1.addFloat();
    sch1.addFloat();
    sch1.addFloat();
    sch1.addFloat();

    int size1 = 4 * sizeof(float);
    char a1[size1];

    RowTuple rt1 = RowTuple(&sch1, a1);

    for (int i = 0; i < size1/sizeof(float); i++)
        rt1.setIntAt(i * sizeof(float), i);
    for (int i = 0; i < size1/sizeof(float); i++)
        ASSERT_EQ(rt1.getFloatAt(i*sizeof(float)), rt1.getFloat(i));
}

TEST(Tuple, Doubles) {
    Schema sch1 = Schema();
    sch1.addDouble();
    sch1.addDouble();
    sch1.addDouble();
    sch1.addDouble();

    int size1 = 4 * sizeof(double);
    char a1[size1];

    RowTuple rt1 = RowTuple(&sch1, a1);

    for (int i = 0; i < size1/sizeof(double); i++)
        rt1.setIntAt(i * sizeof(double), i);
    for (int i = 0; i < size1/sizeof(double); i++)
        ASSERT_EQ(rt1.getDoubleAt(i*sizeof(double)), rt1.getDouble(i));
}


TEST(Tuple, Ints2) {
    Schema sch1 = Schema();
    sch1.addInt();
    sch1.addInt();
    sch1.addInt();
    sch1.addInt();

    int size = 4 * sizeof(int);
    char a1[size];

    RowTuple rt1 = RowTuple(&sch1, a1);

    for (int i = 0; i < size/sizeof(int); i++)
        rt1.setIntAt(i * sizeof(int), i);

    Schema sch2 = Schema();
    sch2.addInt();
    sch2.addInt();
    sch2.addInt();
    sch2.addInt();

    char a2[size];

    RowTuple rt2 = RowTuple(&sch2, a2);

    for (int i = 0; i < size/sizeof(int); i++)
        rt2.setIntAt(i * sizeof(int), rt1.getIntAt(i * sizeof(int)) * 2);
    for (int i = 0; i < size/sizeof(int); i++)
        ASSERT_EQ(rt2.getIntAt(i * sizeof(int)), rt1.getIntAt(i * sizeof(int)) * 2);
}

TEST(Tuple, Doubles2) {
    Schema sch1 = Schema();
    sch1.addDouble();
    sch1.addDouble();
    sch1.addDouble();
    sch1.addDouble();

    int size = 4 * sizeof(double);
    char a1[size];

    RowTuple rt1 = RowTuple(&sch1, a1);

    for (int i = 0; i < size/sizeof(double); i++)
        rt1.setIntAt(i * sizeof(double), i);

    Schema sch2 = Schema();
    sch2.addDouble();
    sch2.addDouble();
    sch2.addDouble();
    sch2.addDouble();

    char a2[size];

    RowTuple rt2 = RowTuple(&sch2, a2);

    for (int i = 0; i < size/sizeof(double); i++)
        rt2.setDoubleAt(i * sizeof(double), rt1.getDoubleAt(i * sizeof(double)) * 2);
    for (int i = 0; i < size/sizeof(double); i++)
        ASSERT_EQ(rt2.getDoubleAt(i * sizeof(double)), rt1.getDoubleAt(i * sizeof(double)) * 2);
}

TEST(Tuple, Floats2) {
    Schema sch1 = Schema();
    sch1.addFloat();
    sch1.addFloat();
    sch1.addFloat();
    sch1.addFloat();

    int size = 4 * sizeof(float);
    char a1[size];

    RowTuple rt1 = RowTuple(&sch1, a1);

    for (int i = 0; i < size/sizeof(float); i++)
        rt1.setIntAt(i * sizeof(float), i);

    Schema sch2 = Schema();
    sch2.addFloat();
    sch2.addFloat();
    sch2.addFloat();
    sch2.addFloat();

    char a2[size];

    RowTuple rt2 = RowTuple(&sch2, a2);

    for (int i = 0; i < size/sizeof(float); i++)
        rt2.setFloatAt(i * sizeof(float), rt1.getFloatAt(i * sizeof(float)) * 2);
    for (int i = 0; i < size/sizeof(float); i++)
        ASSERT_EQ(rt2.getFloatAt(i * sizeof(float)), rt1.getFloatAt(i * sizeof(float)) * 2);
}

TEST(Tuple, Mixed) {
    Schema sch8 = Schema();
    sch8.addInt();
    sch8.addFloat();
    sch8.addDouble();

    int size8 = sizeof(int) + sizeof(float) + sizeof(double);
    char a8[size8];

    RowTuple rt8 = RowTuple(&sch8, a8);
    rt8.setInt(0, 1);
    rt8.setFloat(1, 2.01);
    rt8.setDouble(2, 15.1234);
    ASSERT_EQ(rt8.getInt(0), 1);
    ASSERT_EQ(rt8.getFloat(1), 2.01f);
    ASSERT_EQ(rt8.getDouble(2), 15.1234);
}

/* TEST(Tuple, ColumnTuple) {
    ADD COLUMN TUPLE
}
*/

/*
 TEST(Tuple, HybridTuple) {
    ADD HYBRID TUPLE
}
*/
