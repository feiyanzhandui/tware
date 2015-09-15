package db;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public abstract class Benchmark {
    private static final double NSEC = 1000000000.0;

    public static final String MONETDB_DRV = "nl.cwi.monetdb.jdbc.MonetDriver";
    public static final String MONETDB_STR = "jdbc:monetdb://localhost/bench";
    public static final String MONETDB_USR = "monetdb";
    public static final String MONETDB_PWD = "monetdb";

    public static String getQuery(String file) throws IOException {
        return new String(Files.readAllBytes(Paths.get(file)));
    }

    public void run() throws ClassNotFoundException {
        Class.forName(MONETDB_DRV);
        String str = MONETDB_STR;
        String usr = MONETDB_USR;
        String pwd = MONETDB_PWD;
        try (
            Connection conn = DriverManager.getConnection(str, usr, pwd);
            Statement stmt = conn.createStatement();
        ) {
            long init1 = System.nanoTime();
            init(stmt);
            long init2 = System.nanoTime();
            long exec1 = System.nanoTime();
            execute(stmt);
            long exec2 = System.nanoTime();
            long clean1 = System.nanoTime();
            cleanup(stmt);
            long clean2 = System.nanoTime();
            System.out.println("RUNTIME");
            System.out.println("  init:    " + ((init2 - init1) / NSEC));
            System.out.println("  execute: " + ((exec2 - exec1) / NSEC));
            System.out.println("  cleanup: " + ((clean2 - clean1) / NSEC));
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public abstract void init(Statement stmt) throws SQLException;
    public abstract void execute(Statement stmt) throws SQLException;
    public abstract void cleanup(Statement stmt) throws SQLException;
}
