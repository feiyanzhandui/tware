import java.io.BufferedReader;
import java.io.FileReader;

public class Bayes {
    public static final double NSEC = 0.000000001;
    public static final int DATA = 68682;//2747253;
    public static final int ATTR = 90;
    public static final int VALUE = 10;
    public static final int LABEL = 10;

    public static void main(String[] args) {
        int[] data = new int[DATA * (ATTR + 1)];
        int[] counts = new int[LABEL + LABEL * ATTR * VALUE];

        try (
            BufferedReader br = new BufferedReader(new FileReader(args[0]));
        ) {
            String ln = "";

            //data
            for (int i = 0; i < DATA && (ln = br.readLine()) != null; i++) {
                String[] toks = ln.split(",");
                int dataOff = i * ATTR;
                for (int j = 0; j <= ATTR; j++)
                    data[dataOff + j] = Integer.parseInt(toks[j]);
            }

            //counts
            for (int i = 0; i < LABEL + LABEL * ATTR * VALUE; i++)
                counts[i] = 0;
        }
        catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        long start = System.nanoTime();
        for (int i = 0; i < DATA; i++) {
            int dataOff = i * (ATTR + 1);

            //train
            int label = data[dataOff + ATTR];
            counts[label]++;
            int countOff = LABEL + label * ATTR * VALUE;
            for (int j = 0; j < ATTR; j++)
                counts[countOff + j * VALUE + data[dataOff + j]]++;
        }
        long stop = System.nanoTime();

        System.err.println("bayes: " + ((stop - start) * NSEC));
    }
}
