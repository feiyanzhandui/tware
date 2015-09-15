import java.io.BufferedReader;
import java.io.FileReader;
import java.text.SimpleDateFormat;
import java.util.HashMap;

public class TpchQ1 {
    public static final double NSEC = 0.000000001;
    public static final int LINEITEM = 59986052;
    public static final long DATE1 = 904708800;

    public static void main(String[] args) {
        char[] l_returnflag = new char[LINEITEM];
        char[] l_linestatus = new char[LINEITEM];
        float[] l_quantity = new float[LINEITEM];
        float[] l_extendedprice = new float[LINEITEM];
        float[] l_discount = new float[LINEITEM];
        float[] l_tax = new float[LINEITEM];
        long[] l_shipdate = new long[LINEITEM];

        HashMap<Short,Q1Tuple> vals = new HashMap<>();

        try (
            BufferedReader br = new BufferedReader(new FileReader(args[0]));
        ) {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
            String ln = "";
            for (int i = 0; (ln = br.readLine()) != null; i++) {
                String[] toks = ln.split("\\|");
                l_quantity[i] = Float.parseFloat(toks[4]);
                l_extendedprice[i] = Float.parseFloat(toks[5]);
                l_discount[i] = Float.parseFloat(toks[6]);
                l_tax[i] = Float.parseFloat(toks[7]);
                l_returnflag[i] = toks[8].charAt(0);
                l_linestatus[i] = toks[9].charAt(0);
                l_shipdate[i] = sdf.parse(toks[10]).getTime() / 1000;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        long start = System.nanoTime();
        for (int i = 0; i < LINEITEM; i++)
            if (l_shipdate[i] <= DATE1) {
                short key = (short) (l_returnflag[i] << 8);
                key |= l_linestatus[i];
                Q1Tuple val = vals.get(key);
                if (val == null) {
                    val = new Q1Tuple(l_returnflag[i], l_returnflag[i]);
                    vals.put(key, val);
                }
                val.r_quantity += l_quantity[i];
                val.r_extendedprice += l_extendedprice[i];
                val.r_discountprice += l_extendedprice[i] * (1 - l_discount[i]);
                val.r_discounttax += l_extendedprice[i] * (1 - l_discount[i])
                                   * (1 + l_tax[i]);
                val.r_discount += l_discount[i];
            }
        long stop = System.nanoTime();

        System.err.println(vals);
        System.err.println("tpch_q1: " + ((stop - start) * NSEC));
    }

    private static class Q1Tuple {
        public final char r_returnflag;
        public final char r_linestatus;
        public float r_quantity;
        public float r_extendedprice;
        public float r_discountprice;
        public float r_discounttax;
        public float r_discount;

        public Q1Tuple(char r_returnflag, char r_linestatus) {
            this.r_returnflag = r_returnflag;
            this.r_linestatus = r_linestatus;
            r_quantity = 0.0f;
            r_extendedprice = 0.0f;
            r_discountprice = 0.0f;
            r_discounttax = 0.0f;
            r_discount = 0.0f;
        }

        public String toString() {
            return "(" + r_returnflag + ","
                       + r_linestatus + ","
                       + r_quantity + ","
                       + r_extendedprice + ","
                       + r_discountprice + ","
                       + r_discounttax + ","
                       + r_discount + ")";
        }
    }
}
