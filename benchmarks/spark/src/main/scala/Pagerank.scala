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
import java.util.StringTokenizer

import org.apache.spark.rdd.RDD
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.storage.StorageLevel._

object Pagerank {
  val WARM = 1
  val ITER = 20

  def main(args: Array[String]) {
    val filename1 = args(0)
    val filename2 = args(1)
    val numPages = args(2).toInt
    val compress = args(3)
    val numParts = args(4)
    val kryo = args(5).toBoolean

    var conf = new SparkConf()
      .setAppName("pagerank")
      .set("spark.rdd.compress", compress)
      .set("spark.default.parallelism", numParts)
    if (kryo) {
      conf = conf.set("spark.serializer",
                 "org.apache.spark.serializer.KryoSerializer")
    }
    val sc = new SparkContext(conf)

    val file1 = sc.textFile(filename1, numParts.toInt)
    val links = file1.map(parseLink _)
    if (kryo) {
      links.persist(MEMORY_ONLY_SER)
    }
    else {
      links.persist(MEMORY_ONLY)
    }

    val file2 = sc.textFile(filename2, numParts.toInt)
    val temp = file2.map(parsePage _).toArray
    val counts = Array.fill[Int](numPages)(0)
    var ranks = Array.fill[Float](numPages)(1.0f)
    var i = 0
    while (i < temp.length) {
      val page = temp(i)
      counts(page.id) = page.count
      i += 1
    }

    i = 0
    while (i < WARM) {
      links.map {
        l => (l(1), ranks(l(0)) / counts(l(0)))
      }.reduceByKey(_ + _).collect().foreach {
        r => ranks(r._1) = 0.15f + 0.85f * r._2
      }
      i += 1
    }

    ranks = Array.fill[Float](numPages)(1.0f)
    val start = System.nanoTime()
    i = 0
    while (i < ITER) {
      links.map {
        l => (l(1), ranks(l(0)) / counts(l(0)))
      }.reduceByKey(_ + _).collect().foreach {
        r => ranks(r._1) = 0.15f + 0.85f * r._2
      }
      i += 1
    }
    val stop = System.nanoTime()


    println("runtime: " + (stop - start))
  }

  case class Page(id: Int, count: Int, rank: Float)

  def parsePage(line: String): Page = {
    val tok = new StringTokenizer(line, ",")
    val id = tok.nextToken.toInt
    val count = tok.nextToken.toInt
    val rank = tok.nextToken.toFloat
    Page(id, count, rank)
  }

  def parseLink(line: String): Array[Int] = {
    return line.split(',').map(_.toInt).toArray
  }
}
