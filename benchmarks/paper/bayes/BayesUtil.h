#ifndef BAYES_UTIL_H
#define BAYES_UTIL_H

#define BAYES_DATA 6100
//#define BAYES_DATA 7622
//#define BAYES_DATA 76220
//#define BAYES_DATA 86016//2747253
#define BAYES_ATTR 1024
//#define BAYES_ATTR 90
#define BAYES_VALUE 10
#define BAYES_COUNT (BAYES_VALUE + BAYES_VALUE * BAYES_ATTR * BAYES_VALUE)
#define BAYES_SEND (BAYES_COUNT * sizeof(int))

#endif
