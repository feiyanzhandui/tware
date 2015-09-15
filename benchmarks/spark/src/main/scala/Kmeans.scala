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
import scala.math.sqrt

import org.apache.spark.rdd.RDD
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.storage.StorageLevel._

object Kmeans {
  val WARM = 5
  val ITER = 20

  def main(args: Array[String]) {
    val filename = args(0)
    val numAttrs = args(1).toInt
    val numCents = args(2).toInt
    val compress = args(3)
    val numParts = args(4)
    val kryo = args(5).toBoolean

    var conf = new SparkConf()
      .setAppName("kmeans")
      .set("spark.rdd.compress", compress)
      .set("spark.default.parallelism", numParts)
    if (kryo) {
      conf = conf.set("spark.serializer",
                 "org.apache.spark.serializer.KryoSerializer")
    }
    val sc = new SparkContext(conf)

    val file = sc.textFile(filename, numParts.toInt)
    val data = file.map(parse _)
    if (kryo) {
      file.persist(MEMORY_ONLY_SER)
    }
    else {
      file.persist(MEMORY_ONLY)
    }

    var cent = centroids(data, numCents)
    var resSize = cent.length + numCents
    var i = 0
    while (i < WARM) {
      cent = recompute(data.map(t => distance(t, cent, numCents))
                 .map(t => minimum(t._1, t._2))
                 .aggregate(new Array[Float](resSize))(reassign(_, _),
                     add(_, _)), cent, numCents, numAttrs)
      i += 1
    }

    cent = centroids(data, numCents)
    val start = System.nanoTime()
    i = 0
    while (i < ITER) {
      cent = recompute(data.map(t => distance(t, cent, numCents))
                 .map(t => minimum(t._1, t._2))
                 .aggregate(new Array[Float](resSize))(reassign(_, _),
                     add(_, _)), cent, numCents, numAttrs)
      i += 1
    }
    val stop = System.nanoTime()

    println("runtime: " + (stop - start))
  }

  def parse(line: String): Array[Float] = {
    return line.split(',').map(_.toFloat).toArray
  }

  def add(a1: Array[Float], a2: Array[Float]): Array[Float] = {
    var i = 0
    while (i < a1.length) {
      a1(i) += a2(i)
      i += 1
    }
    return a1
  }

  def centroids(data: RDD[Array[Float]], numCents: Int): Array[Float] = {
    val res = data.take(numCents)
    var cent = new Array[Float](numCents * res(0).length)
    var i = 0
    while (i < res.length) {
      var j = 0
      while (j < res(i).length) {
        cent(i * res(i).length + j) = res(i)(j)
        j += 1
      }
      i += 1
    }
    return cent
  }

  def distance(data: Array[Float], cent: Array[Float], numCents: Int):
        (Array[Float],Array[Float]) = {
    var dist = Array.fill[Float](numCents)(0.0f)
    var i = 0
    while (i < numCents) {
      var j = 0
      while (j < data.length) {
        val diff = cent(i * data.length + j) - data(j)
        dist(i) += diff * diff
        j += 1
      }
      dist(i) = sqrt(dist(i)).toFloat
      i += 1
    }
    return (data,dist)
  }

  def minimum(data: Array[Float], dist: Array[Float]): (Int,Array[Float]) = {
    var min = dist(0)
    var assign = 0
    var i = 1
    while (i < dist.length) {
      if (dist(i) < min) {
        min = dist(i)
        assign = i
      }
      i += 1
    }
    return (assign,data)
  }

  def reassign(r: Array[Float], d: (Int,Array[Float])): Array[Float] = {
    var i = 0
    while (i < d._2.length) {
      r(d._1 * (d._2.length + 1) + i) += d._2(i)
      i += 1
    }
    r(d._1 * (d._2.length + 1) + d._2.length) += 1
    return r
  }

  def recompute(r: Array[Float], cent: Array[Float], numCents: Int, numAttrs: Int):
        Array[Float] = {
    var i = 0
    while (i < numCents) {
      var j = 0
      while (j < numAttrs) {
        cent(i * numAttrs + j) = r(i * (numAttrs + 1) + j) /
                                 r(i * (numAttrs + 1) + numAttrs)
        j += 1
      }
      i += 1
    }
    return cent
  }
}
