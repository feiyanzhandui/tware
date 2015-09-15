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

object Tpch4 {
  case class Order(o_orderkey: Int,
                   o_orderdate: Long,
                   o_orderpriority: Byte)
  case class Lineitem(l_orderkey: Int,
                      l_commitdate: Long,
                      l_receiptdate: Long)

  def main(args: Array[String]) {
    val filename1 = args(0)
    val filename2 = args(1)
    val compress = args(2)
    val numParts = args(3)
    val codegen = args(4)

    val conf = new SparkConf()
      .setAppName("tpch4")
      .set("spark.sql.inMemoryColumnarStorage.compressed", compress)
      .set("spark.sql.shuffle.partitions", numParts)
      .set("spark.sql.codegen", codegen)
    val sc = new SparkContext(conf)
    val sqlContext = new SQLContext(sc)
    import sqlContext.createSchemaRDD

    val fmt = new java.text.SimpleDateFormat("yyyy-MM-dd")
    val order = sc.textFile(filename1)
                  .map(line => line.split('|'))
                  .map(t => Order(t(0).toInt,
                                  fmt.parse(t(4)).getTime()/1000,
                                  t(5)(0).toByte))
    val lineitem = sc.textFile(filename2)
                     .map(line => line.split('|'))
                     .map(t => Lineitem(t(0).toInt,
                                        fmt.parse(t(10)).getTime()/1000,
                                        fmt.parse(t(12)).getTime()/1000))

    order.registerTempTable("orders")
    lineitem.registerTempTable("lineitem")
    sqlContext.cacheTable("orders")
    sqlContext.cacheTable("lineitem")
    val result = sqlContext.sql("""
                    select  o_orderpriority,
                            count(distinct o_orderkey) as order_count
                    from    orders
                    join    lineitem
                    on      l_orderkey = o_orderkey
                    where   l_commitdate < l_receiptdate
                        and o_orderdate >= 741499200
                        and o_orderdate < 749448000
                    group by o_orderpriority""")

    for (i <- 1 to 10) {
      val start = System.nanoTime()
      result.collect()
      val stop = System.nanoTime()
      println("compress=" + compress + ",numparts=" + numParts + ",codegen=" + codegen + ",time: " + (stop - start))
    }
    println(result.collect().foreach(println))
  }
}
