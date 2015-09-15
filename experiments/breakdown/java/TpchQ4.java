import java.io.BufferedReader;
import java.io.FileReader;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

public class TpchQ4 {
    public static final double NSEC = 0.000000001;
    public static final int Q4_ORDERS = 15000000;
    public static final int Q4_LINEITEM = 59986052;
    public static final long Q4_DATE1 = 741499200;
    public static final long Q4_DATE2 = 749448000;

    public static void main(String[] args) {
        int[] o_orderkey = new int[Q4_ORDERS];
        long[] o_orderdate = new long[Q4_ORDERS];
        char[] o_orderpriority = new char[Q4_ORDERS];
        int[] l_orderkey = new int[Q4_LINEITEM];
        long[] l_commitdate = new long[Q4_LINEITEM];
        long[] l_receiptdate = new long[Q4_LINEITEM];

        HashSet<Integer> exists = new HashSet<>();
        HashMap<Character,Integer> vals = new HashMap<>();

        try (
            BufferedReader br1 = new BufferedReader(new FileReader(args[0]));
            BufferedReader br2 = new BufferedReader(new FileReader(args[1]));
        ) {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
            String ln = "";
            for (int i = 0; (ln = br1.readLine()) != null; i++) {
                String[] toks = ln.split("\\|");
                o_orderkey[i] = Integer.parseInt(toks[0]);
                o_orderdate[i] = sdf.parse(toks[4]).getTime() / 1000;
                o_orderpriority[i] = toks[5].charAt(0);
            }

            for (int i = 0; (ln = br2.readLine()) != null; i++) {
                String[] toks = ln.split("\\|");
                l_orderkey[i] = Integer.parseInt(toks[0]);
                l_commitdate[i] = sdf.parse(toks[11]).getTime() / 1000;
                l_receiptdate[i] = sdf.parse(toks[12]).getTime() / 1000;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        long start = System.nanoTime();
        for (int i = 0; i < Q4_LINEITEM; i++)
            if (l_commitdate[i] < l_receiptdate[i])
                exists.add(l_orderkey[i]);
        for (int i = 0; i < Q4_ORDERS; i++)
            if (o_orderdate[i] >= Q4_DATE1 && o_orderdate[i] < Q4_DATE2)
                if (exists.contains(o_orderkey[i])) {
                    char key = o_orderpriority[i];
                    Integer val = vals.get(key);
                    if (val == null)
                        val = 0;
                    vals.put(key, val + 1);
                }
        long stop = System.nanoTime();

        System.err.println(vals);
        System.err.println("tpch_q4: " + ((stop - start) * NSEC));
    }
}
