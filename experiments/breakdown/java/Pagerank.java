import java.io.BufferedReader;
import java.io.FileReader;
import static java.lang.Math.exp;
import static java.lang.Math.random;

public class Pagerank {
    public static final double NSEC = 0.000000001;
    public static final int LINK = 3254010;//130160392;
    public static final int PAGE = 5716810;

    public static void main(String[] args) {
        int[] links = new int[LINK * 2];
        int[] counts = new int[PAGE];
        float[] ranks = new float[PAGE];
        float[] temp = new float[PAGE];

        try (
            BufferedReader br1 = new BufferedReader(new FileReader(args[0]));
            BufferedReader br2 = new BufferedReader(new FileReader(args[1]));
        ) {
            String ln = "";

            //links
            for (int i = 0; i < LINK && (ln = br1.readLine()) != null; i++) {
                String[] toks = ln.split(",");
                int linkOff = i * 2;
                links[linkOff] = Integer.parseInt(toks[0]);
                links[linkOff + 1] = Integer.parseInt(toks[1]);
            }

            //counts
            for (int i = 0; i < PAGE && (ln = br2.readLine()) != null; i++) {
                String[] toks = ln.split(",");
                int id = Integer.parseInt(toks[0]);
                counts[id] = Integer.parseInt(toks[1]);
            }

            //ranks + temp
            for (int i = 0; i < PAGE; i++) {
                ranks[i] = 1.0f;
                temp[i] = 0.0f;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        long start = System.nanoTime();
        for (int i = 0; i < LINK; i++) {
            int linkOff = i * 2;

            //train
            int src = links[linkOff];
            int dst = links[linkOff + 1];
            temp[dst] += ranks[src] / counts[src];
        }
        long stop = System.nanoTime();

        for (int i = 0; i < 20; i++)
            System.out.println("temp[" + i + "]=" + temp[i]);

        System.err.println("pagerank: " + ((stop - start) * NSEC));
    }
}
