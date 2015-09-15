#!/bin/bash

#args
KMEANS=$1
LINKS=$2
COUNTS=$3
LOGREG=$4
BAYES=$5
ORDERS=$6
LINEITEM=$7

#baseline
>&2 printf "\n*** baseline ***\n"
#./run_regular.sh base/kmeans $KMEANS
#./run_regular.sh base/pagerank $LINKS $COUNTS
#./run_regular.sh base/logreg $LOGREG
#./run_regular.sh base/bayes $BAYES
#./run_regular.sh base/tpch_q1 $LINEITEM
#./run_regular.sh base/tpch_q4 $ORDERS $LINEITEM
#./run_regular.sh base/tpch_q6 $LINEITEM

#h1
>&2 printf "\n*** h1 ***\n"
#./run_regular.sh h1/kmeans $KMEANS
#./run_regular.sh h1/pagerank $LINKS $COUNTS
#./run_regular.sh h1/logreg $LOGREG
#./run_regular.sh h1/bayes $BAYES
#./run_regular.sh h1/tpch_q1 $LINEITEM
#./run_regular.sh h1/tpch_q4 $ORDERS $LINEITEM

#h2
>&2 printf "\n*** h2 ***\n"
#./run_regular.sh h2/kmeans $KMEANS
#./run_regular.sh h2/pagerank $LINKS $COUNTS
#./run_regular.sh h2/logreg $LOGREG
#./run_regular.sh h2/bayes $BAYES
#./run_regular.sh h2/tpch_q1 $LINEITEM
#./run_regular.sh h2/tpch_q4 $ORDERS $LINEITEM

#h3
>&2 printf "\n*** h3 ***\n"
#./run_regular.sh h3/tpch_q1 $LINEITEM
#./run_regular.sh h3/tpch_q4 $ORDERS $LINEITEM
./run_regular.sh h3/tpch_q6_mk1 $LINEITEM
./run_regular.sh h3/tpch_q6_mk2 $LINEITEM
./run_regular.sh h3/tpch_q6_mk3 $LINEITEM
./run_regular.sh h3/tpch_q6_mk4 $LINEITEM
./run_regular.sh h3/tpch_q6_mk5 $LINEITEM
./run_regular.sh h3/tpch_q6_mk6 $LINEITEM
./run_regular.sh h3/tpch_q6_mk7 $LINEITEM
./run_regular.sh h3/tpch_q6_mk8 $LINEITEM
./run_regular.sh h3/tpch_q6_mk9 $LINEITEM
./run_regular.sh h3/tpch_q6_mk10 $LINEITEM

#iterator
>&2 printf "\n*** iterator ***\n"
#./run_regular.sh iterator/kmeans $KMEANS
#./run_regular.sh iterator/pagerank $LINKS $COUNTS
#./run_regular.sh iterator/logreg $LOGREG
#./run_regular.sh iterator/bayes $BAYES
#./run_regular.sh iterator/tpch_q1 $LINEITEM
#./run_regular.sh iterator/tpch_q4 $ORDERS $LINEITEM
#./run_regular.sh iterator/tpch_q6 $LINEITEM

#java
>&2 printf "\n*** java ***\n"
#./run_java.sh Kmeans $KMEANS
#./run_java.sh Pagerank $LINKS $COUNTS
#./run_java.sh Logreg $LOGREG
#./run_java.sh Bayes $BAYES
#./run_java.sh TpchQ1 $LINEITEM
#./run_java.sh TpchQ4 $ORDERS $LINEITEM
#./run_java.sh TpchQ6 $LINEITEM

#noinline
>&2 printf "\n*** noinline ***\n"
#./run_noinline.sh noinline/kmeans $KMEANS
#./run_noinline.sh noinline/pagerank $LINKS $COUNTS
#./run_noinline.sh noinline/logreg $LOGREG
#./run_noinline.sh noinline/bayes $BAYES
#./run_noinline.sh noinline/tpch_q1 $LINEITEM
#./run_noinline.sh noinline/tpch_q4 $ORDERS $LINEITEM
#./run_noinline.sh noinline/tpch_q6 $LINEITEM

>&2 printf "\n*** nosimd ***\n"
#./run_nosimd.sh base/kmeans $KMEANS
#./run_nosimd.sh base/pagerank $LINKS $COUNTS
#./run_nosimd.sh base/logreg $LOGREG
#./run_nosimd.sh base/bayes $BAYES
#./run_nosimd.sh base/tpch_q1 $LINEITEM
#./run_nosimd.sh base/tpch_q4 $ORDERS $LINEITEM
#./run_nosimd.sh base/tpch_q6 $LINEITEM
