import java.io.BufferedReader;
import java.io.FileReader;
import java.text.SimpleDateFormat;

public class TpchQ6 {
    public static final double NSEC = 0.000000001;
    public static final int LINEITEM = 59986052;
    public static final long DATE1 = 757400400;
    public static final long DATE2 = 788936400;

    public static void main(String[] args) {
        float[] l_quantity = new float[LINEITEM];
        float[] l_extendedprice = new float[LINEITEM];
        float[] l_discount = new float[LINEITEM];
        long[] l_shipdate = new long[LINEITEM];

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
                l_shipdate[i] = sdf.parse(toks[10]).getTime() / 1000;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        long start = System.nanoTime();
        float revenue = 0.0f;
        for (int i = 0; i < LINEITEM; i++)
            if (l_shipdate[i] >= DATE1
             && l_shipdate[i] < DATE2
             && l_discount[i] >= 0.05f
             && l_discount[i] <= 0.07f
             && l_quantity[i] < 24.0f)
                revenue += l_extendedprice[i] * l_discount[i];
        long stop = System.nanoTime();

        System.err.println("revenue=" + revenue + "\n");
        System.err.println("tpch_q6: " + ((stop - start) * NSEC));
    }
}
