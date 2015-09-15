#include <cmath>
#include <iostream>
#include <sys/time.h>
#include <vector>

#include "CsvReader.h"
#include "Iterator.h"
#include "RemoteReader.h"
#include "RemoteWriter.h"
#include "Schema.h"

using namespace std;
using namespace tupleware;

#define D 5

Schema *getModelSchema();
Schema *getDataSchema();

void local(int workerNum, string filename) {
    cout << endl << "Starting local worker #" << workerNum << "..." << endl;
    string addr = "localhost";
    int port = Connection::RESULT_PORT + workerNum;

    //receive model
    Schema *wsch = getModelSchema();
    RemoteReader<Tuple> *wr = new RemoteReader<Tuple>(wsch, addr, port);
    if (!wr->hasNext()) {
        cout << "ERROR: no model received" << endl;
        return;
    }

    //create gradient
    Tuple *g = new Tuple(wsch);
    RemoteWriter<Tuple> *gw = new RemoteWriter<Tuple>(6000);

    //load data
    CsvReader *dr = new CsvReader(getDataSchema(), filename);
    vector<Tuple *> *dset = new vector<Tuple *>();
    while (dr->hasNext())
        dset->push_back(dr->next());

    //loop while model has not converged
    for (int i = 0; wr->hasNext(); i++) {
        Tuple *w = wr->next();
        for (int i = 0; i < D; i++)
            g->setDouble(i, 0);

        //iterate over data
        for (Tuple *val : *dset) {
            double z = 0;
            for (int i = 0; i < D; i++)
                z += val->getDouble(i) * w->getDouble(i);
            double a = val->getDouble(D);
            double scale = (1 / (1 + exp(-a * z)) - 1) * a;
            for (int i = 0; i < D; i++)
                g->setDouble(i, g->getDouble(i) + scale * val->getDouble(i));
        }

        //send resulting gradient
        //cout << g->toString() << endl;
        gw->write(g);
    }
    wr->close();
    gw->close();

    cout << "Worker terminated." << endl << endl;
}

void global(int numWorkers, double conv, int maxIterations) {
    cout << endl << "Starting global worker..." << endl;
    cout << "(numWorkers=" << numWorkers << "; conv=" << conv
         << "; maxIterations=" << maxIterations << ")" << endl;
    string addr = "localhost";

    struct timeval tstart, tstop;
    gettimeofday(&tstart, NULL);
    Schema *wsch = getModelSchema();
    Tuple *w = new Tuple(wsch);

    srand((unsigned) time(NULL));
    for (int i = 0; i < D; i++) {
        double x = ((double) rand() / (double) RAND_MAX) * 2 - 1;
        w->setDouble(i, x);
    }

    vector<RemoteWriter<Tuple> *> ww = vector<RemoteWriter<Tuple> *>();
    for (int i = 0; i < numWorkers; i++) {
        int port = Connection::RESULT_PORT + i + 1;
        ww.push_back(new RemoteWriter<Tuple>(port));
        ww[i]->write(w);
    }

    RemoteReader<Tuple> *gr = new RemoteReader<Tuple>(wsch, addr, 6000);
    int i = 0;
    double c;
    for (; i < maxIterations; i++) {
        c = 0;
        if (gr->hasNext()) {
            Tuple *g = gr->next();
            for (int j = 0; j < D; j++) {
                double y = g->getDouble(j) / numWorkers;
                w->setDouble(j, w->getDouble(j) - y);
                c += abs(y);
            }
        }

        if (c < conv)
            break;

        ww[i % 2]->write(w);
    }

    RemoteWriter<Tuple> *result = new RemoteWriter<Tuple>(5000);
    result->write(w);
    result->close();

    for (int i = 0; i < numWorkers; i++)
        ww[i]->close();

    gettimeofday(&tstop, NULL);
    double start = (double) tstart.tv_sec + (double) tstart.tv_usec * 0.000001;
    double stop = (double) tstop.tv_sec + (double) tstop.tv_usec * 0.000001;
    double runtime = stop - start;

    cout << endl
         << "Execution Details" << endl
         << "=================" << endl
         << "Final model:     " << w->toString() << endl
         << "Iterations:      " << i << endl
         << "Convergence:     " << c << endl
         << "Running time:    " << runtime << " seconds" << endl
         << endl;
}

void test(Tuple *w, int numWorkers, vector<string> filenames) {
    int err = 0;
    int N = 0;
    Schema *dataSch = getDataSchema();
    CsvReader *data;
    Tuple *val;
    for (string filename : filenames) {
        data = new CsvReader(dataSch, filename);
        while (data->hasNext()) {
            N++;
            double z = 0;
            val = data->next();
            for (int i = 0; i < D; i++)
                z += val->getDouble(i) * w->getDouble(i);
            if (exp(z) / (1 + exp(z)) > 1 - 0.05)
                if (val->getDouble(0) == 1)
                    err++;
            //else if (val->getDouble(D) == -1)
            //    err++;
        }
        data->close();
    }
    cout << err << " of " << N << " classified correctly." << endl;
}

Schema *getSchema(int numAttrs) {
    Schema *sch = new Schema();
    for (int i = 0; i < numAttrs; i++)
        sch->addDoubleAttribute();
    return sch;
}

Schema *getModelSchema() {
    return getSchema(D);
}

Schema *getDataSchema() {
    return getSchema(D + 1);
}
