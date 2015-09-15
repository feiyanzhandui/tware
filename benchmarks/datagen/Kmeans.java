package datagen;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

import datagen.Util;

public class Kmeans {
    private static final long DIMENSIONS = 2;
    private static final float[][] CENTROIDS = {{2f,2f},
                                                {4f,4f},
                                                {6f,6f},
                                                {8f,8f}};

    public static void main(String[] args) throws IOException {
        String file = args[0];
        int size = Integer.parseInt(args[1]);
        long numData = size * Util.MB / Util.FLOAT / DIMENSIONS;
        Random rcentroid = new Random();
        Random rdata = new Random();
        BufferedWriter w = new BufferedWriter(new FileWriter(file));
        for (long i = 0; i < numData; i++) {
            int c = rcentroid.nextInt(CENTROIDS.length);
            float x = rdata.nextFloat() - 0.5f + CENTROIDS[c][0];
            float y = rdata.nextFloat() - 0.5f + CENTROIDS[c][1];
            w.write(x + "," + y + "\n");
        }
        w.close();
        System.out.println("numData: " + numData);
    }
}
