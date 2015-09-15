import java.io.BufferedReader;
import java.io.FileReader;

public class Kmeans {
    public static final double NSEC = 0.000000001;
    public static final int DATA = 749089;//29963548;
    public static final int ATTR = 2;
    public static final int CENT = 4;

    public static void main(String[] args) {
        float[] data = new float[DATA * ATTR];
        float[] cent = new float[CENT * ATTR];
        float[] temp = new float[CENT * (ATTR + 1)];
        float[] dist = new float[CENT];

        try (
            BufferedReader br = new BufferedReader(new FileReader(args[0]));
        ) {
            String ln = "";

            //data
            for (int i = 0; i < DATA && (ln = br.readLine()) != null; i++) {
                String[] toks = ln.split(",");
                int dataOff = i * ATTR;
                for (int j = 0; j < ATTR; j++)
                    data[dataOff + j] = Float.parseFloat(toks[j]);
            }

            //cent + temp
            for (int i = 0; i < CENT; i++) {
                int centOff = i * ATTR;
                int tempOff = i * (ATTR + 1);
                for (int j = 0; j < ATTR; j++) {
                    cent[centOff + j] = data[centOff + j];
                    temp[tempOff + j] = 0.0f;
                }
                temp[tempOff + ATTR] = 0.0f;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        long start = System.nanoTime();
        for (int i = 0; i < DATA * ATTR; i += ATTR) {
            //DATAOFF

            //distance
            for (int j = 0; j < CENT; j++) {
                int centOff = j * ATTR;
                for (int k = 0; k < ATTR; k++) {
                    float d = cent[centOff + k] - data[i + k];
                    dist[j] += d * d;
                }
                dist[j] = (float) Math.sqrt(dist[j]);
            }

            //minimum
            int assign = 0;
            float min = dist[0];
            for (int j = 1; j < CENT; j++)
                if (dist[j] < min) {
                    min = dist[j];
                    assign = j;
                }

            //reassign
            int tempOff = assign * (ATTR + 1);
            for (int j = 0; j < ATTR; j++)
                temp[tempOff + j] += data[i + j];
            temp[tempOff + ATTR]++;
        }
        long stop = System.nanoTime();

        System.err.println("kmeans: " + ((stop - start) * NSEC));
    }
}
