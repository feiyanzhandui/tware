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
import org.apache.spark.graphx._
import org.apache.spark.rdd.RDD
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.storage.StorageLevel._

object PagerankMllib {
  val WARM = 1
  val ITER = 20

  def main(args: Array[String]) {
    val filename = args(0)
    val numPages = args(1).toInt
    val compress = args(2)
    val fraction = args(3)
    val numParts = args(4)
    val kryo = args(5).toBoolean

    var conf = new SparkConf()
      .setAppName("kmeans mllib")
      .set("spark.rdd.compress", compress)
      .set("spark.storage.memoryFraction", fraction)
      .set("spark.default.parallelism", numParts)
    if (kryo) {
      conf = conf.set("spark.serializer",
                 "org.apache.spark.serializer.KryoSerializer")
    }
    val sc = new SparkContext(conf)

    val graph = GraphLoader.edgeListFile(sc, filename).persist(MEMORY_ONLY)
    var ranks = graph.staticPageRank(WARM).vertices

    val start = System.nanoTime()
    ranks = graph.staticPageRank(ITER).vertices
    val stop = System.nanoTime()

    println("****    Time:" + (stop - start))
  }
}
