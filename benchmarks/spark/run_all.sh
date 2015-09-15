BAYES_FILE=/data/agg/test_data/small/bayes.csv
KMEANS_FILE=/data/agg/test_data/small/kmeans.csv
LOGREG_FILE=/data/agg/test_data/small/reg.csv
PAGERANK_LINKS=/data/agg/test_data/small/links.csv
PAGERANK_PAGES=/data/agg/test_data/small/pages.csv

BAYES_ATTR=90
KMEANS_ATTR=2
KMEANS_CENT=4
LOGREG_ATTR=90
PAGERANK_NUM_PAGES=5716810

SCALE_FACTOR=1
PMIN=8
PMAX=32
PSTEP=8
MEM=60g
MASTER="local"
COMPRESS="true,false"
KRYO="true,false"
RUNS=1

sbt package
echo "algo,compress,parts,kryo,time"

python run.py -algo "Bayes" -f1 $BAYES_FILE -a $BAYES_ATTR -m $MASTER -mem $MEM -sf $SCALE_FACTOR -pmin $PMIN -pmax $PMAX -pstep $PSTEP -comp $COMPRESS -kryo $KRYO -n $RUNS 
python run.py -algo "Kmeans" -f1 $KMEANS_FILE -a $KMEANS_ATTR -c $KMEANS_CENT -m $MASTER -mem $MEM -sf $SCALE_FACTOR -pmin $PMIN -pmax $PMAX -pstep $PSTEP -comp $COMPRESS -kryo $KRYO -n $RUNS
python run.py -algo "Logreg" -f1 $LOGREG_FILE -a $LOGREG_ATTR -m $MASTER -mem $MEM -sf $SCALE_FACTOR -pmin $PMIN -pmax $PMAX -pstep $PSTEP -comp $COMPRESS -kryo $KRYO -n $RUNS
python run.py -algo "Pagerank" -f1 $PAGERANK_LINKS -f2 $PAGERANK_PAGES -p $PAGERANK_NUM_PAGES -m $MASTER -mem $MEM -sf $SCALE_FACTOR -pmin $PMIN -pmax $PMAX -pstep $PSTEP -comp $COMPRESS -kryo $KRYO -n $RUNS
