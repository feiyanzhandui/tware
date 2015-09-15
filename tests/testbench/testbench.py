import os
import sys

with open("testbench.cpp", 'w') as fout:
    fout.write("#include <gtest/gtest.h>\n")
    for f in os.listdir("../"):
        if f[0] != '_' and f != "testbench":
            for cpp in os.listdir("../" + f + "/"):
                path = "tests/" + f + "/" + cpp
                if path[-3:] == "cpp" and cpp[0] != '_':
                    fout.write('#include "' + path + '"\n')

    fout.write("int main(int argc, char **argv) {\n")
    fout.write("    testing::InitGoogleTest(&argc, argv);\n")
    fout.write("    return RUN_ALL_TESTS();\n")
    fout.write("}\n")
    fout.close();

