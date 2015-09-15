#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <vector>

using namespace std;

void basicStrategy(int cacheSize, int* data, int size);
void cacheStrategy(int cacheSize, int *data, int size);
void populateArray(int *vals, int size, int max);
void printTime();

clock_t tstart, tstop;

int main(int argc, char * argv[]){

  int cacheSize = 3 * 1024 * 1024;  //2mb in bytes
  const int DATA_SIZE= atoi(argv[1]);
  const int MAX_NUMBER = atoi(argv[2]);

  cout<<"Cache Size: " << cacheSize << " bytes\n"
          "Data Size: " << DATA_SIZE << "\n"
          "Max. Number: " <<MAX_NUMBER<<endl;

  int *vals = new int[DATA_SIZE];
  populateArray(vals, DATA_SIZE, MAX_NUMBER);

  cout << "----------------------" << endl
       << "    BASIC STRATEGY    " << endl
       << "----------------------" << endl;

  basicStrategy(cacheSize, vals, DATA_SIZE);
  printTime();
  delete vals;

  cout << "---------------------------" << endl
    << "    CACHE AWARE STRATEGY    " << endl
    << "---------------------------" << endl;

  int *vals2 = new int[DATA_SIZE];
  populateArray(vals2, DATA_SIZE, MAX_NUMBER);
  cacheStrategy(cacheSize, vals2, DATA_SIZE);
  printTime();
  delete vals2;
  return 0;

}

void basicStrategy(int cacheSize, int *vals, int size){

  tstart = clock();
  unordered_map<int, double> htable;
  htable.reserve(size);

  for (int i = 0; i < size; i++){
        double res = sqrt(vals[i]);
        htable[vals[i]] += res;
  }
  tstop = clock();
}

void cacheStrategy(int cacheSize, int *vals, int size){
  tstart = clock();

  int tupleSize = sizeof(1) + sizeof(1.2);
  int hashSize = size * tupleSize;

  double cacheFillFactor;
  int tuplesPerBatch, numBatches;
  if(hashSize > cacheSize){
    //hash table won't fit in cache
    cacheFillFactor = .99; //use 1/n of cache for hash table
    tuplesPerBatch = cacheFillFactor*cacheSize/tupleSize;
    numBatches = size / tuplesPerBatch;
  }
  else{
    cout<<"*** Hash table fits entirely in cache ***" <<endl;
    return;
  }

  cout << "\nTuple size: " << tupleSize << " bytes" << endl;
  cout << "Max. Hash Table Size: " << hashSize << " bytes" << endl;
  cout << "Cache Fill Factor: " << cacheFillFactor<< " " << endl;
  cout << "Tuples / batch: " << tuplesPerBatch << endl;
  cout << "Number of Batches (0-indexed): " << numBatches << endl;

  vector<unordered_map<int, double> > maps;
  maps.reserve(numBatches);

  //create numBatches of hash tables so that the hash table is
  //allowed to take up 1/fillFactor of the cache
  clock_t buildClockStart = clock();

  //create numBatches number of small hash tables
  for (int j = 0; j <= numBatches; j++){
    unordered_map<int, double> htable;
    htable.reserve(tuplesPerBatch);

    for(int i = 0; i < tuplesPerBatch; i++) {
      int index = j*tuplesPerBatch + i;
      if (index == size) {
        break;
      }

      double res = sqrt(vals[index]);
      htable[vals[index]] += res;
    }
    maps.push_back(htable);
  }
  clock_t buildClockEnd = clock();
  cout << "\nBuild Hash Table Time: ";
  cout << "Time: \t" << (double) (buildClockEnd - buildClockStart) / CLOCKS_PER_SEC <<endl;
  clock_t mergeClockStart = clock();

  //merge hash tables and create 1 final hash table
  int count = 0;
  unordered_map<int, double> finalHtable;
  finalHtable.reserve(size);

  for (int i = 0; i < maps.size(); i++){
    for(auto kv : maps[i]){
      finalHtable[kv.first] += kv.second;
    }

  }
  clock_t mergeClockEnd = clock();

  cout<<"Merge Hash Tables: ";
  cout << "Time: \t" << (double) (mergeClockEnd -mergeClockStart) / CLOCKS_PER_SEC << endl <<endl;
  tstop = clock();
}


void populateArray(int *vals, int size, int max) {
    srand((unsigned) time(NULL));
    for (int i = 0; i < size; i++)
        vals[i] = (int) rand() % max;
}

void printTime() {
    cout << "Time: \t" << (double) (tstop - tstart) / CLOCKS_PER_SEC << endl <<endl;
}
