package db.pagerank;

import java.sql.SQLException;
import java.sql.Statement;

import db.Benchmark;

public class Pagerank {
    private static final int ITER = 20;

    public static void main(String[] args) throws Exception {
        final String init = Benchmark.getQuery(args[0]);
        final String q1 = Benchmark.getQuery(args[1]);
        final String cleanup = Benchmark.getQuery(args[2]);
        new Benchmark() {
            public void init(Statement stmt) throws SQLException {
                stmt.executeUpdate(init);
            }

            public void execute(Statement stmt) throws SQLException {
                for (int i = 0; i < ITER; i++)
                    stmt.executeUpdate(q1);
            }

            public void cleanup(Statement stmt) throws SQLException {
                stmt.executeUpdate(cleanup);
            }
        }.run();
    }
}
