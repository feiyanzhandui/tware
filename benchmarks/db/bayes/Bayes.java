package db.bayes;

import java.sql.SQLException;
import java.sql.Statement;

import db.Benchmark;

public class Bayes {
    public static void main(String[] args) throws Exception {
        final String init = Benchmark.getQuery(args[0]);
        final String q1 = Benchmark.getQuery(args[1]);
        final String q2 = Benchmark.getQuery(args[2]);
        final String cleanup = Benchmark.getQuery(args[3]);
        new Benchmark() {
            public void init(Statement stmt) throws SQLException {
                stmt.executeUpdate(init);
            }

            public void execute(Statement stmt) throws SQLException {
                stmt.addBatch(q1);
                stmt.addBatch(q2);
                stmt.executeBatch();
            }

            public void cleanup(Statement stmt) throws SQLException {
                stmt.executeUpdate(cleanup);
            }
        }.run();
    }
}
