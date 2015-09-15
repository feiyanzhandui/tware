/*
 * Copyright 2015 Tupleware
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import org.apache.spark.rdd.RDD
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.storage.StorageLevel._

object Bayes {
  val LABELS = 10
  val VALUES = 10

  def main(args: Array[String]) {
    val filename = args(0)
    val numAttrs = args(1).toInt
    val compress = args(2)
    val numParts = args(3)
    val kryo = args(4).toBoolean

    var conf = new SparkConf()
      .setAppName("bayes")
      .set("spark.rdd.compress", compress)
      .set("spark.default.parallelism", numParts)
    if (kryo) {
      conf = conf.set("spark.serializer",
                 "org.apache.spark.serializer.KryoSerializer")
    }
    val sc = new SparkContext(conf)

    val file = sc.textFile(filename, numParts.toInt).map(parse _)
    if (kryo) {
      file.persist(MEMORY_ONLY_SER)
    }
    else {
      file.persist(MEMORY_ONLY)
    }
    println(file.count())

    val size = LABELS + LABELS * numAttrs * VALUES
    val start = System.nanoTime()
    val counts = file.aggregate(new Array[Int](size))(train(_, _), add(_, _))
    val stop = System.nanoTime()

    println("runtime: " + (stop - start))
  }

  def parse(line: String): Array[Int] = {
    return line.split(',').map(_.toInt).toArray
  }

  def add(a1: Array[Int], a2: Array[Int]): Array[Int] = {
    var i = 0
    while (i < a1.length) {
      a1(i) += a2(i)
      i += 1
    }
    return a1
  }

  def train(counts: Array[Int], data: Array[Int]): Array[Int] = {
    val label = data(data.length - 1)
    counts(label) += 1
    val offset = LABELS + label * (data.length - 1) * VALUES
    var i = 0
    while (i < data.length - 1) {
      counts(offset + i * VALUES + data(i)) += 1
      i += 1
    }
    return counts
  }
}
