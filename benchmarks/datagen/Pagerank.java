package datagen;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

import datagen.Util;

public class Pagerank {
    public static int nextPage(Random r, int numPages) {
        return (int) Math.min(Math.abs(r.nextGaussian()) * numPages * 0.2,
                             numPages - 1);
    }

    public static void main(String[] args) throws IOException {
        String file1 = args[0];
        String file2 = args[1];
        int size = Integer.parseInt(args[2]);
        int numPages = Integer.parseInt(args[3]);
        long pageSize = numPages * (Util.INT + Util.FLOAT) / Util.MB;
        long numLinks = (size - pageSize) * Util.MB / (Util.INT * 2);
        Random r = new Random();
        BufferedWriter w1 = new BufferedWriter(new FileWriter(file1));
        BufferedWriter w2 = new BufferedWriter(new FileWriter(file2));
        StringBuilder buff;
        int[] outLinks = new int[numPages];
        for (long i = 0; i < numLinks; i++) {
            buff = new StringBuilder();
            int outPage = nextPage(r, numPages);
            outLinks[outPage]++;
            buff.append(outPage);
            buff.append(',');
            int inPage;
            do {
                inPage = nextPage(r, numPages);
            } while (inPage == outPage);
            buff.append(inPage);
            buff.append('\n');
            w1.write(buff.toString());
        }
        w1.close();
        for (int i = 0; i < numPages; i++) {
            buff = new StringBuilder();
            buff.append(i);
            buff.append(',');
            buff.append(outLinks[i] > 0 ? outLinks[i] : 1);
            buff.append(',');
            buff.append(1.0);
            buff.append('\n');
            w2.write(buff.toString());
        }
        w2.close();
        System.out.println("numPages: " + numPages);
        System.out.println("numLinks: " + numLinks);
    }
}
