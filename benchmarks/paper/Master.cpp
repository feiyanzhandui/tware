#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>

#include "Master.h"
#include "Socket.h"
#include "Util.h"
#include "Worker.h"
#include "bayes/BayesMaster1.h"
#include "bayes/BayesWorker1.h"
#include "kmeans/KmeansMaster1.h"
#include "kmeans/KmeansWorker1.h"
#include "kmeans/KmeansWorker2.h"
#include "kmeans/KmeansWorker3.h"
#include "logreg/LogregMaster1.h"
#include "logreg/LogregWorker1.h"
#include "pagerank/PagerankMaster1.h"
//#include "pagerank/PagerankMaster2.h"
#include "pagerank/PagerankWorker1.h"
//#include "pagerank/PagerankWorker2.h"
//#include "q1/Q1Master1.h"
//#include "q1/Q1Worker1.h"
//#include "tpch/q1/Q1Worker2.h"
//#include "tpch/q1/Q1Worker3.h"
//#include "q4/Q4Master1.h"
//#include "tpch/q4/Q4Master2.h"
//#include "q4/Q4Worker1.h"
//#include "tpch/q4/Q4Worker2.h"
//#include "q6/Q6Master1.h"
//#include "q6/Q6Worker1.h"
//#include "tpch/q6/Q6Worker2.h"
//#include "tpch/q6/Q6Worker3.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

Master *initMaster(int benchmark, int algo, int numWorkers,
        vector<string> files) {
    Socket *in = new Socket("*", PORT2, ZMQ_PULL);
    Socket *out = new Socket("*", PORT1, ZMQ_PUB);
    switch (benchmark) {
        case 1: switch (algo) {
            case 1: return new BayesMaster1(numWorkers, in, out, files);
        }
        case 2: switch (algo) {
            case 1:
            case 2:
            case 3: return new KmeansMaster1(numWorkers, in, out, files);
        }
        case 3: switch (algo) {
            case 1: return new LogregMaster1(numWorkers, in, out, files);
        }
        case 4: switch (algo) {
            case 1: return new PagerankMaster1(numWorkers, in, out, files);
        //    case 2: return new PagerankMaster2(numWorkers, in, out, files);
        }
        default: return NULL;
    }
}

Worker *initWorker(int benchmark, int algo, int id, int numWorkers,
        vector<string> files) {
    Socket *in = new Socket("localhost", PORT1, ZMQ_SUB);
    Socket *out = new Socket("localhost", PORT2, ZMQ_PUSH);
    switch (benchmark) {
        case 1: switch (algo) {
            case 1: return new BayesWorker1(id, numWorkers, in, out, files);
        }
        case 2: switch (algo) {
            case 1: return new KmeansWorker1(id, numWorkers, in, out, files);
            case 2: return new KmeansWorker2(id, numWorkers, in, out, files);
            case 3: return new KmeansWorker3(id, numWorkers, in, out, files);
        }
        case 3: switch (algo) {
            case 1: return new LogregWorker1(id, numWorkers, in, out, files);
        }
        case 4: switch (algo) {
            case 1: return new PagerankWorker1(id, numWorkers, in, out, files);
        //    case 2: return new PagerankWorker2(id, in, out, files); break;
        }
//        case 6: switch (algo) {
//            case 1: return new Q1Worker1(id, numWorkers, in, out, files);
//            case 2: return new Q1Worker2(id, numWorkers, in, out, files);
//            case 3: return new Q1Worker3(id, numWorkers, in, out, files);
//        }
//        case 7: switch (algo) {
//            case 1: return new Q4Worker1(id, numWorkers, in, out, files);
//            //case 2: return new Q4Worker2(id, numWorkers, in, out, files);
//        }
//        case 8: switch (algo) {
//            case 1: return new Q6Worker1(id, numWorkers, in, out, files);
//            case 2: return new Q6Worker2(id, numWorkers, in, out, files);
//            case 3: return new Q6Worker3(id, numWorkers, in, out, files);
//        }
        default: return NULL;
    }
}

int main(int argc, char *argv[]) {
    int benchmark = atoi(argv[1]);
    int algo = atoi(argv[2]);
    int numWorkers = atoi(argv[3]);
    int localWorkers = atoi(argv[4]);

    vector<string> files;
    char *file = strtok(argv[5], ",");
    while (file != NULL) {
        files.push_back(string(file));
        file = strtok(NULL, ",");
    }

    Socket::setMaxSockets(SOCKS);
    pthread_t t[localWorkers + 1];
    Master *master = initMaster(benchmark, algo, numWorkers, files);
    Worker *workers[localWorkers];

    pthread_create(&t[0], NULL, Master::run, (void *) master);
    for (int i = 0; i < localWorkers; i++) {
        workers[i] = initWorker(benchmark, algo, i, numWorkers, files);
        pthread_create(&t[i + 1], NULL, Worker::run, (void *) workers[i]);
    }
    for (int i = 0; i < localWorkers + 1; i++)
        pthread_join(t[i], NULL);

    delete master;
    for (int i = 0; i < localWorkers; i++)
        delete workers[i];

    return 0;
}
