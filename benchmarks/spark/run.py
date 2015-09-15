#
# Copyright 2015 Tupleware
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#!/usr/bin/python
import argparse
import os
from subprocess import call

SPARK_INSTALL = '/data/agg/spark/'
JAR = '/data/agg/tupleware/benchmarks/spark/target/scala-2.10/simple-project_2.10-1.0.jar'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-algo', '--algorithm', required=True)
    parser.add_argument('-f1', '--filename1')
    parser.add_argument('-f2', '--filename2')
    parser.add_argument('-a', '--attr')
    parser.add_argument('-c', '--cent')
    parser.add_argument('-p', '--pages')
    parser.add_argument('-m', '--master', default="local[1]")
    parser.add_argument('-mem', '--memory', required=True)
    parser.add_argument('-sf', '--scale', default="1")
    parser.add_argument('-pmin', '--parts_min', default="1")
    parser.add_argument('-pmax', '--parts_max', default="1")
    parser.add_argument('-pstep', '--parts_step', default="1")
    parser.add_argument('-comp', '--compress', default="true,false")
    parser.add_argument('-kryo', '--kryo', default="true,false")
    parser.add_argument('-n', '--num_times', default="1")
    args = parser.parse_args()
                
    for c in args.compress.split(','):
        for p in range(int(args.parts_min),int(args.parts_max)+1,int(args.parts_step)):
            for k in args.kryo.split(','):
                for n in range(int(args.num_times)):
                    master = args.master
                    if "local" in master:
                        master = "local[" + str(p) + "]"
                    base_cmd = SPARK_INSTALL + 'bin/spark-submit ' \
                             + ' --class "' + args.algorithm + '"' \
                             + ' --master "' + master + '"' \
                             + ' --executor-memory ' + args.memory \
                             + ' ' + JAR + ' ' \
                             + '"'

                    scale = int(args.scale)
                    for i in range(scale):
                        base_cmd += args.filename1
                        if i < scale-1:
                            base_cmd += ','
                    base_cmd += '"'

                    if args.algorithm == "Bayes":
                        base_cmd += " " + args.attr + " "
                    elif args.algorithm == "Kmeans":
                        base_cmd += " " + args.attr + " " + args.cent + " "
                    elif args.algorithm == "Logreg":
                        base_cmd += " " + args.attr + " "
                    elif args.algorithm == "Pagerank":
                        base_cmd += " " + args.filename2 + " " + args.pages + " "
                    else:
                        print "Bad Algorithm"
                        exit(1)

                    exec_cmd =  base_cmd + c + " " + str(p) + " " + k
                    exec_cmd += ' 2>1 | grep "runtime: " | sed "s/runtime: /""/g" ' 
                    call(["printf " + args.algorithm \
                          + "," + c \
                          + "," + str(p) \
                          + "," + k \
                          + ","], shell=True)
                    exec_cmd = "export _JAVA_OPTIONS=-Xmx" + args.memory + "; " \
                          + exec_cmd
                    call([exec_cmd], shell=True)


if __name__ == '__main__':
    main()
