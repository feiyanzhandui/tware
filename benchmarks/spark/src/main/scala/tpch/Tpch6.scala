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
import java.sql.Timestamp
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.sql
import org.apache.spark.sql._

object Tpch6 {
  case class Lineitem(l_quantity: Float,
                         l_extendedprice: Float,
                         l_discount: Float,
                         l_shipdate: Long)

  def main(args: Array[String]) {
    val filename = args(0)
    val compress = args(1)
    val numParts = args(2)
    val codegen = args(3)

    val conf = new SparkConf()
      .setAppName("tpch6")
      .set("spark.sql.inMemoryColumnarStorage.compressed", compress)
      .set("spark.sql.shuffle.partitions", numParts)
      .set("spark.sql.codegen", codegen)
    val sc = new SparkContext(conf)
    val sqlContext = new SQLContext(sc)
    import sqlContext.createSchemaRDD

    val fmt = new java.text.SimpleDateFormat("yyyy-MM-dd")
    val lineitem = sc.textFile(filename)
                     .map(line => line.split('|'))
                     .map(t => Lineitem(t(4).toFloat,
                                        t(5).toFloat,
                                        t(6).toFloat,
                                        fmt.parse(t(10)).getTime()/1000))
    println(lineitem.count())
    lineitem.registerTempTable("lineitem")
    sqlContext.cacheTable("lineitem")
    val result = sqlContext.sql("""
        select
                sum(l_extendedprice * l_discount) as revenue
        from
                lineitem
        where
                l_shipdate >= 757400400
                and l_shipdate < 788936400
                and l_discount >= 0.06 - 0.01
                and l_discount <= 0.06 + 0.01
                and l_quantity < 24""")

    for (i <- 1 to 10) {
      val start = System.nanoTime()
      result.collect()
      val stop = System.nanoTime()
      println("compress=" + compress + ",numparts=" + numParts + ",codegen=" + codegen + ",time: " + (stop - start))
    }

    println(result.collect().foreach(println))
  }
}
