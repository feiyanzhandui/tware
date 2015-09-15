 #include <iostream>
                     #include "frontend/TSet.h"
                     #include "frontend/Connection.h"
                     #include "util/Defaults.h"

                     using namespace std;
                     using namespace tupleware;
                     int main() {
                     TSet("file.csv").map("(int x) {return x*2;}")->reduce("(int x, int y) {return x+y;}", "(int x) {return x%4;}");
                     }
