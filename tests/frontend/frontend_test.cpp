#include <array>
#include <iostream>

#include "frontend/TSet.h"
#include "frontend/Connection.h"
#include "frontend/Context.h"
#include "util/Defaults.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    Connection *conn = Connection::getConnection("localhost", 4000);
usleep(100000);
    array<float,10> w;
    w.fill(0.5f);

    TSet *t0 = new TSet(conn, NULL);
    Context *ctx = t0->context();
    ctx->set("w", w);
    array<float,10> w2 = ctx->get<array<float,10>>("w");
    for (int i = 0; i < 10; i++)
        cout << w2[i] << endl;
    //TSet *t1 = t0->map("int f1(string t) {return atoi(t.c_str());}")
    //             ->map("tuple<int,int> f2(int t) {return make_tuple(t, t * 2);}")
    //             ->reduce("tuple<int,int> f3(tuple<int,int> t1, tuple<int,int> t2) {int k = get<0>(t1); int v1 = get<1>(t1); int v2 = get<1>(t2); return make_tuple(k, v1 + v2);}",
    //                      "int f4(tuple<int,int> t) {return get<0>(t);}")
    //             ->evaluate();
    //cout << "Job Submitted" << endl;
    usleep(100000);
    return 0;
}
