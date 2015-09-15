#include <ctime>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <vector>

#include "Client.h"
#include "Iterator.h"
#include "FunctionCompiler.h"
#include "Job.pb.h"
#include "RemoteReader.h"
#include "Tuple.h"

#include "LogitRegression.h"

using namespace std;
using namespace tupleware;

int main(int argc, const char *argv[]) {
    cout << endl << "Starting logit regression test..." << endl;
    cout << "\tCompiling UDFs..." << endl;
    struct timeval tstart, tstop;
    gettimeofday(&tstart, NULL);
    string jaddr = "localhost";
    int jport = Connection::JOB_PORT;
    Connection *jconn = Connection::push(jaddr, jport);
    jconn->open(CONNECT);

    for (int i = 1; i <= 3; i++) {
        Task *task = new Task();
        task->set_id(i);
        string file =  getenv("TUPLEWARE_HOME") + "/test/logit_regression/Task"
                      + to_string(i) + ".cpp";
        string llvm = FunctionCompiler::compile(file);
        task->set_llvm(llvm);
        jconn->send(task);
    }
    gettimeofday(&tstop, NULL);
    cout << "\tUDFs compiled." << endl;
    double start = (double) tstart.tv_sec + (double) tstart.tv_usec * 0.000001;
    double stop = (double) tstop.tv_sec + (double) tstop.tv_usec * 0.000001;
    double runtime = stop - start;

    cout << "\tAwaiting model..." << endl;
    Schema *sch = new Schema();
    for (int i = 0; i < 5; i++)
        sch->addDoubleAttribute();
    RemoteReader<Tuple> *rr =
            new RemoteReader<Tuple>(sch, jaddr, Connection::RESULT_PORT);
    vector<string> filenames;
    filenames.push_back(getenv("TUPLEWARE_HOME") + "/test/logit_regression/test1.csv");
    filenames.push_back( getenv("TUPLEWARE_HOME") + "/test/logit_regression/test2.csv");

    Tuple *w;
    if (rr->hasNext())
        w = rr->next();
    cout << "\tModel received." << endl;

    cout << endl
         << "Execution Details" << endl
         << "=================" << endl
         << "UDF compilation: " << runtime << " seconds" << endl
         << "Computed model:  " << w->toString() << endl
         << endl;

    cout << "Logit regression test complete." << endl << endl;
    return 0;
}
