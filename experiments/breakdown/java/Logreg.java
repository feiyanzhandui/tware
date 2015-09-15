import java.io.BufferedReader;
import java.io.FileReader;
import static java.lang.Math.exp;

public class Logreg {
    public static final double NSEC = 0.000000001;
    public static final int DATA = 68682;//2747253;
    public static final int ATTR = 90;

    public static void main(String[] args) {
        float[] d = new float[DATA * (ATTR + 1)];
        float[] w = new float[ATTR];
        float[] g = new float[ATTR];

        try (
            BufferedReader br = new BufferedReader(new FileReader(args[0]));
        ) {
            String ln = "";

            //d
            for (int i = 0; i < DATA && (ln = br.readLine()) != null; i++) {
                String[] toks = ln.split(",");
                int dOff = i * ATTR;
                for (int j = 0; j <= ATTR; j++)
                    d[dOff + j] = Float.parseFloat(toks[j]);
            }

            //w + g
            for (int i = 0; i < ATTR; i++) {
                w[i] = 0.5f;
                g[i] = 0f;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        long start = System.nanoTime();
        for (int i = 0; i < DATA; i++) {
            int dOff = i * (ATTR + 1);

            //train
            float dot = 0f;
            for (int j = 0; j < ATTR; j++)
                dot += d[dOff + j] * w[j];
            float label = d[dOff + ATTR];
            float scale = (float) (1f / (1f + exp(-label * dot)) - 1f) * label;
            for (int j = 0; j < ATTR; j++)
                g[j] += scale * d[dOff + j];
        }
        long stop = System.nanoTime();

        System.err.println("logreg: " + ((stop - start) * NSEC));
    }
}
